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
	case ITEM_CLOTH:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Cloth_Icon", L"Texture\\Item\\T_Paper_256x256.tga");
		break;
	case ITEM_WOODCLUB:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"WoodClub_Icon", L"Texture\\Item\\T_Three_Stick_256x256.tga");
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
	case ITEM_MACHETTE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Machette_Icon", L"Texture\\Item\\T_Machette_256x256.tga");
		break;
	case ITEM_BOW:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Bow_Icon", L"Texture\\Item\\T_Bow_256x256.tga");
		break;
	case ITEM_CAMPFIRE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Campfire_Icon", L"Texture\\Item\\T_Bonfire_256x256.tga");
		break;
	case ITEM_MAP:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Map_Icon", L"Texture\\Item\\T_Map_256x256.tga");
		break;
	case ITEM_ARROW:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Arrow_Icon", L"Texture\\Item\\T_Arrows_256x256.tga");
		break;
	case ITEM_MEAT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"RawMeat_Icon", L"Texture\\Item\\T_Cooked_Meat_02_256x256.tga");
		break;
	case ITEM_COOKMEAT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"CookedMeat_Icon", L"Texture\\Item\\T_Cooked_Meat_02_256x256.tga");
		break;
	case ITEM_CHOCOLATEBAR:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"ChocolateBar_Icon", L"Texture\\Item\\T_Chocolate_bar_256x256.tga");
		break;
	case ITEM_APPLE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Apple_Icon", L"Texture\\Item\\T_Apple_256x256.tga");
		break;
	case ITEM_CAN:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Can_Icon", L"Texture\\Item\\T_Can_of_Beans_Small_256x256.tga");
		break;
	case ITEM_EMPTY_BOTTLE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"EmptyBottle_Icon", L"Texture\\Item\\T_Bottle_256x256.tga");
		break;
	case ITEM_WATER_BOTTLE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"WaterBottle_Icon", L"Texture\\Item\\T_Bottle_256x256.tga");
		break;
	case ITEM_SODA:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Soda_Icon", L"Texture\\Item\\T_Soda_256x256.tga");
		break;
	case ITEM_MEDICKIT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"MedicKit_Icon", L"Texture\\Item\\T_Medic_Box_256x256.tga");
		break;
	case ITEM_BANDAGE:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Bandage_Icon", L"Texture\\Item\\T_Bandage_256x256.tga");
		break;
	case ITEM_TSHIRT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"TShirt_Icon", L"Texture\\Item\\T_NewTshirt_256x256.tga");
		break;
	case ITEM_SHIRT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Shirt_Icon", L"Texture\\Item\\T_NewShirt_256x256.tga");
		break;
	case ITEM_JACKET:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Jacket_Icon", L"Texture\\Item\\T_NewJacket_256x256.tga");
		break;
	case ITEM_BLUEPRINT:
		pTex = CResMgr::GetInst()->Load<CTexture>(L"Blueprint_Icon", L"Texture\\Item\\T_Tent_256x256.tga");
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
	case ITEM_CLOTH:
		strName = "Cloth";
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
	case ITEM_MACHETTE:
		strName = "Machette";
		break;
	case ITEM_WOODCLUB:
		strName = "WoodClub";
		break;
	case ITEM_BOW:
		strName = "Bow";
		break;
	case ITEM_CAMPFIRE:
		strName = "Campfire";
		break;
	case ITEM_MAP:
		strName = "Map";
		break;
	case ITEM_ARROW:
		strName = "Arrow";
		break;
	case ITEM_MEAT:
		strName = "Raw Meat";
		break;
	case ITEM_COOKMEAT:
		strName = "Cooked Meat";
		break;
	case ITEM_CHOCOLATEBAR:
		strName = "Chocolate Bar";
		break;
	case ITEM_APPLE:
		strName = "Apple";
		break;
	case ITEM_CAN:
		strName = "Can of beans";
		break;
	case ITEM_EMPTY_BOTTLE:
		strName = "Empty Bottle";
		break;
	case ITEM_WATER_BOTTLE:
		strName = "Water Bottle";
		break;
	case ITEM_SODA:
		strName = "Soda";
		break;
	case ITEM_MEDICKIT:
		strName = "Medic Kit";
		break;
	case ITEM_BANDAGE:
		strName = "Bandage";
		break;
	case ITEM_TSHIRT:
		strName = "T-Shirt";
		break;
	case ITEM_SHIRT:
		strName = "Shirt";
		break;
	case ITEM_JACKET:
		strName = "Jacket";
		break;
	case ITEM_BLUEPRINT:
		strName = "Blueprint";
		break;
	default:
		strName = "NoneItem";
		break;
	}
	return strName;
}

