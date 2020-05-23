#include "stdafx.h"
#include "IngameScene.h"

#include <Engine/Device.h>

#include <Engine/Ptr.h>

#include <Engine/Texture.h>

#include <Engine/ResMgr.h>
#include <Engine/CollisionMgr.h>
#include <Engine/FontMgr.h>

#include <Engine/Layer.h>
#include <Engine/Scene.h>

#include <Engine/GameObject.h>
#include <Engine/MeshRender.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/Font.h>
#include <Engine/Light3D.h>
#include <Engine/LandScape.h>
#include <Engine/GridScript.h>

#include <Engine/ToolCamScript.h>
#include <Engine/MonsterScript.h>

#include "StatusScript.h"
#include "QuickSlotScript.h"
#include "PlayerCamScript.h"
#include "InventoryScript.h"
#include "SunshineScript.h"
#include "PlayerScript.h"
#include "FPSCamScript.h"

#include "InputScript.h"
#include "ChatScript.h"

#include <Engine/TestScript.h>

#include <Engine/ParticleSystem.h>

#include "Network.h"

CIngameScene::CIngameScene() 
	: m_pChat(NULL)
	, m_pPlayer(NULL)
{
}


CIngameScene::~CIngameScene()
{
}

void CIngameScene::Init()
{
	//Ptr<CMeshData> pTestMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\player.fbx");
	//pTestMeshData->Save(pTestMeshData->GetPath());
	//Ptr<CMeshData> pTestTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\player.mdat", L"MeshData\\player.mdat");
	//pTestMeshData = CResMgr::GetInst()->LoadFBX( L"FBX\\Wolf.fbx" );
	//pTestMeshData->Save( pTestMeshData->GetPath() );

	// MeshData 로드
	Ptr<CMeshData> pBearTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bear.mdat", L"MeshData\\bear.mdat");
	Ptr<CMeshData> pWolfTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\wolf.mdat", L"MeshData\\wolf.mdat");
	Ptr<CMeshData> pBoarTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\boar.mdat", L"MeshData\\boar.mdat");
	Ptr<CMeshData> pDeerTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\deer.mdat", L"MeshData\\deer.mdat");
	Ptr<CMeshData> pTreeATex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sprucea.mdat", L"MeshData\\sprucea.mdat");
	Ptr<CMeshData> pTreeBTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\spruceb.mdat", L"MeshData\\spruceb.mdat");
	Ptr<CMeshData> pTreeCTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\sprucec.mdat", L"MeshData\\sprucec.mdat");

	CGameObject * pTestObject = pBearTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.5f, 2.5f, 2.5f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->SetName(L"Bear");
	pTestObject->Transform()->SetLocalPos(Vec3(-300.f, 20.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	 // ===================================================================
	pTestObject = pWolfTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->SetName(L"Wolf");
	pTestObject->Transform()->SetLocalPos(Vec3(-600.f, 50.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================
	pTestObject = pBoarTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(60.f, 60.f, 60.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->SetName(L"Boar");
	pTestObject->Transform()->SetLocalPos(Vec3(300.f, 50.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================





	// ====================================================================
	pTestObject = pBearTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.5f, 2.5f, 2.5f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->SetName(L"Bear");
	pTestObject->Transform()->SetLocalPos(Vec3(20.f, 300.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ===================================================================
	pTestObject = pWolfTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->GetScript<CMonsterScript>()->SetUpDown(false);

	pTestObject->SetName(L"Wolf");
	pTestObject->Transform()->SetLocalPos(Vec3(50.f, -600.f, 1000.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================
	pTestObject = pBoarTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(60.f, 60.f, 60.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pTestObject->GetScript<CMonsterScript>()->SetUpDown(false);

	pTestObject->SetName(L"Boar");
	pTestObject->Transform()->SetLocalPos(Vec3(50.f, 300.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================






	// ====================================================================
	pTestObject = pBearTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 80.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.5f, 2.5f, 2.5f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CUBE);

	pTestObject->SetName(L"Bear");
	pTestObject->Transform()->SetLocalPos(Vec3(20.f, 300.f, 3000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ===================================================================
	pTestObject = pWolfTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CUBE);

	pTestObject->GetScript<CMonsterScript>()->SetUpDown(false);

	pTestObject->SetName(L"Wolf");
	pTestObject->Transform()->SetLocalPos(Vec3(50.f, -600.f, 3000.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================
	pTestObject = pBoarTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(60.f, 60.f, 60.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CUBE);

	pTestObject->GetScript<CMonsterScript>()->SetUpDown(false);

	pTestObject->SetName(L"Boar");
	pTestObject->Transform()->SetLocalPos(Vec3(50.f, 300.f, 3000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================
	// ===================================================================
	pTestObject = pWolfTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 50.f, 0.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CUBE);

	pTestObject->SetName(L"Wolf");
	pTestObject->Transform()->SetLocalPos(Vec3(-400.f, 300.f, 3000.f));
	pTestObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 30.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================
	pTestObject = pBoarTex->Instantiate();
	pTestObject->AddComponent(new CCollider2D);
	pTestObject->AddComponent(new CMonsterScript);
	pTestObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 50.f));
	pTestObject->Collider2D()->SetOffsetScale(Vec3(60.f, 60.f, 60.f));
	pTestObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CUBE);

	pTestObject->SetName(L"Boar");
	pTestObject->Transform()->SetLocalPos(Vec3(50.f, 300.f, 3000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================


	for (int i = 0; i < 70; ++i)
	{
		int type = rand() % 3;
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
		float fScale = (float)(rand() % 10 + 30);
		pTestObject->SetName(L"Tree");
		pTestObject->FrustumCheck(false);
		pTestObject->Transform()->SetLocalPos(Vec3((float)(rand()%10000 - 5000), 20.f, (float)(rand() % 8000)));
		pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, XM_PI, 0.f));
		pTestObject->Transform()->SetLocalScale(Vec3(fScale, fScale, fScale));
		m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	}

	Ptr<CMeshData> pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\genericcliffb.mdat", L"MeshData\\genericcliffb.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-5000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(12.f, 12.f, 12.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\genericcliffa.mdat", L"MeshData\\genericcliffa.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-4000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(12.f, 12.f, 12.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\lowrockmedium.mdat", L"MeshData\\lowrockmedium.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-3500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(12.f, 12.f, 12.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks01.mdat", L"MeshData\\MountainsRocks01.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-2500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks01_A.mdat", L"MeshData\\MountainsRocks01_A.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-1500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks01_B.mdat", L"MeshData\\MountainsRocks01_B.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-1000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks01_C.mdat", L"MeshData\\MountainsRocks01_C.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(-500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks01_D.mdat", L"MeshData\\MountainsRocks01_D.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(0.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks02.mdat", L"MeshData\\MountainsRocks02.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks02_A.mdat", L"MeshData\\MountainsRocks02_A.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(1000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks02_B.mdat", L"MeshData\\MountainsRocks02_B.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(1500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\MountainsRocks03.mdat", L"MeshData\\MountainsRocks03.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(2000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RockDetailsA.mdat", L"MeshData\\RockDetailsA.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(2500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RockDetailsB.mdat", L"MeshData\\RockDetailsB.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(3000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RockMediumA.mdat", L"MeshData\\RockMediumA.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(3500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RockMediumB.mdat", L"MeshData\\RockMediumB.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(4000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\RockMediumC.mdat", L"MeshData\\RockMediumC.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(4500.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	pRockTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Stone_01_Base.mdat", L"MeshData\\Stone_01_Base.mdat");
	// ====================================================================
	pTestObject = pRockTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(5000.f, 100.f, 1000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================


	// ====================================================================
	pTestObject = pDeerTex->Instantiate();
	pTestObject->SetName(L"Rock");
	pTestObject->FrustumCheck(false);
	pTestObject->Transform()->SetLocalPos(Vec3(0.f, 100.f, 2000.f));
	pTestObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//pTestObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	pTestObject->Transform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pTestObject);
	// ====================================================================

	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>( L"TestTex", L"Texture\\Health.png" );
	Ptr<CTexture> pExplosionTex = CResMgr::GetInst()->Load<CTexture>( L"Explosion", L"Texture\\Explosion\\Explosion80.png" );
	Ptr<CTexture> pBlackTex = CResMgr::GetInst()->Load<CTexture>( L"Black", L"Texture\\asd.png" );
	Ptr<CTexture> pSky01 = CResMgr::GetInst()->Load<CTexture>( L"Sky01", L"Texture\\Skybox\\Sky01.png" );
	Ptr<CTexture> pSky02 = CResMgr::GetInst()->Load<CTexture>( L"Sky02", L"Texture\\Skybox\\Sky02.jpg" );

	Ptr<CTexture> pColor = CResMgr::GetInst()->Load<CTexture>( L"Tile", L"Texture\\Tile\\TILE_03.tga" );
	Ptr<CTexture> pNormal = CResMgr::GetInst()->Load<CTexture>( L"Tile_n", L"Texture\\Tile\\TILE_03_N.tga" );

	Ptr<CTexture> pDiffuseTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"DiffuseTargetTex" );
	Ptr<CTexture> pNormalTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"NormalTargetTex" );
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>( L"PositionTargetTex" );
	// Compute Shader Test
	// UAV 용 Texture 생성
	Ptr<CTexture> pTestUAVTexture = CResMgr::GetInst()->CreateTexture(L"UAVTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
		, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	Ptr<CMaterial> pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_3, pSky01.GetPointer());

	pPM = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");
	pPM->SetData(SHADER_PARAM::TEX_2, pSky01.GetPointer());

	CGameObject* pObject = nullptr;
	CGameObject* pChildObject = nullptr;


	// ===================
	// Player 오브젝트 생성
	// ===================
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\monster.mdat", L"MeshData\\monster.mdat");

	CGameObject* pPlayer = pMeshData->Instantiate();
	// Script 설정
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->Collider2D()->SetOffsetScale(Vec3(20.f, 40.f, 20.f));
	pPlayer->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	pPlayer->SetName(L"Player Object");
	pPlayer->FrustumCheck(false);
	pPlayer->Transform()->SetLocalPos(Vec3(0.f, 600.f, 0.f));
	pPlayer->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	pPlayer->Transform()->SetLocalRot(Vec3(0.f, 180.f, 0.f));
	m_pScene->FindLayer(L"Player")->AddGameObject(pPlayer);
	m_pPlayer = pPlayer;

	// ====================
	// Monster 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName( L"Monster Object" );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );

	// Transform 설정
	pObject->Transform()->SetLocalPos( Vec3( 0.f, 200.f, 500.f ) );
	pObject->Transform()->SetLocalScale( Vec3( 100.f, 100.f, 1.f ) );

	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"TestMtrl" ) );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, pNormalTargetTex.GetPointer() );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_1, pNormal.GetPointer() );

	// Script 설정
	// pObject->AddComponent(new CMonsterScript);

	// AddGameObject
	m_pScene->FindLayer( L"Monster" )->AddGameObject( pObject );

	// ==================
	// Camera Object 생성
	// ==================
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName( L"MainCam" );
	pMainCam->AddComponent( new CTransform );
	pMainCam->AddComponent( new CCamera );
	pMainCam->AddComponent( new CFPSCamScript );

	pMainCam->Transform()->SetLocalPos( Vec3( 0.f, 100.f, 0.f ) );
	//pMainCam->Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));

	pMainCam->Camera()->SetProjType( PROJ_TYPE::PERSPECTIVE );
	pMainCam->Camera()->SetFar( 100000.f );
	pMainCam->Camera()->SetLayerAllCheck();
	pMainCam->Camera()->SetLayerCheck( 30, false );
	pMainCam->Camera()->SetLayerCheck( 29, false );
	pMainCam->GetScript<CFPSCamScript>()->SetPlayer(pPlayer);
	m_pScene->FindLayer( L"Default" )->AddGameObject( pMainCam );

	// ====================
	// Player Camera
	// ====================
	CGameObject* pPlayerCam = new CGameObject;
	pPlayerCam->AddComponent(new CTransform);
	pPlayerCam->AddComponent(new CCamera);
	pPlayerCam->AddComponent(new CPlayerCamScript);
	pPlayerCam->GetScript<CPlayerCamScript>()->SetPlayer(pPlayer);
	
	//pPlayerCam->Transform()->SetLocalPos(Vec3(0.f, 25.f, 150.f));
	pPlayerCam->Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));

	pPlayerCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pPlayerCam->Camera()->SetFar(10000.f);
	pPlayerCam->Camera()->SetLayerCheck(1, true);
	pPlayerCam->Camera()->SetCamType(CAM_TYPE::INVENTORY);

	m_pScene->FindLayer(L"Default")->AddGameObject(pPlayerCam);

	// ====================
	// UI Camera
	// ====================
	CGameObject* pUICam = new CGameObject;
	pUICam->AddComponent( new CTransform );
	pUICam->AddComponent( new CCamera );

	pUICam->Camera()->SetProjType( PROJ_TYPE::ORTHGRAPHIC );
	pUICam->Camera()->SetFar( 10000.f );
	pUICam->Camera()->SetLayerCheck( 30, true );

	m_pScene->FindLayer(L"Default")->AddGameObject(pUICam);

	CreatePlayerStatusUI();
	CreateInventoryUI();
	CreateChatUI();
	CSceneMgr::GetInst()->CreateMRTUI();

	// ====================
	// 3D Light Object 추가
	// ====================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CLight3D );
	pObject->AddComponent( new CSunshineScript );

	pObject->Light3D()->SetLightPos(Vec3(0.f, 500.f, 0.f));
	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
	pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	pObject->Light3D()->SetLightRange(1000.f);

	m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );

	CGameObject* pSun = pObject;

	// ===================
	// Test 오브젝트 생성
	// ===================
	pObject = new CGameObject;
	pObject->SetName( L"Test Object" );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );

	// Transform 설정
	pObject->Transform()->SetLocalPos( Vec3( 0.f, 200.f, 1000.f ) );
	pObject->Transform()->SetLocalScale( Vec3( 100.f, 100.f, 100.f ) );
	//pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"SphereMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"Std3DMtrl" ) );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, pColor.GetPointer() );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_1, pNormal.GetPointer() );


	// AddGameObject
	m_pScene->FindLayer( L"Monster" )->AddGameObject( pObject );

	// Script 설정
	pObject->AddComponent( new CMonsterScript );

	// AddGameObject
	m_pScene->FindLayer( L"Monster" )->AddGameObject( pObject );

	// ====================
	// Skybox 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName( L"SkyBox" );
	pObject->FrustumCheck( false );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );

	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"SphereMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"SkyboxMtrl" ) );
	pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, pSky01.GetPointer() );

	// AddGameObject
	m_pScene->FindLayer( L"Default" )->AddGameObject( pObject );
	pSun->GetScript<CSunshineScript>()->SetSkybox(pObject);

	// =======================
	// LandScape 오브젝트 생성
	// =======================
	/*pObject = new CGameObject;
	pObject->SetName(L"LandScape Object");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CLandScape);
	pObject->LandScape()->CreateLandScape(L"Texture/TestLandScape.bmp", 219, 219);
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"LandScapeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pSky01.GetPointer());
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(15.f, 15.f, 15.f));
	pObject->FrustumCheck(false);

	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);*/


	// ====================
	// Grid 오브젝트 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName( L"Grid" );
	pObject->FrustumCheck( false );
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CMeshRender );
	pObject->AddComponent( new CGridScript );

	// Transform 설정
	pObject->Transform()->SetLocalScale( Vec3( 100000.f, 100000.f, 1.f ) );
	pObject->Transform()->SetLocalRot( Vec3( XM_PI / 2.f, 0.f, 0.f ) );

	// MeshRender 설정
	pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
	pObject->MeshRender()->SetMaterial( CResMgr::GetInst()->FindRes<CMaterial>( L"GridMtrl" ) );

	// Script 설정	
	pObject->GetScript<CGridScript>()->SetToolCamera( pMainCam );
	pObject->GetScript<CGridScript>()->SetGridColor( Vec3( 0.8f, 0.2f, 0.2f ) );

	// AddGameObject
	m_pScene->FindLayer( L"Tool" )->AddGameObject( pObject );

	// ===================
	// Test 오브젝트 생성
	// ===================
	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			pObject = new CGameObject;
			pObject->SetName(L"Test Object");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			// Transform 설정
			if(i > 9)
				pObject->Transform()->SetLocalPos(Vec3(-5000.f + (j * 1000.f), -60.f, 9000.f - (i * 1000.f) + 20.f));
			else
				pObject->Transform()->SetLocalPos(Vec3(-5000.f + (j * 1000.f), 10.f, 9000.f - (i * 1000.f)));
			pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1.f));
			if(i > 9)
				pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.2f, 0.f, 0.f));
			else
				pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

			// MeshRender 설정
			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pColor.GetPointer());
			pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, pNormal.GetPointer());

			// AddGameObject
			m_pScene->FindLayer(L"Monster")->AddGameObject(pObject);
		}
	}

	// ==========================
	// Distortion Object 만들기
	// ==========================
	pObject = new CGameObject;
	pObject->SetName(L"Water");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	// Material 값 셋팅
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestWaterMtrl");

	float tessellation = 12.f;

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &tessellation);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -50.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(10000.f, 10000.f, 1.f));
	pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

	m_pScene->FindLayer(L"Monster")->AddGameObject(pObject);

	// ====================
	// Particle Object 생성
	// ====================
	pObject = new CGameObject;
	pObject->SetName(L"Particle");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleSystem);

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(-300.f, 50.f, 300.f));

	m_pScene->FindLayer(L"Default")->AddGameObject(pObject);

	// =============
	// FBX 파일 로드
	// =============
	//Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\monster.fbx");
	//pMeshData->Save(pMeshData->GetPath());

	//// MeshData 로드
	/*Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\monster.mdat", L"MeshData\\monster.mdat");

	pObject = pMeshData->Instantiate();
	pObject->SetName(L"House");
	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(0.f, 600.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	m_pScene->FindLayer(L"Monster")->AddGameObject(pObject);*/

	// =================================
	// CollisionMgr 충돌 그룹(Layer) 지정
	// =================================
	// Player Layer 와 Monster Layer 는 충돌 검사 진행
	CCollisionMgr::GetInst()->CheckCollisionLayer( L"Player", L"Monster" );
	CCollisionMgr::GetInst()->CheckCollisionLayer( L"Monster", L"Monster" );
}

void CIngameScene::Update()
{
	if (KEY_TAB(KEY_TYPE::KEY_ENTER))
	{
		if (m_pChat)
		{
			if (m_pChat->GetScript<CInputScript>()->GetEnable()) 
			{
				string str = m_pChat->GetScript<CInputScript>()->GetString();
				wstring strPlayerName = m_pPlayer->GetName();
				m_pChat->GetScript<CChatScript>()->AddChat("Test", str);
				m_pChat->GetScript<CInputScript>()->SetEnable(false);
			}
			else
				m_pChat->GetScript<CInputScript>()->SetEnable(true);
		}
	}
}

void CIngameScene::CreateQuickSlotUI(CGameObject* _pInventory)
{
	// ===================
	// UI 오브젝트 생성
	// ===================
	// UI QuickSlot BackGround
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(0.f, -320.f, 1000.f));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 80.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	int a = 1;
	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	pObject->AddComponent(new CQuickSlotScript);
	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

	// UI QuickSlot slot
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pChildObject = new CGameObject;
		pChildObject->AddComponent(new CTransform);
		pChildObject->AddComponent(new CMeshRender);

		//pChildObject->Transform()->SetLocalPos(Vec3(-190.f + (i * 95.f), -330.f, 500.f));
		pChildObject->Transform()->SetLocalPos(Vec3(-0.4f + (i * 0.2f), -0.1f, -100.f));
		pChildObject->Transform()->SetLocalScale(Vec3(75.f / 500.f, 75.f / 80.f, 1.f));

		pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

		a = 2;
		pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

		pObject->AddChild(pChildObject);
		m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);
		_pInventory->GetScript<CInventoryScript>()->AddSlot(pChildObject);
	}


	Ptr<CTexture> pFont = CFontMgr::GetInst()->GetFontTex();
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pChildObject = new CGameObject;
		pChildObject->AddComponent(new CTransform);
		pChildObject->AddComponent(new CMeshRender);

		//pChildObject->Transform()->SetLocalPos(Vec3(-190.f + (i * 95.f), -330.f, 500.f));
		pChildObject->Transform()->SetLocalPos(Vec3(-0.4f + (i * 0.2f), -0.6f, -100.f));
		pChildObject->Transform()->SetLocalScale(Vec3(20.f / 500.f, 20.f / 80.f, 1.f));

		pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));

		pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pFont.GetPointer());

		// 추후에 폰트 정보를 파일입출력으로 받아온 후
		// 각 글자에 따라서 정보를 찍어주면 됨
		// 49
		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[(char)i + 49];
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		Vec4 fontColor = Vec4(1.f, 1.f, 1.f, 1.f);
		Vec4 fontBackColor = Vec4(0.4f, 0.4f, 0.8f, 1.f);

		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_3, &heightV);

		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &fontColor);
		pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_1, &fontBackColor);

		pObject->AddChild(pChildObject);
		m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);
	}
}

