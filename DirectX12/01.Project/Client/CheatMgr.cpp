#include "stdafx.h"
#include "CheatMgr.h"

#include "ItemScript.h"
#include "PlayerScript.h"
#include "InventoryScript.h"
#include "StuffScript.h"

#include "SunshineScript.h"

CCheatMgr::CCheatMgr()
{
}


CCheatMgr::~CCheatMgr()
{
}

void CCheatMgr::Init()
{
}

void CCheatMgr::SetPlayer(CGameObject * pObject)
{
	m_pPlayer = pObject;
}

void CCheatMgr::SetClock(CGameObject * pObject)
{
	m_pClock = pObject;
}

bool CCheatMgr::CheckCheat(string strText)
{
	bool bResult = false;
	if (strText == "Wood")
	{
		CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_WOOD);
		m_pPlayer->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, 50);
		bResult = true;
	}
	else if (strText == "Stone")
	{
		CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_STONE);
		m_pPlayer->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, 50);
		bResult = true;
	}
	else if (strText == "Leather")
	{
		CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_LEATHER);
		m_pPlayer->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, 50);
		bResult = true;
	}
	else if (strText == "Cloth")
	{
		CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_CLOTH);
		m_pPlayer->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->AddItem(pItem, 50);
		bResult = true;
	}
	else if (strText == "Night")
	{
		m_pClock->GetScript<CSunshineScript>()->SetTime(3, 30);
		bResult = true;
	}
	else if (strText == "Day")
	{
		m_pClock->GetScript<CSunshineScript>()->SetTime(12, 30);
		bResult = true;
	}
	else if (strText == "FastTime")
	{
		m_pClock->GetScript<CSunshineScript>()->SetDayCycle(2500.f);
		bResult = true;
	}
	else if (strText == "NormalTime")
	{
		m_pClock->GetScript<CSunshineScript>()->SetDayCycle(60.f);
		bResult = true;
	}
	return bResult;
}
