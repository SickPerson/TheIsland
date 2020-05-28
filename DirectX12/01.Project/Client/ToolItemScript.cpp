#include "stdafx.h"
#include "ToolItemScript.h"


CToolItemScript::CToolItemScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType)
{
	if (eType == ITEM_TYPE::ITEM_PICKAXE)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Pickaxe_Icon", L"Texture\\Item\\Tex_tools_07.png"),
			CResMgr::GetInst()->Load<CTexture>(L"Pickaxe_Back_Icon", L"Texture\\Item\\Tex_tools_07_b.png")
		);
	}
	else if (eType == ITEM_TYPE::ITEM_AXE)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Axe_Icon", L"Texture\\Item\\Tex_tools_09.png"),
			CResMgr::GetInst()->Load<CTexture>(L"Axe_Back_Icon", L"Texture\\Item\\Tex_tools_09_b.png")
		);
	}
	else if (eType == ITEM_TYPE::ITEM_HAMMER)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Hammer_Icon", L"Texture\\Item\\Tex_tools_10.png"),
			CResMgr::GetInst()->Load<CTexture>(L"Hammer_Back_Icon", L"Texture\\Item\\Tex_tools_10_b.png")
		);
	}
	else
	{
		assert(NULL);
	}

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
