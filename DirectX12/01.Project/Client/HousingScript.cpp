#include "stdafx.h"
#include "HousingScript.h"

#include "PlayerScript.h"
#include "InventoryScript.h"

#include <iostream>

CHousingScript::CHousingScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT)eType),
	m_eType(HOUSING_TYPE::HOUSING_FOUNDATION)
{
	m_iMaxCount = HOUSING_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = HOUSING_MAX_COUNT;
	m_iCount = iCount;

	for (int i = HOUSING_FOUNDATION; i < HOUSING_END; ++i)
	{		
		if(i == HOUSING_FOUNDATION)
			m_pTex[i] = CResMgr::GetInst()->Load<CMeshData>(L"housing_foundation.mdat", L"MeshData\\housing_foundation.mdat");
		else if (i == HOUSING_WALL)
			m_pTex[i] = CResMgr::GetInst()->Load<CMeshData>(L"housing_wall.mdat", L"MeshData\\housing_wall.mdat");
		else if (i == HOUSING_DOOR)
			m_pTex[i] = CResMgr::GetInst()->Load<CMeshData>(L"housing_door.mdat", L"MeshData\\housing_door.mdat");
		else if (i == HOUSING_WINDOW)
			m_pTex[i] = CResMgr::GetInst()->Load<CMeshData>(L"housing_window.mdat", L"MeshData\\housing_window.mdat");
		else if (i == HOUSING_FLOOR)
			m_pTex[i] = CResMgr::GetInst()->Load<CMeshData>(L"housing_floor.mdat", L"MeshData\\housing_floor.mdat");

		m_pObj[i] = m_pTex[i]->Instantiate();
		m_pObj[i]->AddComponent(new CBuildScript((HOUSING_TYPE)i));

		m_pObj[i]->AddComponent(new CCollider2D);
		m_pObj[i]->Collider2D()->SetOffsetScale(Vec3(195.f, 195.f, 195.f));

		if (i >= HOUSING_WALL && i < HOUSING_FLOOR)
			m_pObj[i]->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 120.f));

		m_pObj[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

		m_pObj[i]->SetName(L"Foundation");
		m_pObj[i]->Transform()->SetLocalPos(Vec3(0.f, 20.f, 0.f));
		m_pObj[i]->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj[i]->Transform()->SetLocalScale(Vec3(0.6f, 0.6f, 0.6f));
		m_pObj[i]->GetScript<CBuildScript>()->Init();
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(m_pObj[i]);
	}
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

	{
		tEvent evt = {};
		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = (DWORD_PTR)m_pObj[m_eType];
		evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);
		CEventMgr::GetInst()->AddEvent(evt);
	}

	int iType = m_eType;
	iType++;
	if (iType >= HOUSING_END)
		m_eType = HOUSING_FOUNDATION;
	else
		m_eType = (HOUSING_TYPE)iType;

	{
		tEvent evt = {};
		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = (DWORD_PTR)m_pObj[m_eType];
		evt.lParam = ((DWORD_PTR)5 << 16 | (DWORD_PTR)true);
		CEventMgr::GetInst()->AddEvent(evt);
	}
}

void CHousingScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Housing Item Left Use" << std::endl;
	if (m_pObj[m_eType]->GetScript<CBuildScript>()->Build())
	{
		m_pObj[m_eType]->MeshRender()->SetDynamicShadow(true);

		tEvent evt = {};

		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = (DWORD_PTR)m_pObj[m_eType];
		evt.lParam = ((DWORD_PTR)6 << 16 | (DWORD_PTR)true);

		CEventMgr::GetInst()->AddEvent(evt);


		// Regen
		m_pObj[m_eType] = m_pTex[m_eType]->Instantiate();
		m_pObj[m_eType]->AddComponent(new CBuildScript(m_eType));

		m_pObj[m_eType]->AddComponent(new CCollider2D);
		m_pObj[m_eType]->Collider2D()->SetOffsetScale(Vec3(195.f, 195.f, 195.f));
		if (m_eType >= HOUSING_WALL && m_eType < HOUSING_FLOOR)
			m_pObj[m_eType]->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 120.f));
		m_pObj[m_eType]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = 20;

		m_pObj[m_eType]->SetName(L"Foundation");
		m_pObj[m_eType]->Transform()->SetLocalPos(vPos);
		m_pObj[m_eType]->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj[m_eType]->Transform()->SetLocalScale(Vec3(0.6f, 0.6f, 0.6f));
		m_pObj[m_eType]->GetScript<CBuildScript>()->Init();
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Build")->AddGameObject(m_pObj[m_eType]);
	}
}

void CHousingScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{
	if (m_pObj[m_eType])
	{
		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = 20;
		m_pObj[m_eType]->Transform()->SetLocalPos(vPos);
	}
}


void CHousingScript::EnableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj[m_eType];
	evt.lParam = ((DWORD_PTR)5 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}

void CHousingScript::DisableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj[m_eType];
	evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}