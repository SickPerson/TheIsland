#include "stdafx.h"
#include "StuffScript.h"


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

void CStuffScript::Use()
{
	
}
