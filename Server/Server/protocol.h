#pragma once

/* ----------------------------------------
[NETWORK]
-----------------------------------------*/
<<<<<<< HEAD
constexpr	int				SERVER_PORT = 9000;
constexpr	unsigned int	MAX_USER	= 5000;
constexpr	unsigned int	NO_TARGET = MAX_USER;
constexpr	unsigned int	ANIMAL_BEAR = 50;
constexpr	unsigned int	ANIMAL_BOAR = 50;
constexpr	unsigned int	ANIMAL_DEER = 100;
constexpr	unsigned int	ANIMAL_WOLF = 100;
constexpr	unsigned int	MAX_ANIMAL = ANIMAL_BEAR + ANIMAL_BOAR + ANIMAL_DEER;
constexpr	int	MAX_BUF = 255;
constexpr	int	MAX_STR_LEN = 15;
constexpr	int MAX_MSG_LEN = 50;
=======
constexpr	int	SERVER_PORT = 9000;
constexpr	unsigned int	MAX_USER	= 3;
constexpr	unsigned int	ANIMAL_BEAR = 50;
constexpr	unsigned int	ANIMAL_WILD_PIG = 100;
constexpr	unsigned int	ANIMAL_DEER = 150;
constexpr	unsigned int	MAX_ANIMAL = ANIMAL_BEAR + ANIMAL_WILD_PIG + ANIMAL_DEER;
constexpr	int	MAX_BUF = 1024;
constexpr	int	MAX_STR_LEN = 15;
constexpr	int MAX_MEG_LEN = 50;
>>>>>>> master
constexpr	int MAX_QUICK_SLOT = 5;

//char SERVERIP[20] = "127.0.0.1";

// Client -> Server Packet Protocol
<<<<<<< HEAD
constexpr	char	CS_LOGIN =	0;
constexpr	char	CS_MOVE =	1;
constexpr	char	CS_CHAT =	2;
constexpr	char	CS_LOGOUT = 3;
constexpr	int		CS_END =	4;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_OK = 0;
constexpr	char	SC_LOGIN_FAIL = 1;
constexpr	char	SC_PUT_PLAYER = 2;
constexpr	char	SC_REMOVE_PLAYER = 3;
constexpr	char	SC_POS = 4;
constexpr	char	SC_ROT = 5;
constexpr	char	SC_CHAT = 6;
// Monster, NPC °ü·Ã
constexpr	char	SC_LOGIN_NPC = 7;
constexpr	char	SC_PUT_NPC = 8;
constexpr	char	SC_POS_NPC = 9;
constexpr	char	SC_REMOVE_NPC = 10;

constexpr	char	SC_END = 11;

// About Player
constexpr float PLAYER_BETWEEN_RANGE = 1000.f;
constexpr float MONSTER_BETWEEN_RANGE = 500.f;
=======
constexpr	char	CS_LOGIN = 0;
constexpr	char	CS_LOGOUT = 1;
constexpr	char	CS_LOOK = 2;
constexpr	char	CS_POS = 3;
constexpr	char	CS_CHAT = 4;
constexpr	int		CS_END = 6;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_OK = 1;
constexpr	char	SC_LOGIN_FAIL = 2;
constexpr	char	SC_LOGIN = 3;
constexpr	char	SC_CONNECT = 4;
constexpr	char	SC_DISCONNECT = 5;
constexpr	char	SC_POS = 6;
constexpr	char	SC_CHAT = 7;
constexpr	char	SC_STAT_CHANGE = 8;
constexpr	char	SC_REMOVE_OBJECT = 9;
constexpr	char	SC_END = 10;

// About Player
constexpr float PLAYER_BETWEEN_RANGE = 2500.f;

#pragma	pack(push, 1)
// ___________________________________________________________________
//						[ Sever -> Client ]
// ___________________________________________________________________
struct sc_login_state_packet {
	char size;
	char type;
	unsigned int id;
};

struct sc_login_ip_packet {
	char size;
	char type;
	unsigned int id;
};
>>>>>>> master

#pragma	pack(push, 1)
// ___________________________________________________________________
//						[ Sever -> Client ]
// ___________________________________________________________________
struct sc_login_ok_packet {
<<<<<<< HEAD
	char			size;
	char			type;
=======
	char	size;
	char	type;
>>>>>>> master
	unsigned int	id;
};

struct sc_login_fail_packet {
	char			size;
	char			type;
};

