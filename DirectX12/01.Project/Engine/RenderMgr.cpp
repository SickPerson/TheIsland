#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"
#include "ResMgr.h"

#include "Light3D.h"

#include "MRT.h"


CRenderMgr::CRenderMgr()
	: m_iRTVHeapSize(0)
	, m_arrMRT{}		
{	
}

CRenderMgr::~CRenderMgr()
{	
	Safe_Delete_Array( m_arrMRT );
}

void CRenderMgr::Render()
{
	// 초기화
	float arrColor[4] = { 0.f,0.f, 0.f, 1.f };
	CDevice::GetInst()->Render_Start(arrColor);

	static CConstantBuffer* pGlobalBuffer = CDevice::GetInst()->GetCB( CONST_REGISTER::b5 );
	CDevice::GetInst()->SetConstBufferToRegister( pGlobalBuffer, pGlobalBuffer->AddData( &g_global ) );

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();

	// SwapChain MRT 초기화
	UINT iIdx = CDevice::GetInst()->GetSwapchainIdx();
	m_arrMRT[( UINT )MRT_TYPE::SWAPCHAIN]->Clear( iIdx );

	// DeferredMRT 초기화
	m_arrMRT[( UINT )MRT_TYPE::DEFERRED]->Clear();
	//m_arrMRT[(UINT)MRT_TYPE::PLAYER]->Clear();

	// LightMRT 초기화
	m_arrMRT[( UINT )MRT_TYPE::LIGHT]->Clear();

	// ==================================
	// Main Camera 로 Deferred 렌더링 진행
	// ==================================
	m_vecCam[0]->SortGameObject();

	// Deferred MRT 셋팅
	m_arrMRT[( UINT )MRT_TYPE::DEFERRED]->OMSet();
	m_vecCam[0]->Render_Deferred();
	m_arrMRT[( UINT )MRT_TYPE::DEFERRED]->TargetToResBarrier();

	// Render Light
	Render_Lights();

	// Merge (Diffuse Target, Diffuse Light Target, Specular Target )		
	Merge_Light();

	// Forward Render
	m_vecCam[0]->Render_Forward(); // skybox, grid, ui

	//=================================
	// 추가 카메라는 forward render 만
	//=================================
	for ( int i = 1; i < m_vecCam.size(); ++i )
	{
		if (m_vecCam[i]->GetCamType() == CAM_TYPE::INVENTORY)
		{
			m_vecCam[i]->SortGameObject();

			//m_arrMRT[(UINT)MRT_TYPE::PLAYER]->OMSet();
			//m_vecCam[i]->Render_Deferred();

			m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(1, iIdx);
			m_vecCam[i]->Render_Forward();
		}
		else
		{
			m_vecCam[i]->SortGameObject();
			m_vecCam[i]->Render_Forward();
		}
	}

	// 출력
	CDevice::GetInst()->Render_Present();
}

void CRenderMgr::Render_tool()
{
	// 초기화
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	//Clear(arrColor);

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();
}

void CRenderMgr::Render_Lights()
{
	m_arrMRT[( UINT )MRT_TYPE::LIGHT]->OMSet();

	// 광원을 그린다.
	for ( UINT i = 0; i < m_vecLight3D.size(); ++i )
	{
		m_vecLight3D[i]->Light3D()->Render();
	}

	m_vecLight3D.clear();
	m_arrMRT[( UINT )MRT_TYPE::LIGHT]->TargetToResBarrier();

	// SwapChain MRT 셋팅
	UINT iIdx = CDevice::GetInst()->GetSwapchainIdx();
	m_arrMRT[( UINT )MRT_TYPE::SWAPCHAIN]->OMSet( 1, iIdx );
}

void CRenderMgr::Merge_Light()
{
	// Diffuse + Lights
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" );
	static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>( L"MergeLightMtrl" );

	pMtrl->UpdateData();
	pRectMesh->Render();
}

void CRenderMgr::UpdateLight2D()
{
	static CConstantBuffer* pLight2DBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b3);

	UINT iOffsetPos = pLight2DBuffer->AddData(&m_tLight2DInfo);
	CDevice::GetInst()->SetConstBufferToRegister(pLight2DBuffer, iOffsetPos);

	m_tLight2DInfo.iCount = 0;
}

void CRenderMgr::UpdateLight3D()
{
	static CConstantBuffer* pLight3DBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b4);
	
	tLight3DInfo tLight3DArray;

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		const tLight3D& info = m_vecLight3D[i]->GetLight3DInfo();
		tLight3DArray.arrLight3D[i] = info;
	}
	tLight3DArray.iCurCount = (UINT)m_vecLight3D.size();
	
	UINT iOffsetPos = pLight3DBuffer->AddData(&tLight3DArray);
	CDevice::GetInst()->SetConstBufferToRegister(pLight3DBuffer, iOffsetPos);
	
	m_vecLight3D.clear();
}

void CRenderMgr::RegisterLight2D( const tLight2D & _Light2D )
{
	if ( 100 <= m_tLight2DInfo.iCount )
	{
		return;
	}

	m_tLight2DInfo.arrLight2D[m_tLight2DInfo.iCount++] = _Light2D;
}

void CRenderMgr::RegisterLight3D( CLight3D * _pLight3D )
{
	if ( m_vecLight3D.size() >= 100 )
		return;
	m_vecLight3D.push_back( _pLight3D );
}

CMRT * CRenderMgr::GetMRT( MRT_TYPE eType )
{
	return m_arrMRT[(UINT)eType];
}

UINT CRenderMgr::GetRTVHeapSize()
{
	return m_iRTVHeapSize;
}
