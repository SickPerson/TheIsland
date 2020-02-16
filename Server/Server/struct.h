#pragma once

///////////////////////////////////////
// 통신을 위한 구조체
///////////////////////////////////////
typedef struct OVER_EX
{
	WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_DataBuffer;
	UCHAR			m_MessageBuffer[MAX_BUF];
	EVENT_TYPE		m_Event;
}s_OVER_EX, s_pOVER_EX;

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