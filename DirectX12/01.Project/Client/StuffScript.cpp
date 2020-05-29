#include "stdafx.h"
#include "StuffScript.h"


CStuffScript::CStuffScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT) eType)
{
	if (eType == ITEM_TYPE::ITEM_STONE)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Stone_Icon", L"Texture\\Item\\Tex_stone_08.png"), 
			CResMgr::GetInst()->Load<CTexture>(L"Stone_Back_Icon", L"Texture\\Item\\Tex_stone_08_b.png")
		);
		SetName(L"Stone");
	}
	else if (eType == ITEM_TYPE::ITEM_WOOD)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Wood_Icon", L"Texture\\Item\\Tex_wood_03.png"), 
			CResMgr::GetInst()->Load<CTexture>(L"Wood_Back_Icon", L"Texture\\Item\\Tex_wood_03_b.png")
		);
		SetName(L"Wood");
	}
	else if (eType == ITEM_TYPE::ITEM_WOODSTICK)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"WoodStick_Icon", L"Texture\\Item\\Tex_wood_01.png"),
			CResMgr::GetInst()->Load<CTexture>(L"WoodStick_Back_Icon", L"Texture\\Item\\Tex_wood_01_b.png")
		);
		SetName(L"WoodStick");
	}
	else if (eType == ITEM_TYPE::ITEM_LEATHER)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Leather_Icon", L"Texture\\Item\\Tex_leather_07.png"),
			CResMgr::GetInst()->Load<CTexture>(L"Leather_Back_Icon", L"Texture\\Item\\Tex_leather_07_b.png")
		);
		SetName(L"Leather");
	}
	else if (eType == ITEM_TYPE::ITEM_BONE)
	{
		SetItemTex(
			CResMgr::GetInst()->Load<CTexture>(L"Bone_Icon", L"Texture\\Item\\Tex_bone_07.png"),
			CResMgr::GetInst()->Load<CTexture>(L"Bone_Back_Icon", L"Texture\\Item\\Tex_bone_07_b.png")
		);
		SetName(L"Bone");
	}
	else
	{
		assert(NULL);
	}

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