void CIngameScene::CreatePlayerStatusUI()
{
	Ptr<CTexture> pUITex = CResMgr::GetInst()->Load<CTexture>(L"UITex", L"Texture\\Image2.png");
	Ptr<CTexture> pTexHealth = CResMgr::GetInst()->Load<CTexture>(L"HealthIconTex", L"Texture\\Health01.png");
	Ptr<CTexture> pTexHungry = CResMgr::GetInst()->Load<CTexture>(L"HungryIconTex", L"Texture\\Hungry01.png");
	Ptr<CTexture> pTexThirst = CResMgr::GetInst()->Load<CTexture>(L"ThirstIconTex", L"Texture\\Thirst01.png");

	//
	// PlayerStatus BackGround
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->SetName(L"Player Status");
	pObject->Transform()->SetLocalPos(Vec3(490.f, -300.f, 1500.f));
	pObject->Transform()->SetLocalScale(Vec3(250.f, 135.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	int a = 4; // White
	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	pObject->AddComponent(new CStatusScript);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

	// PlayerStatus HealthBar
	CGameObject* pChildObject = new CGameObject;
	pChildObject->AddComponent(new CTransform);
	pChildObject->AddComponent(new CMeshRender);
	//pChildObject->Transform()->SetLocalPos(Vec3(510.f, -260.f, 1000.f));
	pChildObject->Transform()->SetLocalPos(Vec3(0.075f, 0.3f, -100.f));
	pChildObject->Transform()->SetLocalScale(Vec3(190.f / 250.f, 30.f / 135.f, 1.f));
	pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	a = 5; // Green
	pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	pObject->AddChild(pChildObject);
	m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);

	// PlayerStatus HungryBar
	pChildObject = new CGameObject;
	pChildObject->AddComponent(new CTransform);
	pChildObject->AddComponent(new CMeshRender);
	//pChildObject->Transform()->SetLocalPos(Vec3(510.f, -300.f, 1000.f));
	pChildObject->Transform()->SetLocalPos(Vec3(0.075f, 0.f, -100.f));
	pChildObject->Transform()->SetLocalScale(Vec3(190.f / 250.f, 30.f / 135.f, 1.f));
	pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	a = 6; // Brown
	pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	pObject->AddChild(pChildObject);
	m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);
	// PlayerStatus ThirstBar
	pChildObject = new CGameObject;
	pChildObject->AddComponent(new CTransform);
	pChildObject->AddComponent(new CMeshRender);
	pChildObject->Transform()->SetLocalPos(Vec3(0.075f, -0.3f, -100.f));
	pChildObject->Transform()->SetLocalScale(Vec3(190.f / 250.f, 30.f / 135.f, 1.f));
	pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	a = 3; // Blue
	pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	pObject->AddChild(pChildObject);
	m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);

	// =================================
	pChildObject = new CGameObject;
	pChildObject->SetName(L"Screen Damage");
	pChildObject->AddComponent(new CTransform);
	pChildObject->AddComponent(new CMeshRender);

	// Transform 설정
	pChildObject->Transform()->SetLocalPos(Vec3(-1.95f, 2.25f, -1400.f));
	pChildObject->Transform()->SetLocalScale(Vec3(5.2f, 5.8f, 1.f));

	// MeshRender 설정
	pChildObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChildObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	pChildObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pUITex.GetPointer());
	float health = 1.f;
	pChildObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &health);
	// AddGameObject
	pObject->AddChild(pChildObject);
	m_pScene->FindLayer(L"UI")->AddGameObject(pChildObject);

	//
	// Health Icon
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(390.f, -260.f, 500.f));
	pObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"IconMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pTexHealth.GetPointer());
	float fa = 1.f;

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fa);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	//
	// Hungry Icon
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(390.f, -300.f, 500.f));
	pObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"IconMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pTexHungry.GetPointer());

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fa);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	//
	// Thirst Icon
	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetLocalPos(Vec3(390.f, -340.f, 500.f));
	pObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"IconMtrl"));

	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pTexThirst.GetPointer());

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fa);

	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
}

