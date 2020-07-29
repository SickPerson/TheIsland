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
	unsigned int	m_uiOtherID;
}OVER_EX, POVER_EX;

// 임시
struct SOCKETINFO
{
	OVER_EX	recv_over;
	SOCKET	socket;
	int		id;

	bool is_connected;
	bool is_active;
	short	x, y;
	set <int> near_id;
	mutex near_lock;
	mutex vm_lock;
};

struct User_info {
	SOCKET				socket;
	OVER_EX				over;
	unsigned int		id;
	DirectX::XMFLOAT3	pos;
	set<unsigned int>	near_id;
	mutex				near_lock;
};

/*_____________________________________________
			[Timer Event]
_______________________________________________*/
struct Update_Event {
	std::chrono::high_resolution_clock::time_point wakeup_time;
<<<<<<< HEAD
	EVENT_TYPE		m_EventType;
	OBJ_STATE_TYPE	m_ObjState;
	unsigned int m_Do_Object;
	unsigned int m_From_Object;
=======
	EVENT_TYPE	m_EventType;
	EVENT_TYPE	m_OverType;
	unsigned int m_usID;
	unsigned int m_usOtherID;
>>>>>>> master

	constexpr bool operator <(const Update_Event& _left) const
	{
		return wakeup_time > _left.wakeup_time;
	}
};

/*_____________________________________________
			[DataBase Event]
_______________________________________________*/
<<<<<<< HEAD
typedef struct DB_Event {
	char state; // DataBase_TYPE
	int inum; // ID 시드값
	wstring sid; // ID
=======
typedef struct DataBase_Event {
	char	state;
	unsigned int client_num;
	wchar_t	id[MAX_STR_LEN];

	int HP;
	int Stamina;
	int Hunger;
	int Thirst;
>>>>>>> master

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
<<<<<<< HEAD
	unsigned int m_uiID;
=======
	unsigned int m_usID;
>>>>>>> master
	int m_iPlayerNum;

	User_Data(std::string sID, unsigned int usNum, int iPlayerNum) {
		m_sID = sID;
		m_uiID = usNum;
		m_iPlayerNum = iPlayerNum;
	}
};

struct tPlayerStatus
{
	float fHP;
	float fHungry;
	float fStamina;
	float fThirst;
};

struct tAnimalStatus
{
	float fHP;
	float fStamina;
	float fSpeed;
	float fDamage;

	float fBehaviorTime;

	BEHAVIOR_TYPE eType;
	ANIMAL_TYPE eKind;
};