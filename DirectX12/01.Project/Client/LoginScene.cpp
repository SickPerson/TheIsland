#include "stdafx.h"
#include "LoginScene.h"

#include <Engine/Scene.h>
#include <Engine/Layer.h>

#include <Engine/GameObject.h>

#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Mesh.h>

#include <Engine/QuickSlotScript.h>

#include <Engine/ResMgr.h>

#include "ChatScript.h"

CLoginScene::CLoginScene() :
	m_pID(NULL),
	m_pPassword(NULL)

{
}


CLoginScene::~CLoginScene()
{
}

void CLoginScene::Init()
{
	m_pScene->GetLayer( 0 )->SetName( L"Default" );
	m_pScene->GetLayer( 1 )->SetName( L"Player" );
	m_pScene->GetLayer( 2 )->SetName( L"Monster" );
	m_pScene->GetLayer( 3 )->SetName( L"Bullet" );
	m_pScene->GetLayer( 4 )->SetName( L"UI" );
	m_pScene->GetLayer( 31 )->SetName( L"Tool" );

	CGameObject* pObject = NULL;

	// ====================
	// UI Camera
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CCamera );

	pObject->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC ); // 직교 투영
	pObject->Camera()->SetFar( 10000.f );
	pObject->Camera()->SetLayerCheck( 4, true );

	m_pScene->GetLayer( 0 )->AddGameObject( pObject );

	// ===================
	//      Login창
	// ===================
	//pObject = new CGameObject;
	//pObject->SetName( L"Login Frame" );
	//pObject->AddComponent( new CTransform );
	//pObject->AddComponent( new CMeshRender );
	//pObject->FrustumCheck( false );

	//pObject->Transform()->SetLocalPos( Vec3( 0.f, -320.f, 1.f ) );
	//pObject->Transform()->SetLocalScale( Vec3( 500.f, 80.f, 1.f ) );

	//pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	//pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"UIMtrl" ) );

	int a = 1;
	//pObject->MeshRender()->GetCloneMaterial()->SetData( SHADER_PARAM::INT_0, &a );
	//pObject->AddComponent( new CQuickSlotScript );

	//m_pScene->FindLayer( L"UI" )->AddGameObject( pObject );

	m_pID = new CGameObject;
	m_pID->SetName( L"ID" );
	m_pID->AddComponent( new CTransform );
	m_pID->AddComponent( new CMeshRender );
	m_pID->FrustumCheck( false );

	m_pID->Transform()->SetLocalPos( Vec3( 0.f, -280.f, 1.f ) );
	m_pID->Transform()->SetLocalScale( Vec3( 500.f, 40.f, 1.f ) );

	m_pID->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	m_pID->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"UIMtrl" ) );
	
	m_pID->MeshRender()->GetCloneMaterial()->SetData( SHADER_PARAM::INT_0, &a );
	m_pID->AddComponent( new CChatScript );

	m_pScene->FindLayer( L"UI" )->AddGameObject( m_pID );

	m_pPassword = new CGameObject;
	m_pPassword->SetName( L"Password" );
	m_pPassword->AddComponent( new CTransform );
	m_pPassword->AddComponent( new CMeshRender );
	m_pPassword->FrustumCheck( false );
	
	m_pPassword->Transform()->SetLocalPos( Vec3( 0.f, -330.f, 1.f ) );
	m_pPassword->Transform()->SetLocalScale( Vec3( 500.f, 40.f, 1.f ) );
	
	m_pPassword->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	m_pPassword->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"UIMtrl" ) );
	
	m_pPassword->MeshRender()->GetCloneMaterial()->SetData( SHADER_PARAM::INT_0, &a );
	m_pPassword->AddComponent( new CChatScript );

	m_pScene->FindLayer( L"UI" )->AddGameObject( m_pPassword );
}

CGameObject * CLoginScene::GetIDObj()
{
	return m_pID;
}

CGameObject * CLoginScene::GetPasswordObj()
{
	return m_pPassword;
}

CChatScript * CLoginScene::GetID()
{
	return m_pID->GetScript< CChatScript >();
}

CChatScript * CLoginScene::GetPassword()
{
	return m_pPassword->GetScript< CChatScript >();
}
