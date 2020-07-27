#include "stdafx.h"
#include "HousingScript.h"

#include "BuildScript.h"
#include "PlayerScript.h"
#include "InventoryScript.h"

#include <iostream>

CHousingScript::CHousingScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT)eType)
{
	m_iMaxCount = HOUSING_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = HOUSING_MAX_COUNT;
	m_iCount = iCount;

	Ptr<CMeshData> pTex = CResMgr::GetInst()->Load<CMeshData>(L"housing_foundation.mdat", L"MeshData\\housing_foundation.mdat");
	m_pObj = pTex->Instantiate();
	m_pObj->AddComponent(new CBuildScript);

	m_pObj->AddComponent(new CCollider2D);
	m_pObj->Collider2D()->SetOffsetScale(Vec3(200.f, 200.f, 200.f));
	m_pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

	m_pObj->SetName(L"Foundation");
	m_pObj->Transform()->SetLocalPos(Vec3(0.f, 20.f, 0.f));
	m_pObj->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
	m_pObj->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
	m_pObj->GetScript<CBuildScript>()->Init();
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(m_pObj);
}


CHousingScript::~CHousingScript()
{
}

void CHousingScript::Update()
{
	CItemScript::Update();
}

void CHousingScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Housing Item Right Use" << std::endl;
}

void CHousingScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Housing Item Left Use" << std::endl;
	if (m_pObj->GetScript<CBuildScript>()->Build())
	{
		m_pObj->MeshRender()->SetDynamicShadow(true);

		tEvent evt = {};

		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = (DWORD_PTR)m_pObj;
		evt.lParam = ((DWORD_PTR)6 << 16 | (DWORD_PTR)true);

		CEventMgr::GetInst()->AddEvent(evt);


		// Regen
		Ptr<CMeshData> pTex = CResMgr::GetInst()->Load<CMeshData>(L"housing_foundation.mdat", L"MeshData\\housing_foundation.mdat");
		m_pObj = pTex->Instantiate();
		m_pObj->AddComponent(new CBuildScript);

		m_pObj->AddComponent(new CCollider2D);
		m_pObj->Collider2D()->SetOffsetScale(Vec3(200.f, 200.f, 200.f));
		m_pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = 20;

		m_pObj->SetName(L"Foundation");
		m_pObj->Transform()->SetLocalPos(vPos);
		m_pObj->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
		m_pObj->GetScript<CBuildScript>()->Init();
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Build")->AddGameObject(m_pObj);
	}
}

void CHousingScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{
	if (m_pObj)
	{
		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = 20;
		m_pObj->Transform()->SetLocalPos(vPos);
	}
}


void CHousingScript::EnableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj;
	evt.lParam = ((DWORD_PTR)5 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}

void CHousingScript::DisableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj;
	evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}