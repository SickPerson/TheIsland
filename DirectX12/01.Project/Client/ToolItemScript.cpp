#include "stdafx.h"
#include "ToolItemScript.h"
#include "StuffScript.h"
#include "AnimalScript.h"
#include "InventoryScript.h"
#include "PlayerScript.h"
#include "UsableScript.h"

#include <iostream>

CToolItemScript::CToolItemScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType)
{
	m_iMaxCount = TOOL_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = TOOL_MAX_COUNT;
	m_iCount = iCount;

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

	switch (m_eItemType)
	{
	case ITEM_PICKAXE:
	case ITEM_AXE:
	case ITEM_HAMMER:
	case ITEM_MACHETTE:
	case ITEM_WOODCLUB:
		if (pObj == NULL)
			break;
		if (pObj->GetScript<CAnimalScript>())
		{
			if (!pObj->GetScript<CAnimalScript>()->GetAnimalDead())
			{
				pObj->GetScript<CAnimalScript>()->Damage(GetObj(), m_fDamage);
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
		else if (pObj->GetName() == L"Tree")
		{
			int iAmount = 1;
			if (m_eItemType == ITEM_AXE)
				iAmount = 3;
			CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_WOOD);
			pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, iAmount);
		}
		break;
	case ITEM_BOW:
		break;
	case ITEM_CAMPFIRE:
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
