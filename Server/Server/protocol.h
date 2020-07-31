#pragma once

/* ----------------------------------------
[NETWORK]
-----------------------------------------*/
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
constexpr	int MAX_QUICK_SLOT = 5;

//char SERVERIP[20] = "127.0.0.1";

// Client -> Server Packet Protocol
constexpr	char	CS_LOGIN =	0;
constexpr	char	CS_MOVE =	1;
constexpr	char	CS_CHAT =	2;
constexpr	char	CS_LOGOUT = 3;
constexpr	int		CS_END =	4;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_OK = 0;
constexpr	char	SC_LOGIN_FAIL = 1;
constexpr	char	SC_STATUS_PLAYER = 2;
constexpr	char	SC_PUT_PLAYER = 3;
constexpr	char	SC_REMOVE_PLAYER = 4;
constexpr	char	SC_POS = 5;
constexpr	char	SC_ROT = 6;
constexpr	char	SC_CHAT = 7;
constexpr	char	SC_ANIMATION_PLAYER = 8;
// Monster, NPC ฐüทร
constexpr	char	SC_WAKE_UP_NPC = 9;
constexpr	char	SC_PUT_NPC = 10;
constexpr	char	SC_POS_NPC = 11;
constexpr	char	SC_REMOVE_NPC = 12;
constexpr	char	SC_STATUS_NPC = 13;
constexpr	char	SC_ANIMATION_NPC = 14;

constexpr	char	SC_END = 15;

// About Player
constexpr float PLAYER_BETWEEN_RANGE = 3000.f;
constexpr float MONSTER_BETWEEN_RANGE = 1000.f;

#pragma	pack(push, 1)
// ___________________________________________________________________
//						[ Sever -> Client ]
// ___________________________________________________________________
// Player
struct sc_login_ok_packet {
	char			size;
	char			type;
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
};

struct sc_satus_packet{
	char size;
	char type;
	unsigned int id;

	float fHP;
	float fStamina;
	float fHungry;
};

struct sc_put_player_packet {
	char size;
	char type;
	unsigned int id;

	Vec3	vPos;
	Vec3	vRot;
};

struct sc_pos_player_packet{
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
	wchar_t wcid[MAX_STR_LEN];
	char meesage[MAX_STR_LEN];
};

struct sc_remove_player_packet {
	char size;
	char type;
	unsigned int id;
};

struct sc_animation_player_packet
{
	char size;
	char type;
	char animation;
	unsigned int id;
};

// NPC
struct sc_wake_up_packet{
	char size;
	char type;
	unsigned int id;
};
struct sc_put_npc_packet {
	char size;
	char type;
	unsigned int id;

	Vec3	vPos;
	Vec3	vRot;
};

struct sc_pos_npc_packet{
	char size;
	char type;
	unsigned int id;
	Vec3 vPos;
	Vec3 vRot;
	unsigned move_time;
};

struct sc_remove_npc_packet
{
	char size;
	char type;
	unsigned int id;
};

struct sc_animation_npc_packet
{
	char size;
	char type;
	char animation;
	unsigned int id;
};

// ___________________________________________________________________
//						[ Client -> Server ]
// ___________________________________________________________________

struct cs_login_packet {
	char			size;
	char			type;
	unsigned int	id;
	wchar_t			player_id[MAX_STR_LEN];	
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