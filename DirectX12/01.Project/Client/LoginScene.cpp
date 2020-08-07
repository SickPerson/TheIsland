#include "stdafx.h"
#include "LoginScene.h"

#include <Engine/Scene.h>
#include <Engine/Layer.h>

#include <Engine/GameObject.h>

#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Mesh.h>
#include <Engine/Font.h>

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

#include <Engine/Light3D.h>

#include <Engine/ToolCamScript.h>

CLoginScene::CLoginScene() :
	m_pID(NULL),
	m_pPassword(NULL),
	m_vFontColor(Vec4(0.f, 0.f, 0.f, 1.f)),
	m_vFontBackColor(Vec4(0.3f, 0.3f, 0.3f, 0.0f))

{
}


CLoginScene::~CLoginScene()
{
}

void CLoginScene::Init()
{
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");

	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());

	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());

	CGameObject* pObject = NULL;

	// ==================
	// Camera Object 생성
	// ==================
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCam");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CToolCamScript);

	pMainCam->Transform()->SetLocalPos(Vec3(0.f, 100.f, -2000.f));
	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetFar(100000.f);
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck(30, false);
	pMainCam->Camera()->SetLayerCheck(29, false);
	m_pScene->FindLayer(L"Default")->AddGameObject(pMainCam);


	// ====================
	// UI Camera
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CCamera );

	pObject->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC ); // 직교 투영
	pObject->Camera()->SetFar( 10000.f );
	pObject->Camera()->SetLayerCheck( 30, true );
	pObject->Camera()->SetWidth(CRenderMgr::GetInst()->GetResolution().fWidth);
	pObject->Camera()->SetHeight(CRenderMgr::GetInst()->GetResolution().fHeight);

	m_pScene->GetLayer( 0 )->AddGameObject( pObject );

	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight3D);

	pObject->Light3D()->SetLightPos(Vec3(0.f, 500.f, 0.f));
	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light3D()->SetSpecular(Vec3(0.4f, 0.4f, 0.4f));
	pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pObject->Light3D()->SetLightRange(1000.f);

	//pObject->Transform()->SetLocalPos(Vec3(-4200.f, 2800.f, -1250.f));
	pObject->Transform()->SetLocalPos(Vec3(-1000.f, 1000.f, -1000.f));

	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);

	CreateLoginWorld();

	CreateInputID();
	CreateInputIP();
	//CSceneMgr::GetInst()->CreateMRTUI();

	Ptr<CTexture> pTitle = CResMgr::GetInst()->Load<CTexture>(L"Title", L"Texture\\Title1.png");
	pObject = new CGameObject;
	pObject->SetName(L"Title Image");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(0.f, 270.f, 1.f));
	pObject->Transform()->SetLocalScale(Vec3(800.f, 260.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"IconMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTitle.GetPointer());
	float fa = 1.f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fa);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

	//pObject = new CGameObject;
	//pObject->SetName(L"Help Message");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CConstStringScript);

	//pObject->Transform()->SetLocalPos(Vec3(280.f, 220.f, 1.f));
	//pObject->Transform()->SetLocalScale(Vec3(25.f, 40, 1.f));

	//pObject->GetScript<CConstStringScript>()->Init("DEMO", m_vFontColor, m_vFontBackColor);

	//m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
}

