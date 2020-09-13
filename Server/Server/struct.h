#pragma once

/*_____________________________________________
			[통신을 위한 구조체]
_______________________________________________*/
typedef struct Over_ex
{
	WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_DataBuffer;
	char			m_MessageBuffer[1024];
	EVENT_TYPE		m_Event;
	char			m_Status;
	USHORT	m_usOtherID;
}OVER_EX, POVER_EX;

typedef struct Client {
	OVER_EX m_RecvOver;
	int		m_iPrev;
	int		m_iCur;

	char	m_PacketBuf[MAX_BUF];
};

/*_____________________________________________
			[Update Event]
_______________________________________________*/
struct Update_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EVENT_TYPE		m_EventType;
	char			m_eObjUpdate;
	USHORT			m_Do_Object;
	USHORT			m_From_Object;

	constexpr bool operator <(const Update_Event& _left) const
	{
		return wakeup_time > _left.wakeup_time;
	}
};

/*_____________________________________________
			[DataBase Event]
_______________________________________________*/
typedef struct DB_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EVENT_TYPE	m_EventType;
	char		m_eDbUpdate;

	UINT state; // DataBase_TYPE
	int inum; // ID 시드값
	wstring strID; // ID

	float fHealth;
	float fHungry;
	float fThirst;

	float fX;
	float fY;
	float fZ;
};

struct UserData {
	int m_iPlayerNum;
	wstring m_wsID;

	int hp;
	int stamina;
	int hungry;
	int thirst;
	float x, y, z;

	UserData(int _Num, wstring _wsID, int _hp, int _stamina, int _hungry, int _thirst, float _x, float _y, float _z)
	{
		int m_iPlayerNum = _Num;
		m_wsID = _wsID;
		hp = _hp;
		stamina = _stamina;
		hungry = _hungry;
		thirst = _thirst;
		x = _x;
		y = _y;
		z = _z;
	}
};

struct User_Data {
	string m_sId;
	USHORT m_usId;
	int	m_iUserNum;

	User_Data(string sId, USHORT usId, int iUserNum) {
		m_sId = sId;
		m_usId = usId;
		m_iUserNum = iUserNum;
	}
};

struct tPlayerStatus
{
	float fHealth;
	float fHungry;
	float fThirst;
	float fSpeed;
	float fDamage;
};

struct tAnimalStatus
{
	float fHealth;
	float fStamina;
	float fSpeed;
	float fDamage;

	BEHAVIOR_TYPE eType;
	ANIMAL_TYPE eKind;
};