void CIngameScene::CreateInventoryUI()
{
	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex");

	CGameObject* pInventory = new CGameObject;
	pInventory->SetName(L"Inventory Object");

	pInventory->AddComponent(new CTransform);
	pInventory->AddComponent(new CMeshRender);
	pInventory->AddComponent(new CInventoryScript);

	pInventory->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));

	pInventory->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	// MeshRender 설정
	pInventory->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	pInventory->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	int a = 4; // White
	pInventory->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	// AddGameObject
	m_pScene->FindLayer(L"UI")->AddGameObject(pInventory);

	CreateQuickSlotUI(pInventory);

	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Inventory Player Object");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3(-450.f, 0.f, 100.f));

		Vec3 vScale(250.f, 450.f, 1.f);
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, PlayerTex.GetPointer());

		// AddGameObject
		pInventory->AddChild(pObject);
		m_pScene->FindLayer(L"Invisible")->AddGameObject(pObject);
	}

	{
		for (int i = 0; i < 5; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				CGameObject* pItemSlot = new CGameObject;
				wstring strItem = L"ItemSlot_" + char(i) + char(j);

				pItemSlot->SetName(strItem);

				pItemSlot->AddComponent(new CTransform);
				pItemSlot->AddComponent(new CMeshRender);

				pItemSlot->Transform()->SetLocalPos(Vec3(-120.f + i * 90.f, 100.f - (j * 90.f), 1.f));

				Vec3 vScale(80.f, 80.f, 5.f);
				pItemSlot->Transform()->SetLocalScale(vScale);

				// MeshRender 설정
				pItemSlot->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
				Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl");
				pItemSlot->MeshRender()->SetMaterial(pMtrl->Clone());
				int color = 2; // White
				pItemSlot->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &color);

				// AddGameObject
				pInventory->AddChild(pItemSlot);
				m_pScene->FindLayer(L"Invisible")->AddGameObject(pItemSlot);
				pInventory->GetScript<CInventoryScript>()->AddSlot(pItemSlot);
			}
		}

		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Inventory ItemSlot BackGround");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));

		Vec3 vScale(600.f, 450.f, 10.f);
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		int color = 4; // White
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &color);

		// AddGameObject
		pInventory->AddChild(pObject);
		m_pScene->FindLayer(L"Invisible")->AddGameObject(pObject);
	}

	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName(L"Crafting Item");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vec3(470.f, 50.f, 1.f));

		Vec3 vScale(300.f, 550.f, 10.f);
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		int color = 4; // White
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &color);

		// AddGameObject
		pInventory->AddChild(pObject);
		m_pScene->FindLayer(L"Invisible")->AddGameObject(pObject);
	}
}

