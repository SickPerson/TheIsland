#pragma once

/* ----------------------------------------
[NETWORK]
-----------------------------------------*/
constexpr	int	SERVER_PORT = 9000;
constexpr	unsigned short	MAX_USER	= 5000;
constexpr	unsigned short	ANIMAL_BEAR = 50;
constexpr	unsigned short	ANIMAL_WILD_PIG = 100;
constexpr	unsigned short	ANIMAL_DEER = 150;
constexpr	unsigned short	MAX_ANIMAL = ANIMAL_BEAR + ANIMAL_WILD_PIG + ANIMAL_DEER;
constexpr	int	MAX_BUF = 1024;
constexpr	int	MAX_STR_LEN = 50;

extern char SERVERIP[20];

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800

// Client -> Server Packet Protocol
constexpr	char	CS_LOGIN = 0;
constexpr	char	CS_LOGOUT = 1;
constexpr	char	CS_LOOK = 2;
constexpr	char	CS_POS = 3;
constexpr	char	CS_ATTACK = 4;
constexpr	char	CS_IDLE = 5;
constexpr	int		CS_END = 6;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_OK = 0;
constexpr	char	SC_LOGIN_FAIL = 1;
constexpr	char	SC_CONNECT = 2;
constexpr	char	SC_DISCONNECT = 3;
constexpr	char	SC_POSITION = 4;
constexpr	char	SC_CHAT = 5;
constexpr	char	SC_STAT_CHANGE = 6;
constexpr	char	SC_REMOVE_OBJECT = 7;

#pragma	pack(push, 1)
// Sever -> Client
struct sc_login_state_packet {
	char size;
	char type;
	unsigned short id;
};

struct sc_login_ok_packet {
	char	size;
	char	type;
	unsigned short	id;
	char	ip[MAX_STR_LEN];
};

struct sc_login_fail_packet {
	char	size;
	char	type;
	unsigned short id;
};

struct sc_disconnect_packet {
	char	size;
	char	type;
	unsigned short id;
};

struct sc_packet_accept {
	char	size;
	char	type;
	unsigned short id;
};

struct sc_packet_position {
	char size;
	char type;
	int	id;
	short x, y;
};

struct sc_pacekt_chat {
	char size;
	char type;
	int id;
	wchar_t message[MAX_STR_LEN];
};

struct sc_packet_stat_change {
	char size;
	char type;
	int id;
	int hp, level, exp;
	bool	attack_check;
};

struct sc_packet_remove_object {
	char size;
	char type;
	int id;
};

struct sc_packet_add_object {
	char size;
	char type;
	int id;
	int obj_class;		// 1: PLAYER,    2:ORC,  3:Dragon, ¡¦..
	short x, y;
	int	hp, level, exp;
};

// client->server
struct cs_login_packet {
	char	size;
	char	type;
	char	player_id[MAX_STR_LEN];
	char	player_ip[MAX_STR_LEN];
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