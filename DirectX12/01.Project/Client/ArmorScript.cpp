#include "stdafx.h"
#include "ArmorScript.h"

#include <iostream>

CArmorScript::CArmorScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT)eType)
{
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

void CArmorScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Armor Item Right Use" << std::endl;
}

void CArmorScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Armor Item Left Use" << std::endl;
}

void CArmorScript::Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num)
{

}