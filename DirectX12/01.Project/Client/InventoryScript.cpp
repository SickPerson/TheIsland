#include "stdafx.h"
#include "InventoryScript.h"


CInventoryScript::CInventoryScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bActive(false)
{
	m_vItemSlot.reserve(25);
}


CInventoryScript::~CInventoryScript()
{
	m_vItemSlot.clear();
}

void CInventoryScript::Update()
{

}

void CInventoryScript::AddSlot(CGameObject * pObject)
{
	m_vItemSlot.emplace_back(pObject);
}

void CInventoryScript::Show()
{
	if (m_bActive)
	{
		TransferLayer(29, true);
		m_bActive = false;
	}
	else
	{
		TransferLayer(30, true);
		m_bActive = true;
	}
}
