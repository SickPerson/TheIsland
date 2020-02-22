#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"

#include "Light3D.h"

CRenderMgr::CRenderMgr()
	: m_arrRT{}
	, m_arrMRT{}		
{	
}

CRenderMgr::~CRenderMgr()
{	
}

void CRenderMgr::Render()
{
	// 초기화
	float arrColor[4] = { 0.f,0.f, 0.f, 1.f };
	CDevice::GetInst()->Render_Start(arrColor);

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();

	// SwapChain MRT 초기화

	// DeferredMRT 초기화

	// 카메라 반복문 돌면서
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		// 물체 분류 작업 forward로 출력할것인지 deffered로 출력할 것인지

		// Deferred MRT 셋팅

		// Light MRT 셋팅

		// SwapChain MRT 셋팅		

		// 타겟이 SwapChain으로 바꼈으니 카메라 Render도 Forward로 변경해서 Rendering

		// Merge (Diffuse Target, Diffuse Light Target, Specular Light Target)
	}	

	// 출력
	CDevice::GetInst()->Render_present();
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
