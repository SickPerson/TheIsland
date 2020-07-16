#include "stdafx.h"
#include "ToolItemScript.h"


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

void CToolItemScript::Use()
{
}
