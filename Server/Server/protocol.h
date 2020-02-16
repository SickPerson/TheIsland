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

using Packet = unsigned char;

#define WORLD_WIDTH 800
#define WORLD_HEIGHT 800

// Client -> Server Packet Protocol
constexpr	char	CS_LOGIN = 0;
constexpr	char	CS_LOGOUT = 1;
constexpr	char	CS_MOVE = 2;
constexpr	char	CS_ATTACK = 3;
constexpr	char	CS_CHAT = 4;

// Server -> Client Packet Protocol
constexpr	char	SC_LOGIN_OK = 0;
constexpr	char	SC_LOGIN_FAIL = 1;
constexpr	char	SC_POSITION = 2;
constexpr	char	SC_CHAT = 3;
constexpr	char	SC_STAT_CHANGE = 4;
constexpr	char	SC_REMOVE_OBJECT = 5;

#define		SC_LOGIN_OK				1
#define		SC_LOGIN_FAIL			2
#define		SC_POSITION				3
#define		SC_CHAT						4
#define		SC_STAT_CHANGE		5
#define		SC_REMOVE_OBJECT	6
#define		SC_ADD_OBJECT			7

#pragma	pack(push, 1)

struct sc_packet_login_ok {
	char	size;
	char	type;
	int	id;
	short	x, y;
	int	hp, level, exp;
};

struct sc_packet_login_fail {
	char size;
	char	type;
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

struct cs_packet_login {
	char	size;
	char	type;
	char player_id[10];
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