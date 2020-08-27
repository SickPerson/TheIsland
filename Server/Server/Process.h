#pragma once
#include "stdafx.h"
#include "Objectpool.h"

class CProcess
{
public:
	explicit CProcess();
	virtual ~CProcess();

public:
	static void InitBeforeStart()
	{
		CProcess::m_cpqEventQueue.clear();
		CProcess::m_cusLoginList.clear();
	}

public:
	recursive_mutex	m_rmProcessMutex;

public:
	static USHORT	m_houseNum;
	static CObjectpool*	m_pObjectPool;

	static concurrent_unordered_set<USHORT>	m_cusLoginList; // 로그인 리스트
	static concurrent_priority_queue<Update_Event>	m_cpqEventQueue; // 이벤트 큐(Timer 이벤트)

public:
	static void Initalize();
	static void Release();
public:
	bool ObjectRangeCheck(Vec3& vFirst, Vec3& vSecond, float fDistance);

public: // - Collision
	// - Player
	bool PlayerAndAnimal_CollisionSphere(USHORT playerId, USHORT animalId, float fOffset = 1.f);
	bool PlayerAndNatural_CollisionSphere(USHORT playerId, USHORT naturalId, float fOffset = 1.f);
	bool PlayerAndHouse_Collision(USHORT playerId, USHORT houseId, char eType);
	bool PlayerAndHouse_Collision_Door(USHORT playerId, USHORT houseId, Vec3 vOffsetScale, Vec3 vOffsetPos);
	// - Animal
	bool AnimalAndPlayer_CollisionSphere(USHORT AnimalId, USHORT PlayerId, float fOffset = 1.f);
	bool AnimalAndNatural_CollisionSphere(USHORT AnimalId, USHORT NaturalId, float fOffset = 1.f);
	bool AnimalAndHouse_Collision(USHORT AnimalId, USHORT HouseId, float fOffset = 1.f);
	bool AnimalAndHouse_Collision_Door(USHORT AnimalId, USHORT HouseId, Vec3 vOffsetScale, Vec3 vOffsetPos);

public: // Animal
	void PushEvent_Animal_Behavior(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Attack(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Follow(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Evastion(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Idle(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Die(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Animal_Respawn(USHORT AnimalId);
	void PushEvnet_Animal_Damage(USHORT AnimalId, USHORT PlayerId);

public: // Natural
	void PushEvent_Natural_Respawn(USHORT NaturalId);
	void PushEvent_Natural_Damage(USHORT NaturalId, USHORT PlayerId);

public: // Etc
	void PushEvent_Etc_Player_Collision();
	void PushEvent_Etc_Animal_Collision();
	void PushEvent_Etc_Weather();
	void PushEvent_Etc_Time();

public: // DB
	void PushEvent_DB_UserSave();

public:
	concurrent_unordered_set<USHORT>& GetLoginList()
	{
		lock_guard<recursive_mutex>lock(m_rmProcessMutex);
		return m_cusLoginList;
	}
	void CopyBeforeLoginList(concurrent_unordered_set<USHORT>& _cusList)
	{
		lock_guard<recursive_mutex> lock(m_rmProcessMutex);
		_cusList = m_cusLoginList;
	}
	void DeleteLoginList(USHORT& login_Id)
	{
		concurrent_unordered_set<USHORT> list;
		CopyBeforeLoginList(list);
		for (auto au = list.begin(); au != list.end();)
		{
			if (*au == login_Id)
			{
				au = list.unsafe_erase(au);
				break;
			}
			else
				++au;
		}
		lock_guard<recursive_mutex>	lock(m_rmProcessMutex);
		m_cusLoginList = list;
	}
	void InsertLoginList(USHORT& login_Id)
	{
		lock_guard<recursive_mutex>	lock(m_rmProcessMutex);
		m_cusLoginList.insert(login_Id);
	}
	bool ExistLoginList(USHORT& login_Id)
	{
		if (m_cusLoginList.count(login_Id) != 0)
			return true;
		return false;
	}
public:
	static void PushEventQueue(Update_Event& _ev)
	{
		m_cpqEventQueue.push(_ev);
	}
	static bool TryPopEventQueue(Update_Event& _ev)
	{
		return m_cpqEventQueue.try_pop(_ev);
	}
	static bool EmptyEventQueue()
	{
		return m_cpqEventQueue.empty();
	}
	static bool CheckEventStart(Update_Event& _ev)
	{
		if (m_cpqEventQueue.try_pop(_ev))
		{
			if (_ev.wakeup_time > std::chrono::high_resolution_clock::now())
			{
				PushEventQueue(_ev);
				return false;
			}
			else
				return true;
		}
		return false;
	}
};

