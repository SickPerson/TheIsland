#include "stdafx.h"
#include "MapToolScene.h"

#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/ToolCamScript.h>
#include <Engine/Light3D.h>
#include <Engine/LandScape.h>
#include <Engine/NaviMgr.h>



CMapToolScene::CMapToolScene()
{
}


CMapToolScene::~CMapToolScene()
{
}

void CMapToolScene::Init()
{

	// ==================
	// Camera Object 생성
	// ==================
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName( L"MainCam" );
	pMainCam->AddComponent( new CTransform );
	pMainCam->AddComponent( new CCamera );
	pMainCam->AddComponent( new CToolCamScript );
	CToolCamScript* pScript = pMainCam->GetScript<CToolCamScript>();
	pScript->SetTool( true );

	pMainCam->Transform()->SetLocalPos( Vec3( 0.f, 100.f, 0.f ) );
	pMainCam->Transform()->SetLocalRot( Vec3( XM_PI / 2, 0.f, 0.f ) );

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
	// 3D Light Object 추가
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

	// =======================
	// LandScape 오브젝트 생성
	// =======================
	Ptr<CTexture> pLandScape = CResMgr::GetInst()->Load<CTexture>( L"Grass", L"Texture\\LandScape\\GRASS_00+SAND.bmp" );

	pObject = new CGameObject;
	pObject->SetName( L"LandScape Object" );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );
	pObject->AddComponent( new CLandScape );
	pObject->LandScape()->CreateLandScape( L"Texture/ISLAND_110.bmp", 110, 110);
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"LandScapeMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"LandScapeMtrl" ) );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, pLandScape.GetPointer() );
	pObject->Transform()->SetLocalPos( Vec3( 0.f, 0.f, 0.f ) );
	pObject->Transform()->SetLocalScale( Vec3( 200.f, 400.f, 200.f ) );
	pObject->FrustumCheck( false );
	CNaviMgr::GetInst()->SetLandScape( pObject->LandScape() );
	m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );
}
