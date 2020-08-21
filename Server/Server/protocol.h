#pragma once

/* ----------------------------------------
[NETWORK]
-----------------------------------------*/
constexpr	int	SERVER_PORT = 9000;
constexpr	int	MAX_USER = 100;
constexpr	int	NO_TARGET = MAX_USER;
constexpr	int	BEGIN_ANIMAL = MAX_USER;
constexpr	int	ANIMAL_BEAR = 50;
constexpr	int	ANIMAL_BOAR = 50;
constexpr	int	ANIMAL_DEER = 100;
constexpr	int	ANIMAL_WOLF = 100;
constexpr	int	MAX_ANIMAL = ANIMAL_BEAR + ANIMAL_BOAR + ANIMAL_DEER;
constexpr	int	END_ANIMAL = MAX_USER + MAX_ANIMAL;
constexpr	int	BEGIN_NATURAL = END_ANIMAL;

constexpr	int	MAX_BUF = 255;
constexpr	int	MAX_STR_LEN = 15;
constexpr	int MAX_MSG_LEN = 50;
constexpr	int MAX_QUICK_SLOT = 5;

//char SERVERIP[20] = "127.0.0.1";

// Client -> Server Packet Protocol
constexpr	char	CS_LOGIN = 0;
constexpr	char	CS_MOVE = 1;
constexpr	char	CS_CHAT = 2;
constexpr	char	CS_LOGOUT = 3;
constexpr	char	CS_POS = 4;
constexpr	char	CS_ROT = 5;
constexpr	char	CS_COLLISION = 6;
constexpr	char	CS_HOUSING_INSTALL = 7;
constexpr	char	CS_HOUSING_REMOVE = 8;
constexpr	char	CS_ATTACK = 9;
constexpr	char	CS_GET_ITEM = 10;
constexpr	char	CS_REMOVE_ITEM = 11;
constexpr	char	CS_USE_ITEM = 12;
constexpr	char	CS_DRESS_ITEM = 13;
constexpr	char	CS_UNDRESS_ITEM = 14;
constexpr	char	CS_END = 15;

// [ Server -> Client Packet Protocol ]
// - Player
constexpr	char	SC_LOGIN_OK = 0;
constexpr	char	SC_LOGIN_FAIL = 1;
constexpr	char	SC_DISCONNECT_SERVER = 2;
constexpr	char	SC_STATUS_PLAYER = 3;
constexpr	char	SC_PUT_PLAYER = 4;
constexpr	char	SC_POS_PLAYER = 5;
constexpr	char	SC_ROT_PLAYER = 6;
constexpr	char	SC_REMOVE_PLAYER = 7;
constexpr	char	SC_CHAT = 8;
constexpr	char	SC_ANIMATION_PLAYER = 9;

// - Animal
constexpr	char	SC_PUT_ANIMAL = 10;
constexpr	char	SC_POS_ANIMAL = 11;
constexpr	char	SC_REMOVE_ANIMAL = 12;
constexpr	char	SC_ANIMATION_ANIMAL = 13;

// - Natural
constexpr	char	SC_PUT_NATURAL = 15;
constexpr	char	SC_DESTROY_NATURAL = 16;

// Housing
constexpr	char	SC_INSTALL_HOUSE = 20;
constexpr	char	SC_REMOVE_HOUSE = 21;
constexpr	char	SC_INSTALL_HOUSING = 25;
constexpr	char	SC_REMOVE_HOUSING = 26;

// ITEM
constexpr	char	SC_ADD_ITEM = 30;

// etc
constexpr	char	SC_WEATHER = 35;
constexpr	char	SC_TIME = 36;
constexpr	char	SC_END = 40;

// About Player
constexpr float PLAYER_VIEW_RANGE = 3000.f;
constexpr float ANIMAL_VIEW_RANGE = 300.f;

// enum 
enum class PLAYER_ANIMATION_TYPE
{
	WALK, RUN, IDLE1, IDLE2, DIE, TAKE_WEAPON, ATTACK1, ATTACK2, ATTACK3, HIT1, HIT2, JUMP, END
};

enum class ANIMAL_ANIMATION_TYPE { WALK, RUN, IDLE, EAT, DIE, ATTACK };
#pragma	pack(push, 1)
// ___________________________________________________________________
//						[ Sever -> Client ]
// ___________________________________________________________________
// Player
struct sc_login_ok_packet {
	char	size;
	char	type;
	USHORT	id;
};

struct sc_login_fail_packet {
	char			size;
	char			type;
};

struct sc_logout_packet{
	char	size;
	char	type;
	USHORT id;
};

struct sc_death_player_packet {
	char size;
	char type;
	USHORT id;

	float	fHealth;
	float	fStamina;
	float	fHungry;

	Vec3	vLocalPos;
	Vec3	vLocalRot;
};

struct sc_status_player_packet{
	char size;
	char type;
	USHORT id;

