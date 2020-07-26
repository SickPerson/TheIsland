#include "stdafx.h"
#include "MapToolScene.h"

#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/ToolCamScript.h>
#include <Engine/Light3D.h>



CMapToolScene::CMapToolScene()
{
}


CMapToolScene::~CMapToolScene()
{
}

void CMapToolScene::Init()
{
	// ==================
	// Camera Object ����
	// ==================
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName( L"MainCam" );
	pMainCam->AddComponent( new CTransform );
	pMainCam->AddComponent( new CCamera );
	pMainCam->AddComponent( new CToolCamScript );

	pMainCam->Transform()->SetLocalPos( Vec3( 0.f, 0.f, -500.f ) );
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
	CGameObject* pUICam = new CGameObject;
	pUICam->AddComponent( new CTransform );
	pUICam->AddComponent( new CCamera );

	pUICam->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC );
	pUICam->Camera()->SetFar( 10000.f );
	pUICam->Camera()->SetLayerCheck( 30, true );

	m_pScene->FindLayer( L"Default" )->AddGameObject( pUICam );

	CSceneMgr::GetInst()->CreateMRTUI();

	CGameObject* pObject = NULL;

	// ====================
	// 3D Light Object �߰�
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CLight3D );

	pObject->Light3D()->SetLightPos( Vec3( 0.f, 500.f, 0.f ) );
	pObject->Light3D()->SetLightType( LIGHT_TYPE::DIR );
	pObject->Light3D()->SetDiffuseColor( Vec3( 1.f, 1.f, 1.f ) );
	pObject->Light3D()->SetSpecular( Vec3( 0.3f, 0.3f, 0.3f ) );
	pObject->Light3D()->SetAmbient( Vec3( 0.1f, 0.1f, 0.1f ) );
	pObject->Light3D()->SetLightDir( Vec3( 1.f, -1.f, 1.f ) );
	pObject->Light3D()->SetLightRange( 1000.f );

	m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );

	//Ptr<CMeshData> pRockTex = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\genericcliffb.mdat", L"MeshData\\genericcliffb.mdat" );
	//// ====================================================================
	//pObject = pRockTex->Instantiate();
	//pObject->SetName( L"Rock" );
	//pObject->FrustumCheck( false );
	//pObject->Transform()->SetLocalPos( Vec3( 0.f, 0.f, 0.f ) );
	//pObject->Transform()->SetLocalRot( Vec3( 0.f, 0.f, 0.f ) );
	//pObject->Transform()->SetLocalRot( Vec3( -XM_PI / 2.f, 0.f, 0.f ) );
	//pObject->Transform()->SetLocalScale( Vec3( 12.f, 12.f, 12.f ) );
	//m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );
	// ====================================================================
}
