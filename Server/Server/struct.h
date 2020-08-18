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
	char			m_Status;
	USHORT	m_usOtherID;
}OVER_EX, POVER_EX;

struct User_info {
	SOCKET				socket;
	OVER_EX				over;
	USHORT		id;
	DirectX::XMFLOAT3	pos;
	set<USHORT>	near_id;
	mutex				near_lock;
};

/*_____________________________________________
			[Timer Event]
_______________________________________________*/
struct Update_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EVENT_TYPE		m_EventType;
	UINT	m_ObjState;
	USHORT m_Do_Object;
	USHORT m_From_Object;

	constexpr bool operator <(const Update_Event& _left) const
	{
		return wakeup_time > _left.wakeup_time;
	}
};

/*_____________________________________________
			[DataBase Event]
_______________________________________________*/
typedef struct DB_Event {
	UINT state; // DataBase_TYPE
	int inum; // ID 시드값
	wstring sid; // ID

	float fHealth;
	float fHungry;
	float fThirst;

	float fX;
	float fY;
	float fZ;
};

/*_____________________________________________
			[Object Struct]
_______________________________________________*/
struct Object {
	int iHp;
	int iStamina;
	int iHungry;
	int iThirst;

	float fPosX;
	float fPosY;
	float fPosZ;

	float fRotX;
	float fRotY;
	float fRotZ;

	chrono::high_resolution_clock::time_point m_tStartTime;
};

/*_____________________________________________
			[Monster Event]
_______________________________________________*/
struct Monster_Event {
	DirectX::XMFLOAT3 m_xmf3ToTarget;
	float m_fRotate;
	char	m_eState;
	Monster_Event() {}
	Monster_Event(DirectX::XMFLOAT3 xmf3ToTarget, float fRotate, UINT eState) {
		m_xmf3ToTarget = xmf3ToTarget; m_fRotate = fRotate; m_eState = eState;
	}
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