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
	unsigned short	m_usOtherID;
}OVER_EX, POVER_EX;

struct User_info {
	SOCKET				socket;
	OVER_EX				over;
	unsigned short		id;
	DirectX::XMFLOAT3	pos;
	set<unsigned short>	near_id;
	mutex				near_lock;
};

/*_____________________________________________
			[Timer Event]
_______________________________________________*/
struct Update_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
	EVENT_TYPE		m_EventType;
	OBJ_STATE_TYPE	m_ObjState;
	unsigned short m_Do_Object;
	unsigned short m_From_Object;

	constexpr bool operator <(const Update_Event& _left) const
	{
		return wakeup_time > _left.wakeup_time;
	}
};

/*_____________________________________________
			[DataBase Event]
_______________________________________________*/
typedef struct DB_Event {
	char state; // DataBase_TYPE
	int inum; // ID 시드값
	wstring sid; // ID

	int ihp;
	int istamina;
	int ihungry;
	int ithirst;

	float fposx;
	float fposy;
	float fposz;
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
	Monster_Event(DirectX::XMFLOAT3 xmf3ToTarget, float fRotate, OBJ_STATE_TYPE eState) {
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
	std::string m_sID;
	unsigned short m_uiID;
	int m_iPlayerNum;

	User_Data(std::string sID, unsigned short usNum, int iPlayerNum) {
		m_sID = sID;
		m_uiID = usNum;
		m_iPlayerNum = iPlayerNum;
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