void CLoginScene::Update()
{
	/*if (CNetwork::GetInst()->GetLogin() && CNetwork::GetInst()->GetLoopStart()) {
		NextScene();
	}*/
	if (KEY_TAB(KEY_TYPE::KEY_ENTER))
	{
		string strID = m_pID->GetScript<CInputScript>()->GetString(); // ID
		string strIP = m_pIP->GetScript<CInputScript>()->GetString(); // IP

		wstring wStrID;
		wStrID.assign(strID.begin(), strID.end());

		if (CNetwork::GetInst()->ConnectServer(strIP))
		{
			cout << "==================" << endl;
			cout << "IP Connect Success" << endl;
			cout << "==================" << endl;
			NextScene();
			CNetwork::GetInst()->Send_Login_Packet(wStrID);
		}
		else
		{
			cout << "d" << endl;
			return;
		}
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

void CLoginScene::CreateLoginWorld()
{
	// ====================
	// Skybox 오브젝트 생성
	// ====================
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>(L"Sky01", L"Texture\\Skybox\\Sky01.png");
	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());

	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// MeshRender 설정
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());
	float fLight = 1.f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);

	// AddGameObject
	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);

	// =====================================================================================================
	// Sea
	pObject = new CGameObject;
	pObject->SetName(L"Water");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Material 값 셋팅
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"AdvancedWaterMtrl");
	
	float tessellation = 128.f;

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &tessellation);

	float fHeight = 50.f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &fHeight);

	float fCull = 0.8f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &fCull);

	float fSeaLight = 1.f;
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_3, &fSeaLight);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -200.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(20000.f, 20000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);
	// =====================================================================================================

	// =====================================================================================================
	// Island
	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>(L"Tile", L"Texture\\Tile\\TILE_03.tga");
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>(L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga");
	pObject = new CGameObject;
	pObject->SetName(L"Island");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -250.f, 6000.f));
	pObject->Transform()->SetLocalScale(Vec3(3000.f, 500.f, 1500.f));

	// Material 값 셋팅
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl")->Clone();
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());
	pObject->MeshRender()->SetDynamicShadow(true);


	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);
	// =====================================================================================================

	// =====================================================================================================
	// Tree
	Ptr<CMeshData> pTreeATex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sprucea.mdat", L"MeshData\\sprucea.mdat");
	Ptr<CMeshData> pTreeBTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\spruceb.mdat", L"MeshData\\spruceb.mdat");
	Ptr<CMeshData> pTreeCTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sprucec.mdat", L"MeshData\\sprucec.mdat");
	CGameObject* pTestObject = NULL;
	for (int i = 0; i < 30; ++i)
	{
		int type = rand() % 3;
		Vec3 vPos = Vec3((float)(rand() % 4000 - 2000), 20.f, 5500.f + (float)(rand() % 1500 - 750));
		float fScale = (float)(rand() % 10 + 30);

		if (type == 0)
		{
			pTestObject = pTreeATex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 0);
		}
		else if (type == 1)
		{
			pTestObject = pTreeBTex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 1);
		}
		else
		{
			pTestObject = pTreeCTex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 0);
		}

		pTestObject->SetName(L"Tree");
		pTestObject->Transform()->SetLocalPos(vPos);
		pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pTestObject->Transform()->SetLocalScale(Vec3(fScale, fScale, fScale));
		pTestObject->FrustumCheck(false);
		pTestObject->MeshRender()->SetDynamicShadow(true);

		m_pScene->FindLayer(L"Default")->AddGameObject(pTestObject);

		// 반사 나무
		if (type == 0)
		{
			pTestObject = pTreeATex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 0);
		}
		else if (type == 1)
		{
			pTestObject = pTreeBTex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 1);
		}
		else
		{
			pTestObject = pTreeCTex->Instantiate();
			pTestObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TreeMtrl"), 0);
		}

		pTestObject->SetName(L"Tree");
		pTestObject->Transform()->SetLocalPos(Vec3(vPos.x, -vPos.y, vPos.z));
		pTestObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, XM_PI, 0.f));
		pTestObject->Transform()->SetLocalScale(Vec3(fScale, fScale, fScale));

		m_pScene->FindLayer(L"Default")->AddGameObject(pTestObject);
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
	m_pID->AddComponent(new CFont);
	m_pID->FrustumCheck(false);

	m_pID->Transform()->SetLocalPos(Vec3(0.f, -260.f, 1.f));
	m_pID->Transform()->SetLocalScale(Vec3(500.f, 60.f, 1.f));
	m_pID->AddComponent(new CInputScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pID);
	m_pID->GetScript<CInputScript>()->SetRect(Vec4(-250.f, -290.f, 250.f, -230.f));
	m_pID->GetScript<CInputScript>()->SetMaxCount(10);
	m_pID->GetScript<CInputScript>()->SetEnable(true);
	m_pID->Font()->SetString(" ");

	// ID Info 
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-320.f, -260.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 60.f, 1.f));

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
		Vec4 vFontColor = m_vFontColor;
		Vec4 vFontBackColor = m_vFontBackColor;
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"ID Info");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->FrustumCheck(false);

		pObject->Transform()->SetLocalPos(Vec3(-280.f, -260.f, 1.f));
		pObject->Transform()->SetLocalScale(Vec3(40.f, 60.f, 1.f));

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
		Vec4 vFontColor = m_vFontColor;
		Vec4 vFontBackColor = m_vFontBackColor;
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
	m_pIP->AddComponent(new CFont);
	m_pIP->FrustumCheck(false);

	m_pIP->Transform()->SetLocalPos(Vec3(0.f, -330.f, 1.f));
	m_pIP->Transform()->SetLocalScale(Vec3(500.f, 40.f, 1.f));
	m_pIP->AddComponent(new CInputScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(m_pIP);
	m_pIP->GetScript<CInputScript>()->SetRect(Vec4(-250.f, -350.f, 250.f, -310.f));
	m_pIP->GetScript<CInputScript>()->SetEnable(false);
	m_pIP->Font()->SetString(" ");

	// IP Info 
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"IP Info");
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
		Vec4 vFontColor = m_vFontColor;
		Vec4 vFontBackColor = m_vFontBackColor;
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &vFontBackColor);
		pObject->MeshRender()->SetMaterial(pMtrl);
	}
	{
		CGameObject* pObject = new CGameObject;
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
		Vec4 vFontColor = m_vFontColor;
		Vec4 vFontBackColor = m_vFontBackColor;
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
