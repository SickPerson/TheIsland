#pragma once
#include <Engine/Ptr.h>
#include <Engine/Texture.h>

enum ITEM_TYPE {
	ITEM_NOTHING = 0,

	ITEM_TOOL,
	ITEM_PICKAXE,
	ITEM_AXE,
	ITEM_HAMMER,
	ITEM_TOOL_END,

	ITEM_STUFF,
	ITEM_WOOD,
	ITEM_WOODSTICK,
	ITEM_STONE,
	ITEM_LEATHER,
	ITEM_BONE,
	ITEM_STUFF_END,

	ITEM_FOOD,
	ITEM_MEAT,
	ITEM_COOKMEAT,
	ITEM_FOOD_END,

	ITEM_USABLE,
	ITEM_USABLE_END,

	ITEM_END
};

typedef struct tItemRecipe {
	ITEM_TYPE eItem;
	UINT iCount;
}tItemRecipe;

Ptr<CTexture> GetItemIconByType(ITEM_TYPE eType);

vector<tItemRecipe> GetItemRecipe(ITEM_TYPE eType);

string GetItemNameByType(ITEM_TYPE eType);
