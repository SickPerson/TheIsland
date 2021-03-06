#pragma once

// [ Worker Thread Event ]
enum EVENT_TYPE {
	EV_RECV,
	EV_SEND,
	EV_MONSTER_UPDATE,
	EV_NATURAL_UPDATE,
	EV_DB,
	EV_ETC,
	EV_END
};
// ------------------------
// [ Update Type ]
// - Animal
enum ANIMAL_UPDATE_TYPE{
	AUT_ATTACK,
	AUT_FOLLOW,
	AUT_EVASION,
	AUT_IDLE,
	AUT_DIE,
	AUT_REMOVE,
	AUT_RESPAWN,
	AUT_DAMAGE,
	AUT_END
};
// - Natural
enum NATURAL_UPDATE_TYPE
{
	NUT_DIE,
	NUT_RESPAWN,
	NUT_END
};
// - Etc
enum ETC_UPDATE_TYPE
{
	EUT_PLAYER_COLLISION,
	EUT_ANIMAL_COLLISION,
	EUT_USERINFO_SAVE,
	EUT_ROT,
	EUT_WEATHER,
	EUT_TIMER,
	EUT_PLAYER_UPDATE,
	EUT_END
};
// - DB
enum DB_UPDATE_TYPE
{
	DUT_LOGIN,
	DUT_LOGOUT,
	DUT_UPDATE_INFO,
	DUT_USER_SAVE,
	DUT_GET_ITEM,
	DUT_REMOVE_ITEM,
	DUT_END
};
// ------------------------
enum DATABASE_SAVE_TYPE {
	DBS_LOGIN_SUCCESS,
	DBS_LOGIN_FAIL,
	DBS_LOGOUT_SUCCESS,
	DBS_LOGOUT_FAIL,
	DBS_END
};

// [ Object Type ]
enum OBJ_TYPE{
	OT_PLAYER,
	OT_ANIMAL,
	OT_NATURAL,
	OT_HOUSE,
	OT_END
};
// - Animal
enum ANIMAL_TYPE{
	A_BEAR,
	A_BOAR,
	A_WOLF,
	A_DEER,
	A_END
};
// - Natural
enum NATURAL_TYPE{
	N_TREE,
	N_STONE,
	N_BUSH,
	N_NONE,
	N_END
};
// - House
enum HOUSING_TYPE {
	HOUSING_FOUNDATION,
	HOUSING_WALL,
	HOUSING_DOOR,
	HOUSING_WINDOW,
	HOUSING_FLOOR,
	HOUSING_ETC,
	HOUSING_END
};

// - Item
enum ITEM_TYPE {
	ITEM_NOTHING = 0,

	ITEM_TOOL,					//======= 도구 =======
	ITEM_PICKAXE,
	ITEM_AXE,
	ITEM_HAMMER,
	ITEM_MACHETTE,
	ITEM_WOODCLUB,
	ITEM_BOW,
	ITEM_CAMPFIRE,
	ITEM_MAP,
	ITEM_TOOL_END,				//======= =======

	ITEM_STUFF = ITEM_TOOL_END, //======= 재료 =======
	ITEM_WOOD,
	ITEM_WOODSTICK,
	ITEM_STONE,
	ITEM_LEATHER,
	ITEM_CLOTH,
	ITEM_BONE,
	ITEM_ARROW,
	ITEM_STUFF_END,				//======= =======

	ITEM_USABLE = ITEM_STUFF_END,	//======= 소모품 =======
	ITEM_FOOD = ITEM_USABLE,		//===== 식량 =====
	ITEM_MEAT,
	ITEM_COOKMEAT,
	ITEM_CHOCOLATEBAR,
	ITEM_CAN,
	ITEM_APPLE,
	ITEM_FOOD_END,					//===== =====
	ITEM_DRINK = ITEM_FOOD_END,		//===== 마실것 =====
	ITEM_EMPTY_BOTTLE,
	ITEM_WATER_BOTTLE,
	ITEM_SODA,
	ITEM_DRINK_END,					//===== =====
	ITEM_HEAL = ITEM_DRINK_END,		//===== 회복 =====
	ITEM_MEDICKIT,
	ITEM_BANDAGE,
	ITEM_HEAL_END,					//===== =====
	ITEM_USABLE_END,				//======= =======

	ITEM_ARMOR = ITEM_USABLE_END,	//======= 방어구(옷) =======
	ITEM_TSHIRT,
	ITEM_SHIRT,
	ITEM_JACKET,
	ITEM_ARMOR_END,					//======= =======

	ITEM_HOUSING = ITEM_ARMOR_END,	//======= 하우징 ========
	ITEM_BLUEPRINT,
	ITEM_HOUSING_END,				//======= ========

	ITEM_END
};
// ------------------------
// [ State Type ]
enum OBJ_STATE_TYPE {
	OST_LIVE,
	OST_DIE,
	OST_REMOVE,
	OST_END
};

enum BEHAVIOR_TYPE {
	B_WARLIKE, 
	B_PASSIVE, 
	B_EVASION, 
	B_END
};
enum ANIMAL_STATE_TYPE {

};
// ------------------------
enum DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};