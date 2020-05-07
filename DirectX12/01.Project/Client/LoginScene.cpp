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

#include "IngameScene.h"
#include <Engine/SceneMgr.h>
#include <Engine/RenderMgr.h>

#include "Network.h"
#include "ConstStringScript.h"

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

	CreateInputID();
	CreateInputIP();

	pObject = new CGameObject;
	pObject->SetName(L"Help Message");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CConstStringScript);

	pObject->Transform()->SetLocalPos(Vec3(-330.f, 280.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(62.f, 108.f, 1.f));

	pObject->GetScript<CConstStringScript>()->Init("THE ISLAND");

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"Help Message");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CConstStringScript);

	pObject->Transform()->SetLocalPos(Vec3(150.f, 200.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(25.f, 40, 1.f));

	pObject->GetScript<CConstStringScript>()->Init("DEMO");

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
}

void CLoginScene::Update()
{
	if (CNetwork::GetInst()->GetLogin() && CNetwork::GetInst()->GetLoopStart()) {
		NextScene();
	}
	if (KEY_TAB(KEY_TYPE::KEY_ENTER))
	{
		//NextScene();
		// 입력한 IP, ID를 받아옴
		string strIP = m_pIP->GetScript<CInputScript>()->GetString(); // IP
		string strID = m_pID->GetScript<CInputScript>()->GetString(); // ID
		// Sever Connect
		if (CNetwork::GetInst()->ConnectServer(strIP)) {
			cout << "IP Connect Success" << endl;
			if (!CNetwork::GetInst()->GetLoginSend()) {
				CNetwork::GetInst()->SetLoginSend(true);
				CNetwork::GetInst()->SendLoginPacket(strID);
			}
		}
		else
			cout << "IP Connect Failure" << endl;
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN))
	{
		tResolution tRes = CRenderMgr::GetInst()->GetResolution();
		POINT vPos = CKeyMgr::GetInst()->GetMousePos();
		Vec4 vBox = m_pIP->GetScript<CInputScript>()->GetRect();

		vPos.x -= tRes.fWidth / 2.f;
		vPos.y -= tRes.fHeight / 2.f;
		vPos.y *= -1.f;

		//std::cout << vPos.x << ", " << vPos.y << std::endl;

		// IP 창 클릭?
		if (vPos.x >= vBox.x && vPos.x <= vBox.z)
		{
			if (vPos.y >= vBox.y && vPos.y <= vBox.w)
			{
				//std::cout << "클릭!" << std::endl;
				m_pIP->GetScript<CInputScript>()->SetEnable(true);
				m_pID->GetScript<CInputScript>()->SetEnable(false);
			}
		}

		vBox = m_pID->GetScript<CInputScript>()->GetRect();

		// ID 창 클릭?
		if (vPos.x >= vBox.x && vPos.x <= vBox.z)
		{
			if (vPos.y >= vBox.y && vPos.y <= vBox.w)
			{
				//std::cout << "클릭!" << std::endl;
				m_pIP->GetScript<CInputScript>()->SetEnable(false);
				m_pID->GetScript<CInputScript>()->SetEnable(true);
			}
		}
	}
}

void CLoginScene::CreateInputID()
{
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
	m_pID->AddComponent(new CInputScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pID);

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"ID Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -270.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	int a = 1;
	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pID->AddChild(pObject);
	m_pID->GetScript<CInputScript>()->SetFontSpace(pObject);

	float fontSize = 500.f / 15.f;

	for (int i = 0; i < 15; ++i)
	{
		pObject = new CGameObject;
		pObject->SetName(L"ID Font");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-250.f + (i * fontSize) + fontSize / 2.f, -270.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(fontSize, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
		m_pID->AddChild(pObject);
		m_pID->GetScript<CInputScript>()->AddInputObject(pObject);
	}
	m_pID->GetScript<CInputScript>()->SetRect(Vec4(-250.f, -290.f, 250.f, -250.f));
	m_pID->GetScript<CInputScript>()->SetEnable(true);


	// ID Info 
	{
		pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-320.f, -270.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['I'];
		Ptr<CMaterial> pMtrl = pObject->MeshRender()->GetCloneMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec4 vFontBackColor = Vec4(0.f, 0.f, 0.f, 0.f);
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
	{
		pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-280.f, -270.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['D'];
		Ptr<CMaterial> pMtrl = pObject->MeshRender()->GetCloneMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec4 vFontBackColor = Vec4(0.f, 0.f, 0.f, 0.f);
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
}

void CLoginScene::CreateInputIP()
{
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

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"IP Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -330.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	int a = 1;
	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pIP->AddChild(pObject);
	m_pIP->GetScript<CInputScript>()->SetFontSpace(pObject);

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
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
		m_pIP->AddChild(pObject);
		m_pIP->GetScript<CInputScript>()->AddInputObject(pObject);
	}
	m_pIP->GetScript<CInputScript>()->SetRect(Vec4(-250.f, -350.f, 250.f, -310.f));
	m_pIP->GetScript<CInputScript>()->SetEnable(false);

	// IP Info 
	{
		pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-320.f, -330.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['I'];
		Ptr<CMaterial> pMtrl = pObject->MeshRender()->GetCloneMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec4 vFontBackColor = Vec4(0.f, 0.f, 0.f, 0.f);
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
	{
		pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-280.f, -330.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 40.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

		m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['P'];
		Ptr<CMaterial> pMtrl = pObject->MeshRender()->GetCloneMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec4 vFontBackColor = Vec4(0.f, 0.f, 0.f, 0.f);
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
}

void CLoginScene::CreateInputPassword()
{
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

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Password Bar");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->FrustumCheck(false);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -130.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	int a = 1;
	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	m_pPassword->AddChild(pObject);
}

void CLoginScene::NextScene()
{
	// 씬 넘어가기전 오브젝트들 정리
	m_pScene->FindLayer(L"UI")->RemoveAll();
	m_pScene->FindLayer(L"Default")->RemoveAll();

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CIngameScene* pGameScene = pScene->CreateSceneScript<CIngameScene>(L"GameScene");
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