void CIngameScene::CreateChatUI()
{
	//m_pChat = new CGameObject;
	//m_pChat->SetName(L"Chat Object");
	//m_pChat->AddComponent(new CTransform);
	//m_pChat->AddComponent(new CInputScript);
	//m_pChat->AddComponent(new CChatScript);
	//m_pChat->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	//m_pChat->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Chat Box");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetLocalPos(Vec3(-480.f, -225.f, 100.f));
	//pObject->Transform()->SetLocalScale(Vec3(300.f, 300.f, 1.f));

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl");
	//pObject->MeshRender()->SetMaterial(pMtrl->Clone());
	//int color = 4; // White
	//pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &color);

	//m_pChat->AddChild(pObject);
	//m_pScene->FindLayer(L"UI")->AddGameObject(pObject);

	//// AddGameObject
	//m_pScene->FindLayer(L"UI")->AddGameObject(m_pChat);

	//pObject = new CGameObject;
	//pObject->SetName(L"Input Chat");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->Transform()->SetLocalPos(Vec3(-480.f, -360.f, 1.f));
	//pObject->Transform()->SetLocalScale(Vec3(300.f, 30.f, 1.f));

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));

	//int a = 1;
	//pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::INT_0, &a);

	//m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	//m_pChat->AddChild(pObject);
	//m_pChat->GetScript<CInputScript>()->SetFontSpace(pObject);


	//float fontSize = 300.f / 20.f;
	//for (int i = 0; i < 20; ++i)
	//{
	//	pObject = new CGameObject;
	//	pObject->SetName(L"Chat Input Font");
	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CMeshRender);

	//	pObject->Transform()->SetLocalPos(Vec3(-630.f + (i * fontSize) + fontSize / 2.f, -360.f, 1.f));
	//	pObject->Transform()->SetLocalScale(Vec3(fontSize, 30.f, 1.f));

	//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
	//	pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

	//	m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
	//	m_pChat->AddChild(pObject);
	//	m_pChat->GetScript<CInputScript>()->AddInputObject(pObject);
	//}
	//m_pChat->GetScript<CInputScript>()->SetEnable(false);


	//fontSize = 300.f / 30.f;
	/*for (int i = 0; i < 30; ++i)
	{
		for (int line = 0; line < MAX_CHAT_LINE; ++line)
		{
			pObject = new CGameObject;
			pObject->SetName(L"Chat Font");
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CMeshRender);

			pObject->Transform()->SetLocalPos(Vec3(-630.f + (i * fontSize) + fontSize / 2.f, -330.f + (line * 30.f), 1.f));
			pObject->Transform()->SetLocalScale(Vec3(fontSize, 30.f, 1.f));

			pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

			pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));
			pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CFontMgr::GetInst()->GetFontTex().GetPointer());

			m_pScene->FindLayer(L"UI")->AddGameObject(pObject);
			m_pChat->AddChild(pObject);
			m_pChat->GetScript<CChatScript>()->AddChatObject(pObject, line);
		}
	}*/
}
