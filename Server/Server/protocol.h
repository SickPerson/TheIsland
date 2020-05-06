#pragma once

/* ----------------------------------------
[NETWORK]
-----------------------------------------*/
constexpr	int	SERVER_PORT = 9000;
constexpr	unsigned int	MAX_USER	= 10;
constexpr	unsigned int	ANIMAL_BEAR = 50;
constexpr	unsigned int	ANIMAL_WILD_PIG = 100;
constexpr	unsigned int	ANIMAL_DEER = 150;
constexpr	unsigned int	MAX_ANIMAL = ANIMAL_BEAR + ANIMAL_WILD_PIG + ANIMAL_DEER;
constexpr	int	MAX_BUF = 1024;
constexpr	int	MAX_STR_LEN = 15;
constexpr	int MAX_MEG_LEN = 50;
constexpr	int MAX_QUICK_SLOT = 5;

//char SERVERIP[20] = "127.0.0.1";

// Client -> Server Packet Protocol
constexpr	char	CS_LOGIN = 0;
constexpr	char	CS_LOGOUT = 1;
constexpr	char	CS_LOOK = 2;
constexpr	char	CS_POS = 3;
constexpr	char	CS_CHAT = 4;
constexpr	int		CS_END = 6;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_IP = 0;
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

struct sc_login_ok_packet {
	char	size;
	char	type;
	unsigned int	id;
};

struct sc_login_fail_packet {
	char	size;
	char	type;
};

struct sc_disconnect_packet {
	char	size;
	char	type;
	unsigned int id;
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
};

struct sc_accept_packet {
	char size;
	char type;
	unsigned int id;

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

	float fPosX;
	float fPosY;
	float fPosZ;

	float fDirX;
	float fDirY;
	float fDirZ;
};

struct sc_chat_packet {
	char size;
	char type;
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

// ___________________________________________________________________
//						[ Client -> Server ]
// ___________________________________________________________________

struct cs_login_packet {
	char	size;
	char	type;
	unsigned int id;
	char	player_id[MAX_STR_LEN];
};

struct cs_chat_packet {
	char size;
	char type;
	unsigned int id;
	wchar_t meesage[MAX_STR_LEN];
};

struct cs_packet_move {
	char	size;
	char	type;
	char	direction;		// 0:Up, 1:Down, 2:Left, 3:Right
};

struct cs_packet_attack {
	char	size;
	char	type;
};

struct cs_packet_chat {
	char	size;
	char	type;
	wchar_t	message[MAX_STR_LEN];
};

struct cs_packet_logout {
	char	size;
	char	type;
};

#pragma pack (pop)