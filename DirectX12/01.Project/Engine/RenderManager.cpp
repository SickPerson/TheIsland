#include "stdafx.h"
#include "RenderManager.h"
#include "Device.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "Light3D.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() : 
	m_arrRT{ },
	m_arrMRT{ }
{
}


CRenderManager::~CRenderManager()
{
}

void CRenderManager::Init( HWND hWnd, const tResolution & res, bool bWindow )
{
	m_tResolution = res;
	m_bWindow = bWindow;

	CreateSamplerState();

	CreateBlendState();

	CreateDepthStencilState();
}

void CRenderManager::Render()
{
	// 초기화
	float arrColor[4] = { 0.6f, 0.6f, 0.6f, 1.f };

	GET_SINGLE( CDevice )->Render_Start( arrColor );

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();

	// 카메라 
	for ( size_t i = 0; i < m_vecCamera.size(); ++i )
	{
		m_vecCamera[i]->Render();
	}

	// 출력
	GET_SINGLE( CDevice )->Render_Present();
}

void CRenderManager::CreateSamplerState()
{
}

void CRenderManager::CreateBlendState()
{
}

void CRenderManager::CreateDepthStencilState()
{
}

void CRenderManager::UpdateLight2D()
{
	static CConstantBuffer* pLight2DBuffer = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b3 );

	UINT iOffsetPos = pLight2DBuffer->AddData( &m_tLight2DInfo );
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pLight2DBuffer, iOffsetPos );

	m_tLight2DInfo.iCount = 0;
}

void CRenderManager::UpdateLight3D()
{
	static CConstantBuffer* pLight3DBuffer = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b4 );

	tLight3DInfo tLight3DArray;

	for ( size_t i = 0; i < m_vecLight3D.size(); ++i )
	{
		const tLight3D& info = m_vecLight3D[i]->GetLight3DInfo();
		tLight3DArray.arrLight3D[i] = info;
	}

	tLight3DArray.iCurCount = ( UINT )m_vecLight3D.size();

	UINT	iOffsetPos = pLight3DBuffer->AddData( &tLight3DArray );
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pLight3DBuffer, iOffsetPos );

	m_vecLight3D.clear();
}

void CRenderManager::RegisterLight2D( const tLight2D & Light2D )
{
	if ( 100 <= m_tLight2DInfo.iCount )
		return;

	m_tLight2DInfo.arrLight2D[m_tLight2DInfo.iCount++] = Light2D;
}

void CRenderManager::RegisterLight3D( CLight3D * pLight3D )
{
	if ( m_vecLight3D.size() >= 100 )
		return;

	m_vecLight3D.push_back( pLight3D );
}

CCamera * CRenderManager::GetCamera( int iIdx )
{
	return m_vecCamera[iIdx];
}

void CRenderManager::RegisterCamera( CCamera * pCamera )
{
	m_vecCamera.push_back( pCamera );
}

void CRenderManager::ClearCamera()
{
	m_vecCamera.clear();
}

tResolution CRenderManager::GetResolution()
{
	return m_tResolution;
}
