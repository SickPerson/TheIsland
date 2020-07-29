#include "stdafx.h"
#include "ToolItemScript.h"
#include "StuffScript.h"
#include "AnimalScript.h"
#include "InventoryScript.h"
#include "PlayerScript.h"
#include "UsableScript.h"

#include "BuildScript.h"
#include "NaturalScript.h"

#include <Engine/ParticleSystem.h>
#include <Engine/Light3D.h>

#include <iostream>

CToolItemScript::CToolItemScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType),
	m_pObj(NULL)
{
	m_iMaxCount = TOOL_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = TOOL_MAX_COUNT;
	m_iCount = iCount;

	Ptr<CMeshData> pTex = NULL;

	switch (m_eItemType)
	{
	case ITEM_PICKAXE:
		m_fDamage = 30.f;
		break;
	case ITEM_AXE:
		m_fDamage = 30.f;
		break;
	case ITEM_HAMMER:
		m_fDamage = 15.f;
		break;
	case ITEM_MACHETTE:
		m_fDamage = 45.f;
		break;
	case ITEM_WOODCLUB:
		m_fDamage = 20.f;
		break;
	case ITEM_BOW:
		m_fDamage = 30.f;
		break;
	case ITEM_CAMPFIRE:
		pTex = CResMgr::GetInst()->Load<CMeshData>(L"Campfire.mdat", L"MeshData\\campfire.mdat");
		m_pObj = pTex->Instantiate();
		m_pObj->AddComponent(new CBuildScript(HOUSING_ETC));
		m_pObj->AddComponent(new CCollider2D);

		m_pObj->Collider2D()->SetOffsetScale(Vec3(150.f, 150.f, 150.f));
		m_pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::SPHERE);

		m_pObj->SetName(L"Campfire");
		m_pObj->Transform()->SetLocalPos(Vec3(0.f, 10.f, 0.f));
		m_pObj->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
		m_pObj->Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 0.3f));
		m_pObj->GetScript<CBuildScript>()->Init();
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(m_pObj);
		m_fDamage = 0.f;
		break;
	case ITEM_MAP:
		m_fDamage = 0.f;
		break;
	default:
		break;
	}
}


CToolItemScript::~CToolItemScript()
{
}

void CToolItemScript::Update()
{
	CItemScript::Update();
}

void CToolItemScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Tool Item Right Use" << std::endl;
}

void CToolItemScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Tool Item Left Use" << std::endl;
	CGameObject* pObject = nullptr;

	switch (m_eItemType)
	{
	case ITEM_PICKAXE:
	case ITEM_AXE:
	case ITEM_HAMMER:
	case ITEM_MACHETTE:
	case ITEM_WOODCLUB:
		if (pObj == NULL)
			break;
		// Animal Attack
		if (pObj->GetScript<CAnimalScript>())
		{
			if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
			{
				pObj->GetScript<CAnimalScript>()->Damage(pHost, m_fDamage);
			}
			else
			{
				int random = rand() % 3;
				int iAmount = 1;
				CItemScript* pItem = NULL;
				if (random == 0)
					pItem = new CStuffScript(ITEM_TYPE::ITEM_LEATHER);
				else if(random == 1)
					pItem = new CStuffScript(ITEM_TYPE::ITEM_BONE);
				else
					pItem = new CUsableScript(ITEM_TYPE::ITEM_COOKMEAT);

				if (m_eItemType == ITEM_MACHETTE)
					iAmount = 3;
				pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, 1);
			}
		}
		// Environment Attack (Tree, Stone, Bush)
		else if (pObj->GetScript<CNaturalScript>())
		{
			if (!pObj->GetScript<CNaturalScript>()->GetDestroy())
			{
				pObj->GetScript<CNaturalScript>()->Damage(pHost, m_fDamage);

				NATURAL_TYPE eType = pObj->GetScript<CNaturalScript>()->GetNaturalType();
				if (eType == NATURAL_TREE)
				{
					int iAmount = 1;
					if (m_eItemType == ITEM_AXE)
						iAmount = 3;
					CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_WOOD);
					pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, iAmount);
				}
				else if (eType == NATURAL_STONE)
				{
					int iAmount = 1;
					if (m_eItemType == ITEM_PICKAXE)
						iAmount = 3;
					CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_STONE);
					pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, iAmount);
				}
				else if (eType == NATURAL_BUSH)
				{
					int iAmount = 2;
					CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_CLOTH);
					pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, iAmount);
				}
			}
		}
		break;
	case ITEM_BOW:
		break;
	case ITEM_CAMPFIRE:
		if (m_pObj->GetScript<CBuildScript>()->Build())
		{
			m_pObj->MeshRender()->SetDynamicShadow(true);

			pObject = new CGameObject;
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);
			pObject->SetName(L"FireEffect");
			pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 60.f));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			m_pObj->AddChild(pObject);

			pObject = new CGameObject;
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CParticleSystem);
			pObject->SetName(L"FireEffect2");
			pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 40.f));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			pObject->ParticleSystem()->SetStartColor(Vec4(1.f, 0.7f, 0.f, 1.f));
			pObject->ParticleSystem()->SetEndColor(Vec4(0.5f, 0.1f, 0.f, 0.6f));
			m_pObj->AddChild(pObject);

			pObject = new CGameObject;
			pObject->AddComponent(new CTransform);
			pObject->AddComponent(new CLight3D);
			pObject->SetName(L"FireLight");

			pObject->Light3D()->SetLightPos(Vec3(0.f, 0.f, 40.f));
			pObject->Light3D()->SetLightType(LIGHT_TYPE::POINT);
			pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
			pObject->Light3D()->SetSpecular(Vec3(0.3f, 0.3f, 0.3f));
			pObject->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
			pObject->Light3D()->SetLightRange(1000.f);

			pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 40.f));
			pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			m_pObj->AddChild(pObject);

			tEvent evt = {};

			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = (DWORD_PTR)m_pObj;
			evt.lParam = ((DWORD_PTR)6 << 16 | (DWORD_PTR)true);

			CEventMgr::GetInst()->AddEvent(evt);

			if (!SetItemIncrease(-1))
			{
				pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->OnAddable(num);
			}
			m_pObj = NULL;
		}
		break;
	case ITEM_MAP:
		break;
	default:
		break;
	}
	/*if (pObj->GetName() == L"Tree")
	{
		CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_WOOD);
		pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, rand() % 3 + 1);
	}
	else if (pObj->GetName() == L"Bear")
	{
		if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
		{
			pObj->GetScript<CAnimalScript>()->Damage(GetObj(), 5);
		}
		else
		{
			CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_LEATHER);
			pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, rand() % 3 + 1);
		}
	}
	else if (pObj->GetName() == L"Boar")
	{
		if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
		{
			pObj->GetScript<CAnimalScript>()->Damage(GetObj(), 5);
		}
		else
		{
			CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_BONE);
			pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, rand() % 3 + 1);
		}
	}
	else if (pObj->GetName() == L"Wolf")
	{
		if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
		{
			pObj->GetScript<CAnimalScript>()->Damage(GetObj(), 5);
		}
		else
		{
			CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_LEATHER);
			pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, rand() % 3 + 1);
		}
	}
	else if (pObj->GetName() == L"Deer")
	{
		if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
		{
			pObj->GetScript<CAnimalScript>()->Damage(GetObj(), 5);
		}
		else
		{
			CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_BONE);
			pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, rand() % 3 + 1);
		}
	}*/
}

void CToolItemScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{
	switch (m_eItemType)
	{
	case ITEM_CAMPFIRE:
		if (m_pObj)
		{
			Vec3 vDir = pHost->Transform()->GetWorldDir(DIR_TYPE::FRONT);
			Vec3 vPos = pHost->Transform()->GetLocalPos();

			vPos += -vDir * 300.f;
			vPos.y = 10;
			m_pObj->Transform()->SetLocalPos(vPos);
		}
		break;
	default:
		break;
	}
}

void CToolItemScript::EnableItem(CGameObject* pHost, int num)
{
	switch (m_eItemType)
	{
	case ITEM_CAMPFIRE:
		if (m_pObj)
		{
			tEvent evt = {};

			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = (DWORD_PTR)m_pObj;
			evt.lParam = ((DWORD_PTR)5 << 16 | (DWORD_PTR)true);

			CEventMgr::GetInst()->AddEvent(evt);
		}
		break;
	default:
		break;
	}
}

void CToolItemScript::DisableItem(CGameObject* pHost, int num)
{
	switch (m_eItemType)
	{
	case ITEM_CAMPFIRE:
		if (m_pObj)
		{
			tEvent evt = {};

			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = (DWORD_PTR)m_pObj;
			evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);

			CEventMgr::GetInst()->AddEvent(evt);
		}
		break;
	default:
		break;
	}
}