string GetItemInfoByType(ITEM_TYPE eType)
{
	string strInfo;
	switch (eType) {
	case ITEM_STONE:
		strInfo = "Used to make items";
		break;
	case ITEM_WOOD:
		strInfo = "This is the most basic item";
		break;
	case ITEM_WOODSTICK:
		strInfo = "Used to make items";
		break;
	case ITEM_LEATHER:
		strInfo = "It is animal skin";
		break;
	case ITEM_CLOTH:
		strInfo = "Used to make items";
		break;
	case ITEM_HAMMER:
		strInfo = "Destroy or upgrade buildings";
		break;
	case ITEM_PICKAXE:
		strInfo = "Collects stones effectively";
		break;
	case ITEM_AXE:
		strInfo = "Collects woods effectively";
		break;
	case ITEM_BONE:
		strInfo = "Used to make items";
		break;
	case ITEM_MACHETTE:
		strInfo = "Effectively hunt animals";
		break;
	case ITEM_WOODCLUB:
		strInfo = "It is a basic tool item";
		break;
	case ITEM_BOW:
		strInfo = "Ranged attacks are possible";
		break;
	case ITEM_CAMPFIRE:
		strInfo = "You can cook meat";
		break;
	case ITEM_MAP:
		strInfo = "Check the location of the island";
		break;
	case ITEM_ARROW:
		strInfo = "Ranged attacks are possible";
		break;
	case ITEM_MEAT:
		strInfo = "Eat only in emergencies";
		break;
	case ITEM_COOKMEAT:
		strInfo = "Rich in protein";
		break;
	case ITEM_CHOCOLATEBAR:
		strInfo = "This is the basic item";
		break;
	case ITEM_APPLE:
		strInfo = "Resolve some hunger";
		break;
	case ITEM_CAN:
		strInfo = "This is the basic item";
		break;
	case ITEM_EMPTY_BOTTLE:
		strInfo = "It is an empty water bottle";
		break;
	case ITEM_WATER_BOTTLE:
		strInfo = "Can solve thirst";
		break;
	case ITEM_SODA:
		strInfo = "This is the basic item";
		break;
	case ITEM_MEDICKIT:
		strInfo = "Recovers maximum health";
		break;
	case ITEM_BANDAGE:
		strInfo = "Recovers some health";
		break;
	case ITEM_TSHIRT:
		strInfo = "Provides extra HP";
		break;
	case ITEM_SHIRT:
		strInfo = "Provides extra HP";
		break;
	case ITEM_JACKET:
		strInfo = "Provides extra a lot HP";
		break;
	case ITEM_BLUEPRINT:
		strInfo = "You can build your own house";
		break;
	default:
		strInfo = "This item does not exist";
		break;
	}
	return strInfo;
}

vector<tItemRecipe> GetItemRecipe(ITEM_TYPE eType)
{
	vector<tItemRecipe> vRecipe;
	tItemRecipe tInfo;

	switch (eType) {
	case ITEM_HAMMER:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 8;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 8;
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
	case ITEM_MACHETTE:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 8;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 3;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_LEATHER;
		tInfo.iCount = 1;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_WOODCLUB:
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 1;
		vRecipe.push_back(tInfo);
	case ITEM_BOW:
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 5;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_LEATHER;
		tInfo.iCount = 3;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_CAMPFIRE:
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 3;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 3;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_MAP:
		tInfo.eItem = ITEM_TYPE::ITEM_LEATHER;
		tInfo.iCount = 4;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_ARROW:
		tInfo.eItem = ITEM_TYPE::ITEM_WOOD;
		tInfo.iCount = 1;
		vRecipe.push_back(tInfo);
		tInfo.eItem = ITEM_TYPE::ITEM_STONE;
		tInfo.iCount = 1;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_BANDAGE:
		tInfo.eItem = ITEM_TYPE::ITEM_CLOTH;
		tInfo.iCount = 2;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_TSHIRT:
		tInfo.eItem = ITEM_TYPE::ITEM_CLOTH;
		tInfo.iCount = 10;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_SHIRT:
		tInfo.eItem = ITEM_TYPE::ITEM_CLOTH;
		tInfo.iCount = 20;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_JACKET:
		tInfo.eItem = ITEM_TYPE::ITEM_LEATHER;
		tInfo.iCount = 10;
		vRecipe.push_back(tInfo);
		break;
	case ITEM_BLUEPRINT:
		tInfo.eItem = ITEM_TYPE::ITEM_CLOTH;
		tInfo.iCount = 10;
		vRecipe.push_back(tInfo);
		break;
	default:
		vRecipe.clear();
		break;
	}
	return vRecipe;
}
