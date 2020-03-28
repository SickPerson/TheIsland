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

#include <Engine/FontMgr.h>
#include <iostream>

#include "ChatScript.h"
#include "InputScript.h"

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
	Ptr<CTexture> pFont = CResMgr::GetInst()->Load<CTexture>(L"FontTex", L"Texture\\Font\\Font_0.png");

	CGameObject* pObject = NULL;

	// ====================
	// UI Camera
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CCamera );

	pObject->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC ); // 직교 투영
	pObject->Camera()->SetFar( 10000.f );
	pObject->Camera()->SetLayerCheck( 30, true );

	m_pScene->GetLayer( 0 )->AddGameObject( pObject );

	int a = 1;
	// ===================
	//		  ID창
	// ===================
	m_pID = new CGameObject;
	m_pID->SetName(L"ID");
	m_pID->AddComponent(new CTransform);
	m_pID->AddComponent(new CMeshRender);
	m_pID->FrustumCheck(false);

	m_pID->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	m_pID->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	m_pID->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pID->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	m_pID->AddComponent(new CChatScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pID);

	pObject = new CGameObject;
	pObject->SetName(L"ID Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -80.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pID->AddChild(pObject);

	// ===================
	//		Password창
	// ===================
	m_pPassword = new CGameObject;
	m_pPassword->SetName(L"Password");
	m_pPassword->AddComponent(new CTransform);
	m_pPassword->AddComponent(new CMeshRender);
	m_pPassword->FrustumCheck(false);

	m_pPassword->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	m_pPassword->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	m_pPassword->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pPassword->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	m_pPassword->AddComponent(new CChatScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pPassword);

	pObject = new CGameObject;
	pObject->SetName(L"Password Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -130.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pPassword->AddChild(pObject);

	// ===================
	//		  IP창
	// ===================
	m_pIP = new CGameObject;
	m_pIP->SetName(L"IP");
	m_pIP->AddComponent(new CTransform);
	m_pIP->AddComponent(new CMeshRender);
	m_pIP->FrustumCheck(false);

	m_pIP->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	m_pIP->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	m_pIP->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pIP->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	m_pIP->AddComponent(new CInputScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pIP);

	pObject = new CGameObject;
	pObject->SetName(L"IP Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -330.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pIP->AddChild(pObject);
	
	float fontSize = 500.f / 15.f;

	for (int i = 0; i < 15; ++i)
	{
		pObject = new CGameObject;
		pObject->SetName(L"IP Font");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-250.f + (i * fontSize) + fontSize / 2.f, -330.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(fontSize, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pFont.GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
		m_pIP->AddChild(pObject);
		m_pIP->GetScript<CInputScript>()->AddInputObject(pObject);
		m_pIP->GetScript<CInputScript>()->SetRect(Rect(-250.f, -330.f, 500.f, 40.f));
	}

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
