#include "stdafx.h"
#include "CheatMgr.h"

#include "ItemScript.h"
#include "PlayerScript.h"
#include "InventoryScript.h"
#include "StatusScript.h"
#include "StuffScript.h"

#include "AnimalSpawner.h"

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

void CCheatMgr::SetSpawner(CGameObject * pObject1, CGameObject * pObject2, CGameObject * pObject3)
{
	m_pSpawner[0] = pObject1;
	m_pSpawner[1] = pObject2;
	m_pSpawner[2] = pObject3;
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
	else if (strText == "SummonAnimal")
	{
		for (int i = 0; i < 3; ++i)
		{
			m_pSpawner[i]->GetScript<CAnimalSpawner>()->Respawn();
		}
		bResult = true;
	}
	else if (strText == "SummonDeer")
	{
		m_pSpawner[0]->GetScript<CAnimalSpawner>()->Respawn();
		bResult = true;
	}
	else if (strText == "SummonMaxAnimal")
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int count = 0; count < 50; ++count)
			{
				m_pSpawner[i]->GetScript<CAnimalSpawner>()->Respawn();
			}
		}
		bResult = true;
	}
	else if (strText == "Invincible")
	{
		m_bInvincible = !m_bInvincible;
		m_pPlayer->GetScript<CPlayerScript>()->GetStatusObject()->GetScript<CStatusScript>()->Invincible(m_bInvincible);
		bResult = true;
	}
	
	else if ( strText == "RainDropTrue" )
	{
		m_pClock->GetScript<CSunshineScript>()->SetRainDrop( true );
		bResult = true;
	}

	else if ( strText == "RainDropFalse" )
	{
		m_pClock->GetScript<CSunshineScript>()->SetRainDrop( false );
		bResult = true;
	}

	return bResult;
}
