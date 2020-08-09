#include "stdafx.h"
#include "RainScript.h"


#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>

#include <Engine/FPSCamScript.h>

CRainScript::CRainScript() : 
	CScript((UINT )SCRIPT_TYPE::WORLDSCRIPT),
	m_bActive(false)
{
}

CRainScript::~CRainScript()
{
}

void CRainScript::Update()
{
	CGameObject* pMainCam = CSceneMgr::GetInst()->GetCurScene()->GetLayer( 0 )->GetMainCamera();
	//CFPSCamScript* pCamScript = pMainCam->GetScript<CFPSCamScript>();

	Vec3 vCamRot = pMainCam->Transform()->GetLocalRot();

	if ( m_bActive )
	{
		if ( vCamRot.x <= -XM_PI / 4 )
			MeshRender()->SetActive( true );

		else
			MeshRender()->SetActive( false );
	}

	if ( KEY_TAB( KEY_TYPE::KEY_L ) )
	{
		m_bActive = !m_bActive;
	}
}
