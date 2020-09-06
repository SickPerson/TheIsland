#include "stdafx.h"
#include "ArmorScript.h"

#include "PlayerScript.h"
#include "StatusScript.h"
#include "InventoryScript.h"

#include <iostream>
#include "PacketMgr.h"

CArmorScript::CArmorScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT)eType),
	m_pHost(NULL)
{

	switch (eType)
	{
	case ITEM_TSHIRT:
		m_fArmor = 30.f;
		break;
	case ITEM_SHIRT:
		m_fArmor = 60.f;
		break;
	case ITEM_JACKET:
		m_fArmor = 100.f;
		break;
	default:
		m_fArmor = 0.f;
		break;
	}
	m_iMaxCount = ARMOR_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = ARMOR_MAX_COUNT;
	m_iCount = iCount;
}


CArmorScript::~CArmorScript()
{
}

void CArmorScript::Update()
{
	CItemScript::Update();
}

UINT CArmorScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	return m_eItemType;
}

UINT CArmorScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	m_pHost = pHost;
	EquipArmor();

	return m_eItemType;
}

void CArmorScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{

}

void CArmorScript::EquipArmor()
{
#ifdef NETWORK_ON
	CPacketMgr::GetInst()->Send_Equip_Armor_Packet(m_eItemType);
#endif // NETWORK_ON
	m_pHost->GetScript<CPlayerScript>()->GetStatusObject()->GetScript<CStatusScript>()->EquipArmor(GetObj(), m_fArmor);
}

void CArmorScript::DestroyArmor()
{
#ifdef NETWORK_ON
	CPacketMgr::GetInst()->Send_Destroy_Armor_Packet();
#endif // NETWORK_ON
	m_pHost->GetScript<CPlayerScript>()->GetInventoryObject()->GetScript<CInventoryScript>()->DestroyArmor();
}

void CArmorScript::SetArmorValue(float fValue)
{
	m_fArmor = fValue;
}