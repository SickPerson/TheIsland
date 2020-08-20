#include "stdafx.h"
#include "HousingScript.h"

#include "PlayerScript.h"
#include "InventoryScript.h"

#include "HousingMgr.h"

#include <Engine/NaviMgr.h>

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
		m_pObj[i] = CHousingMgr::GetInst()->GetHousingMeshData((HOUSING_TYPE)i)->Instantiate();
		m_pObj[i]->AddComponent(new CBuildScript((HOUSING_TYPE)i));
		
#ifdef CHECK_COLLISTION
		m_pObj[i]->AddComponent( new CCollider2D );
		m_pObj[i]->Collider2D()->SetOffsetScale(Vec3(195.f, 195.f, 195.f));

		if (i >= HOUSING_WALL && i < HOUSING_FLOOR)
			m_pObj[i]->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 120.f));

		m_pObj[i]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);
#endif
		m_pObj[i]->SetName(L"House");
		m_pObj[i]->Transform()->SetLocalPos(Vec3(0.f, 20.f, 0.f));
		m_pObj[i]->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj[i]->Transform()->SetLocalScale(Vec3(0.6f, 0.6f, 0.6f));

		m_pObj[i]->MeshRender()->SetDynamicShadow(true);

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

UINT CHousingScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
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

	return m_eItemType;
}

UINT CHousingScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	int idx = pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->CheckItem(ITEM_TYPE::ITEM_WOOD, 3);
	if (idx == -1)
	{
		return 0;
	}

	if (m_pObj[m_eType]->GetScript<CBuildScript>()->Build())
	{
		m_pObj[m_eType]->MeshRender()->SetDynamicShadow(true);

		tEvent evt = {};

		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = (DWORD_PTR)m_pObj[m_eType];
		evt.lParam = ((DWORD_PTR)6 << 16 | (DWORD_PTR)true);

		CEventMgr::GetInst()->AddEvent(evt);


		// Regen
		m_pObj[m_eType] = CHousingMgr::GetInst()->GetHousingMeshData(m_eType)->Instantiate();
		m_pObj[m_eType]->AddComponent(new CBuildScript(m_eType));

#ifdef CHECK_COLLISTION
		m_pObj[m_eType]->AddComponent( new CCollider2D );
		m_pObj[m_eType]->Collider2D()->SetOffsetScale(Vec3(195.f, 195.f, 195.f));
		if (m_eType >= HOUSING_WALL && m_eType < HOUSING_FLOOR)
			m_pObj[m_eType]->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 120.f));
		m_pObj[m_eType]->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);
#endif

		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = CNaviMgr::GetInst()->GetY(vPos);
		vPos.y += 20;

		m_pObj[m_eType]->SetName(L"House");
		m_pObj[m_eType]->Transform()->SetLocalPos(vPos);
		m_pObj[m_eType]->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj[m_eType]->Transform()->SetLocalScale(Vec3(0.6f, 0.6f, 0.6f));

		m_pObj[m_eType]->GetScript<CBuildScript>()->Init();
		m_pObj[m_eType]->MeshRender()->SetDynamicShadow(true);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Build")->AddGameObject(m_pObj[m_eType]);

		pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->DecreaseItem(idx, 3);
	}

	return m_eItemType;
}

void CHousingScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{
	if (m_pObj[m_eType])
	{
		Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
		Vec3 vPos = pHost->Transform()->GetLocalPos();

		vPos += -vDir * 400.f;
		vPos.y = CNaviMgr::GetInst()->GetY(vPos);
		vPos.y += 20;
		m_pObj[m_eType]->Transform()->SetLocalPos(vPos);
	}
}


UINT CHousingScript::EnableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj[m_eType];
	evt.lParam = ((DWORD_PTR)5 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);

	return m_eItemType;
}

void CHousingScript::DisableItem(CGameObject* pHost, int num)
{
	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)m_pObj[m_eType];
	evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}