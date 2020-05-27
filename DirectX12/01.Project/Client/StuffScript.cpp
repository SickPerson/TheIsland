#include "stdafx.h"
#include "StuffScript.h"


CStuffScript::CStuffScript(STUFF_TYPE eType)
	: m_eStuff(eType)
{
	if (eType == STUFF_TYPE::STUFF_STONE)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Stone_Icon", L"Texture\\Item\\Tex_stone_08.png"), 
			CResMgr::GetInst()->Load<CTexture>(L"Stone_Back_Icon", L"Texture\\Item\\Tex_stone_08_b.png")
		);
	}
	else if (eType == STUFF_TYPE::STUFF_WOOD)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Wood_Icon", L"Texture\\Item\\Tex_wood_03.png"), 
			CResMgr::GetInst()->Load<CTexture>(L"Wood_Back_Icon", L"Texture\\Item\\Tex_wood_03_b.png")
		);
	}
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
