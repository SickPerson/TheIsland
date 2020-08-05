#include "stdafx.h"
#include "TestScene.h"
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/ToolCamScript.h>
#include <Engine/Light3D.h>
#include <Engine/RenderMgr.h>

CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

void CTestScene::Init()
{
	// ==================
	// Camera Object 생성
	// ==================
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName( L"MainCam" );
	pMainCam->AddComponent( new CTransform );
	pMainCam->AddComponent( new CCamera );
	pMainCam->AddComponent( new CToolCamScript );

	pMainCam->Transform()->SetLocalPos( Vec3( 0.f, 100.f, 0.f ) );
	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));

	pMainCam->Camera()->SetProjType( PROJ_TYPE::PERSPECTIVE );
	pMainCam->Camera()->SetFar( 100000.f );
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck( 30, false );
	pMainCam->Camera()->SetLayerCheck( 29, false );
	m_pScene->FindLayer( L"Default" )->AddGameObject( pMainCam );


	// ====================
	// UI Camera
	// ====================
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CCamera );

	pObject->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC ); // 직교 투영
	pObject->Camera()->SetFar( 10000.f );
	pObject->Camera()->SetLayerCheck( 30, true );
	pObject->Camera()->SetWidth( CRenderMgr::GetInst()->GetResolution().fWidth );
	pObject->Camera()->SetHeight( CRenderMgr::GetInst()->GetResolution().fHeight );

	m_pScene->GetLayer( 0 )->AddGameObject( pObject );

	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CLight3D );

	pObject->Light3D()->SetLightPos( Vec3( 0.f, 500.f, 0.f ) );
	pObject->Light3D()->SetLightType( LIGHT_TYPE::DIR );
	pObject->Light3D()->SetDiffuseColor( Vec3( 1.f, 1.f, 1.f ) );
	pObject->Light3D()->SetSpecular( Vec3( 0.4f, 0.4f, 0.4f ) );
	pObject->Light3D()->SetAmbient( Vec3( 0.1f, 0.1f, 0.1f ) );
	pObject->Light3D()->SetLightDir( Vec3( 1.f, -1.f, 1.f ) );
	pObject->Light3D()->SetLightRange( 1000.f );

	//pObject->Transform()->SetLocalPos(Vec3(-4200.f, 2800.f, -1250.f));
	pObject->Transform()->SetLocalPos( Vec3( -1000.f, 1000.f, -1000.f ) );

	m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );
}

void CTestScene::Update()
{
}
