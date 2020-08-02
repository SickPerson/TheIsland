#include "stdafx.h"
#include "ItemInfoScript.h"
#include "Item.h"

#include <Engine/Font.h>

CItemInfoScript::CItemInfoScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_pBackground(NULL),
	m_pItemName(NULL),
	m_pItemInfo(NULL)
{
}


CItemInfoScript::~CItemInfoScript()
{
}

void CItemInfoScript::Update()
{
}

void CItemInfoScript::SetObject(CGameObject * pBackground, CGameObject * pItemName, CGameObject * pItemInfo)
{
	m_pBackground = pBackground;
	m_pItemName = pItemName;
	m_strPrevItemName = pItemName->Font()->GetString();

	m_pItemInfo = pItemInfo;
}

void CItemInfoScript::SetItemName(string strName)
{
	if (m_strPrevItemName == strName)
		return;

	m_pItemName->Font()->SetString(strName);
	m_strPrevItemName = strName;
}

void CItemInfoScript::SetItemInfo(string strInfo)
{
	m_pItemInfo->Font()->SetString(strInfo);
}

void CItemInfoScript::SetItem(UINT eType)
{
	string strName = GetItemNameByType((ITEM_TYPE)eType);

	if (m_strPrevItemName == strName)
		return;

	SetItemName(strName);

	string strInfo = GetItemInfoByType((ITEM_TYPE)eType);
	SetItemInfo(strInfo);
}
