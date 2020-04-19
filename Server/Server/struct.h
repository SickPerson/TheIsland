#pragma once

/*_____________________________________________
			[통신을 위한 구조체]
_______________________________________________*/
typedef struct Over_ex
{
	WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_DataBuffer;
	char			m_MessageBuffer[MAX_BUF];
	EVENT_TYPE		m_Event;
}OVER_EX, POVER_EX;

struct User_info {
	SOCKET				socket;
	OVER_EX				over;
	unsigned short		id;
	DirectX::XMFLOAT3	pos;
	set<unsigned int>	near_id;
	mutex				near_lock;
};

/*_____________________________________________
			[Timer Event]
_______________________________________________*/
struct Object_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EVENT_TYPE	m_EventType;
	EVENT_TYPE	m_OverType;
	unsigned short m_usID;
	unsigned short m_usOtherID;

	constexpr bool operator <(const Object_Event& _left) const
	{
		return wakeup_time > _left.wakeup_time;
	}
};

/*_____________________________________________
			[DataBase Event]
_______________________________________________*/
typedef struct DataBase_Event {
	char	state;
	unsigned short client_num;
	wchar_t	id[MAX_STR_LEN];

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

/*_____________________________________________
			[Object Struct]
_______________________________________________*/
struct Object {
	int iHp;
	int iStamina;
	int iHunger;
	int iThirst;

	float fPosX;
	float fPosY;
	float fPosZ;

	float fDirX;
	float fDirY;
	float fDirZ;

	chrono::high_resolution_clock::time_point m_tStartTime;
};

struct User_Data {
	std::string m_sID;
	unsigned short m_usID;
	int m_iPlayerNum;

	User_Data(std::string sID, unsigned short usNum, int iPlayerNum) {
		m_sID = sID;
		m_usID = usNum;
		m_iPlayerNum = iPlayerNum;
	}
};