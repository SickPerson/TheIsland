#include "stdafx.h"
#include "ToolItemScript.h"

#include <iostream>

CToolItemScript::CToolItemScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType)
{
	m_iMaxCount = TOOL_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = TOOL_MAX_COUNT;
	m_iCount = iCount;
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
}
