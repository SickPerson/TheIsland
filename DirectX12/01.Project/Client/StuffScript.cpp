#include "stdafx.h"
#include "StuffScript.h"

#include <iostream>

CStuffScript::CStuffScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType)
{
	m_iMaxCount = STUFF_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = STUFF_MAX_COUNT;
	m_iCount = iCount;
}


CStuffScript::~CStuffScript()
{
}

void CStuffScript::Update()
{
	CItemScript::Update();
}

void CStuffScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Stuff Item Right Use" << std::endl;
}

void CStuffScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Stuff Item Left Use" << std::endl;
}