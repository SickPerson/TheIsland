#pragma once

enum EVENT_TYPE {
	EV_RECV,
	EV_SEND,
	EV_UPDATE,
	EV_PLAYER_UPDATE,
	EV_DB,
	EV_END
};

enum DATABASE_TYPE {
	DB_LOGIN,
	DB_LOGOUT,
	DB_END
};

enum OBJ_LOCK_TYPE {
	OBJ_HP,
	OBJ_STAMINA,
	OBJ_HUNGER,
	OBJ_THIRST,
	OBJ_ANITIME,
	OBJ_POS,
	OBJ_DIR,
	OBJ_END
};

/*int iHp;
int iStamina;
int iHunger;
int iThirst;

float fPosX;
float fPosY;
float fPosZ;

float fDirX;
float fDirY;
float fDirZ;*/