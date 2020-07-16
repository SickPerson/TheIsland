#include "stdafx.h"
#include "Item.h"

#include <Engine/ResMgr.h>

Ptr<CTexture> GetItemIconByType(ITEM_TYPE eType)
{
	Ptr<CTexture> pTex = NULL;
	switch (eType) {
	case ITEM_STONE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Stone_Icon", L"Texture\\Item\\T_Rock_01_256x256.tga");
		break;
	case ITEM_WOOD:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Wood_Icon", L"Texture\\Item\\T_Dry_Wood_256x256.tga");
		break;
	case ITEM_WOODSTICK:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"WoodStick_Icon", L"Texture\\Item\\T_Collection_Of_Sticks_01_256x256.tga");
		break;
	case ITEM_LEATHER:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Leather_Icon", L"Texture\\Item\\T_Cloth_256x256.tga");
		break;
	case ITEM_HAMMER:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Hammer_Icon", L"Texture\\Item\\T_Hammer_256x256.tga");
		break;
	case ITEM_PICKAXE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Pickaxe_Icon", L"Texture\\Item\\T_Crowbar_256x256.tga");
		break;
	case ITEM_AXE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Axe_Icon", L"Texture\\Item\\T_Axe_256x256.tga");
		break;
	case ITEM_BONE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Bone_Icon", L"Texture\\Item\\T_Animal_Skull_256x256.tga");
		break;
	default:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"None_Icon", L"Texture\\Item\\T_Powder_256x256.tga");
		break;
	}
	return pTex;
}

string GetItemNameByType(ITEM_TYPE eType)
{
	string strName;
	switch (eType) {
	case ITEM_STONE:
		strName = "Stone";
		break;
	case ITEM_WOOD:
		strName = "Wood";
		break;
	case ITEM_WOODSTICK:
		strName = "Woodstick";
		break;
	case ITEM_LEATHER:
		strName = "Leather";
		break;
	case ITEM_HAMMER:
		strName = "Hammer";
		break;
	case ITEM_PICKAXE:
		strName = "Pickaxe";
		break;
	case ITEM_AXE:
		strName = "Axe";
		break;
	case ITEM_BONE:
		strName = "Bone";
		break;
	default:
		strName = "NoneItem";
		break;
	}
	return strName;
}

vector<tItemRecipe> GetItemRecipe(ITEM_TYPE eType)
{
	vector<tItemRecipe> vRecipe;
	tItemRecipe tInfo;

	switch (eType) {
	case ITEM_HAMMER:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_PICKAXE:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_AXE:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		break;
	default:
		vRecipe.clear();
		break;
	}
	return vRecipe;
}