struct sc_logout_packet{
	char	size;
	char	type;
	unsigned int id;
<<<<<<< HEAD
=======
};

struct sc_first_status_packet {
	char size;
	char type;
	unsigned int id;

	int HP;
	int Stamina;
	int Hunger;
	int Thirst;

	float fPosX;
	float fPosY;
	float fPosZ;

	float fDirX;
	float fDirY;
	float fDirZ;
>>>>>>> master
};

struct sc_stat_change_packet {
	char size;
	char type;
	unsigned int id;
<<<<<<< HEAD

	float fHP;
	float Stamina;
	float Hungry;
};
=======

	int HP;

	float fPosX;
	float fPosY;
	float fPosZ;

	float fDirX;
	float fDirY;
	float fDirZ;
};

struct sc_pos_packet {
	char size;
	char type;
	unsigned int id;
>>>>>>> master

struct sc_put_player_packet {
	char size;
	char type;
	unsigned int id;

	Vec3	vPos;
	Vec3	vRot;
};

struct sc_put_npc_packet {
	char size;
	char type;
	unsigned int id;

	Vec3	vPos;
	Vec3	vRot;
};

struct sc_pos_player_packet 
{
	char size;
	char type;
	unsigned int id;
	Vec3 vPos;
	Vec3 vRot;
	unsigned move_time;
};

struct sc_pos_npc_packet
{
	char size;
	char type;
	unsigned int id;
	Vec3 vPos;
	Vec3 vRot;
	unsigned move_time;
};

struct sc_rot_packet {
	char size;
	char type;
	unsigned int id;
	Vec3	vRot;
};

struct sc_chat_packet {
	char size;
	char type;
<<<<<<< HEAD
	wchar_t wcid[MAX_STR_LEN];
	char meesage[MAX_STR_LEN];
};

struct sc_remove_player_packet {
	char size;
	char type;
	unsigned int id;
};

struct sc_remove_npc_packet
{
	char size;
	char type;
	unsigned int id;
};
=======
	unsigned int id;
	char meesage[MAX_STR_LEN];
};

//struct sc_packet_position {
//	char size;
//	char type;
//	int	id;
//	short x, y;
//};
//
//struct sc_packet_stat_change {
//	char size;
//	char type;
//	int id;
//	int hp, level, exp;
//	bool	attack_check;
//};
//
//struct sc_packet_remove_object {
//	char size;
//	char type;
//	int id;
//};
//
//struct sc_packet_add_object {
//	char size;
//	char type;
//	int id;
//	int obj_class;		// 1: PLAYER,    2:ORC,  3:Dragon, ¡¦..
//	short x, y;
//	int	hp, level, exp;
//};
>>>>>>> master

// ___________________________________________________________________
//						[ Client -> Server ]
// ___________________________________________________________________

struct cs_login_packet {
<<<<<<< HEAD
	char			size;
	char			type;
	unsigned int	id;
	wchar_t			player_id[MAX_STR_LEN];	
=======
	char	size;
	char	type;
	unsigned int id;
	char	player_id[MAX_STR_LEN];
};

struct cs_pos_packet {
	char size;
	char type;
	unsigned int id;

	float fposX; float fposY; float fposZ;
	float fDirX; float fDirY; float fDirZ;
};

struct cs_chat_packet {
	char size;
	char type;
	unsigned int id;
	char meesage[MAX_STR_LEN];
>>>>>>> master
};

struct cs_move_packet {
	char size;
	char type;
	unsigned int id;

	float fSpeed;
	Vec3 vDir;
	Vec3 vRot;
};

struct cs_pos_packet {
	char size;
	char type;
	unsigned int id;

	int		iHp;
	float	fSpeed;
	Vec3	vDir;
	unsigned move_time;
	
};

struct cs_rot_packet {
	char size;
	char type;
	unsigned int id;
	Vec2	vDrag;
	Vec3	vRot;
};

struct cs_chat_packet {
	char size;
	char type;
	unsigned int id;
	char meesage[MAX_STR_LEN];
};

struct cs_packet_chat {
	char	size;
	char	type;
	wchar_t	message[MAX_STR_LEN];
};

struct cs_packet_logout {
	char	size;
	char	type;
	unsigned int id;
};
// ___________________________________________________________________
//						[ Sever -> Client ]
// ___________________________________________________________________
struct sc_monster_login_packet {
	char	size;
	char	type;
	unsigned int id;
};

#pragma pack (pop)