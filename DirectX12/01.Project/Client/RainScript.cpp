#include "stdafx.h"
#include "RainScript.h"


#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>

#include <Engine/FPSCamScript.h>

#include <iostream>

CRainScript::CRainScript() : 
	CScript((UINT )SCRIPT_TYPE::WORLDSCRIPT),
	m_bActive(false),
	m_fAlpha(1.f)
{
}

CRainScript::~CRainScript()
{
}

void CRainScript::Update()
{
	CGameObject* pRain = CSceneMgr::GetInst()->GetCurScene()->GetLayer( 0 )->FindObject( L"Rain" );
	
	if ( !pRain )
		m_bActive = false;
	
	else
		m_bActive = pRain->IsActive();

	CGameObject* pMainCam = CSceneMgr::GetInst()->GetCurScene()->GetLayer( 0 )->GetMainCamera();
	Vec3 vCamRot = pMainCam->Transform()->GetLocalRot();

	float vCamRotMinX = -XM_PI / 2.f;
	float vCamRotMaxX = -XM_PI / 6.f;

	if ( m_bActive )
	{
		if ( vCamRot.x <= vCamRotMaxX )
			MeshRender()->SetActive( true );

		else
			MeshRender()->SetActive( false );
	}

	else
	{
		MeshRender()->SetActive( false );
	}

	float vCamRotSub = vCamRot.x - vCamRotMaxX;
	float fAlpha = ( vCamRot.x - vCamRotMinX ) / ( vCamRotMaxX - vCamRotMinX );

	fAlpha = 1.f - fAlpha;

	std::cout << fAlpha << std::endl;

	MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::FLOAT_0, &fAlpha );
}