	float	fHealth;
	float	fThrist;
	float	fHungry;
};

struct sc_put_player_packet {
	char size;
	char type;
	USHORT id;

	Vec3	vPos;
	Vec3	vRot;
};

struct sc_pos_player_packet{
	char size;
	char type;
	USHORT id;

	Vec3 vPos;
	Vec3 vRot;
	unsigned move_time;
};

struct sc_rot_player_packet {
	char size;
	char type;
	USHORT id;
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
	USHORT id;
};

struct sc_animation_player_packet
{
	char size;
	char type;
	UINT animation_uiType;
	USHORT id;
};

// NPC
struct sc_wake_up_packet{
	char size;
	char type;
	USHORT id;
};
struct sc_put_npc_packet {
	char size;
	char type;
	USHORT id;

	UINT	eType;
	Vec3	vPos;
	Vec3	vRot;
};

struct sc_pos_npc_packet{
	char size;
	char type;
	USHORT id;

	UINT eType;
	Vec3 vPos;
	Vec3 vRot;
};

struct sc_remove_npc_packet
{
	char size;
	char type;
	USHORT id;
};

struct sc_status_npc_packet
{
	char size;
	char type;
	USHORT id;

	float fHealth;
	float fStamina;
	float fDamage;

};
struct sc_animation_npc_packet
{
	char size;
	char type;
	char animation;
	USHORT id;
};

// [ Housing ]
struct sc_install_housing_packet
{
	char	size;
	char	type;
	USHORT	house_id;

	UINT	housing_type;

	Vec3	vLocalPos;
	Vec3	vLocalRot;
	Vec3	vLocalScale;

	Vec3	m_vOffsetPos;
	Vec3	m_vOffsetScale;
};

struct sc_remove_housing_packet
{
	char	size;
	char	type;
	USHORT	house_id;
};

// [ Natural ]
struct sc_put_natural_packet
{
	char size;
	char type;
	USHORT natural_id;
	float	fHealth;
	bool	bDestroy;
	Vec3	vLocalPos;
	Vec3	vLocalScale;
	Vec3	vOffsetPos;
	Vec3	vOffsetScale;
};

struct sc_destroy_natural_packet
{
	char size;
	char type;
	USHORT natural_id;
};

// [ Item ]
struct sc_add_item_packet
{
	char size;
	char type;
	UINT uiItemType;
	UINT uiInven_num;
};

// [ Etc ]
struct sc_weather_packet
{
	char size;
	char type;
	bool bRain;
};

struct sc_time_packet
{
	char size;
	char type;
	float fTime;
};

struct sc_disconnect_server_packet {
	char	size;
	char	tpye;
};

// ___________________________________________________________________
//						[ Client -> Server ]
// ___________________________________________________________________

struct cs_login_packet {
	char			size;
	char			type;
	wchar_t			player_id[MAX_STR_LEN];	
};

struct cs_move_packet {
	char size;
	char type;
	USHORT id;

	bool bRun;
	Vec3 vLocalPos;
	Vec3 vWorldDir;
	float fHeight;
};

struct cs_pos_packet {
	char size;
	char type;
	Vec3 vLocalPos;
};

struct cs_rot_packet {
	char size;
	char type;
	Vec3	vRot;
};

struct cs_chat_packet {
	char size;
	char type;
	USHORT id;
	char meesage[MAX_STR_LEN];
};

struct cs_collision_packet {
	char	size;
	char	type;
	UINT	collision_uitype; // 0 : animal 1: natural 2: house
	USHORT	collision_id;
	bool	bRun;
};

struct cs_packet_chat {
	char	size;
	char	type;
	wchar_t	message[MAX_STR_LEN];
};

struct cs_packet_logout {
	char	size;
	char	type;
	USHORT id;
};

struct cs_attack_packet {
	char size;
	char type;
	UINT attack_uiType;
	USHORT attack_id;
};
//struct cs_collision_packet {
//	char	size;
//	char	type;
//	USHORT id;
//	bool	bRun;
//};

// [ Housing ] 
struct cs_install_housing_packet
{
	char	size;
	char	type;
	USHORT	house_id;

	UINT	housing_type;
	
	Vec3	vLocalPos;
	Vec3	vLocalRot;
	Vec3	vLocalScale;

	Vec3	vOffsetPos;
	Vec3	vOffsetScale;
};

struct cs_remove_housing_packet
{
	char	size;
	char	type;
	USHORT	house_id;
};

struct cs_natural_attack_packet
{
	char	size;
	char	type;
	USHORT	natural_id;
};

struct cs_item_get_packet
{
	char	size;
	char	type;
	UINT	uiType;
	UINT	uiInvenNum;
};

struct cs_item_remove_packet
{
	char	size;
	char	type;
	UINT	uiType;
	UINT	uiInvenNum;
};

#pragma pack (pop)