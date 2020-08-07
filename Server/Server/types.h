#pragma once

// Worker Thread Event
enum EVENT_TYPE {
	EV_RECV,
	EV_SEND,
	EV_MONSTER_UPDATE,
	EV_NATURAL_UPDATE,
	EV_DB,
	EV_END
};

enum DATABASE_SAVE_TYPE {
	DBS_LOGIN_SUCCESS,
	DBS_LOGIN_FAIL,
	DBS_LOGOUT_SUCCESS,
	DBS_LOGOUT_FAIL,
	DBS_END
};

// DB Process Event
enum DB_PROCESS_TYPE {
	DB_LOGIN,
	DB_GET_ITEM,
	DB_REMOVE_ITEM,
	DB_LOGOUT,
	DB_END
};

// [About Animal]
enum ANIMAL_TYPE
{
	A_BEAR,
	A_BOAR,
	A_DEER,
	A_WOLF,
	A_END
};

enum BEHAVIOR_TYPE
{
	B_WARLIKE, // 선공
	B_PASSIVE, // 비선공
	B_EVASION, // 회피
	B_END
};

// Natural
enum NATURAL_TYPE {
	NATURAL_TREE,
	NATURAL_STONE,
	NATURAL_BUSH,
	NATURAL_NONE,
	NATURAL_END
};

// Housing
enum HOUSING_TYPE {

	HOUSING_FOUNDATION,
	HOUSING_WALL,
	HOUSING_DOOR,
	HOUSING_WINDOW,
	HOUSING_FLOOR,
	HOUSING_END,
	HOUSING_ETC
};

enum OBJ_STATE_TYPE {
	OBJ_STATE_ATTACK,
	OBJ_STATE_FOLLOW,
	OBJ_STATE_EVASION,
	OBJ_STATE_IDLE,
	OBJ_STATE_DIE,
	OBJ_STATE_RESPAWN,
	OBJ_STATE_HEAL,
	OBJ_STATE_END
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};