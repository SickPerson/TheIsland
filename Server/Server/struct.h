#pragma once

///////////////////////////////////////
// 통신을 위한 구조체
///////////////////////////////////////
typedef struct _Context
{
	WSAOVERLAPPED	m_sOverlapped;
	WSABUF			m_sWsabuf;
	BYTE			m_bType;
	UCHAR			m_cBuf[MAX_BUF];
	char			m_cState;
	char			m_cMap;
	unsigned short	m_usOtherID;
}s_Context, s_pContext;

typedef struct Position2d {
	short x;
	short y;

	Position2d(short _x, short _y) : x(_x), y(_y) {}
}POS2D, PPOS2D;

typedef struct Position3d {
	short x;
	short y;
	short z;

	Position3d(short _x, short _y, short _z) : x(_x), y(_y), z(_z) {}
}POS3D, PPOS3D;

struct OVER_EX {
	WSAOVERLAPPED	overlapped;
	WSABUF					wsabuf[1];
	char							net_buf[MAX_BUF];
	EVENT_TYPE			event;
};

struct PLAYER_INFO {
	SOCKET				socket;
	OVER_EX				over;
	unsigned int			id;

	// game contents
	POS3D					pos;
	set<unsigned int>near_id;
	mutex					near_lock;
};

struct T_EVENT {
	int do_object;
	EVENT_TYPE event_type;
	high_resolution_clock::time_point	start_time;
	int from_object;

	constexpr bool operator <(const T_EVENT& _Left) const {
		return (start_time > _Left.start_time);
	}
};