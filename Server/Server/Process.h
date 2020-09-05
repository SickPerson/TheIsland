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
	template	<typename T1, typename T2>
	bool CollisionSphere(std::pair<const USHORT, T1*> pFirst, std::pair<const USHORT, T2*> pSecond, float fOffset = 1.f)
	{
		Vec3 vPos1 = pFirst.second->GetLocalPos();
		Vec3 vPos2 = pSecond.second->GetLocalPos();

		Vec3 vScale1 = pFirst.second->GetLocalScale();
		Vec3 vScale2 = pSecond.second->GetLocalScale();

		Vec3 vColScale1 = pFirst.second->GetLocalScale() * fOffset;
		Vec3 vColScale2 = pSecond.second->GetLocalScale();

		float fDist = powf(vPos1.x - vPos2.x, 2) + powf(vPos1.y - vPos2.y, 2) + powf(vPos1.z - vPos2.z, 2);
		fDist = sqrtf(fDist);

		if (fDist > fabsf(vScale1.x * vColScale1.x) + fabsf(vScale2.x * vColScale2.x))
			return false;
		return true;

	}
	template <typename T1, typename T2>
	bool CollisionSphere(T1* pFirst, T2* pSecond, float fOffset = 1.f)
	{
		Vec3 vPos1 = pFirst->GetLocalPos();
		Vec3 vPos2 = pSecond->GetLocalPos();

		Vec3 vScale1 = pFirst->GetLocalScale();
		Vec3 vScale2 = pSecond->GetLocalScale();

		Vec3 vColScale1 = pFirst->GetOffsetScale() * fOffset;
		Vec3 vColScale2 = pSecond->GetOffsetScale();

		float fDist = powf(vPos1.x - vPos2.x, 2) + powf(vPos1.z - vPos2.z, 2);
		fDist = sqrtf(fDist);

		float f = fabsf(vScale1.x * vColScale1.x) + fabsf(vScale2.x * vColScale2.x);
		if (fDist > f)
			return false;
		return true;

	}
	template <typename T1, typename T2>
	float CalculationDistance(T1* pFirst, T2* pSecond) {
		Vec3 vPos1 = pFirst->GetLocalPos();
		Vec3 vPos2 = pSecond->GetLocalPos();

		float fDist = powf(vPos2.z - vPos1.z, 2) + powf(vPos2.x - vPos1.x, 2);
		fDist = sqrtf(fDist);

		return fDist;
	}
	template	<typename T>
	bool CollisionHouse(std::pair<const USHORT, T*> pFirst, std::pair<const USHORT, CHousing*> pSecond, char eType) {
		Vec3 vPos1 = pFirst.second->GetLocalPos();
		Vec3 vPos2 = pSecond.second->GetLocalPos();

		Vec3 vScale1 = pFirst.second->GetLocalScale();
		Vec3 vScale2 = pSecond.second->GetLocalScale();

		Vec3 vColScale1 = pFirst.second->GetOffsetScale();
		Vec3 vColScale2 = Vec3(1.f, 1.f, 1.f);

		Vec3 vHouseOffsetScale = pSecond.second->GetOffsetScale();

		switch (eType)
		{
		case HOUSING_FOUNDATION:
			vColScale2 = vHouseOffsetScale;
			break;
		case HOUSING_WALL:
		case HOUSING_WINDOW:
		case HOUSING_DOOR:
		{
			Vec3 vRot = pSecond.second->GetLocalRot();
			if (vRot.y != 0.f)
			{
				vColScale2 = Vec3(vHouseOffsetScale.z, vHouseOffsetScale.y, vHouseOffsetScale.x);
			}
			else
			{
				vColScale2 = Vec3(vHouseOffsetScale.x, vHouseOffsetScale.y, vHouseOffsetScale.z);
			}
		}
		break;
		case HOUSING_FLOOR:
		{
			vColScale2 = vHouseOffsetScale;
		}
		break;
		default:
			vColScale2 = vHouseOffsetScale;
			break;
		}

		vScale1 *= vColScale1;
		vScale2 *= vColScale2;

		Vec3 vMax, vOtherMax;
		Vec3 vMin, vOtherMin;

		Vec3 vPos = pFirst.second->GetLocalPos();
		vPos.y += 50.f;

		vMax = vPos + vScale1;
		vMin = vPos - vScale1;
		vOtherMax = vPos1 + vScale2;
		vOtherMin = vPos2 - vScale2;

		if (vMax.x < vOtherMin.x || vMin.x > vOtherMax.x) return false;
		if (vMax.y < vOtherMin.y || vMin.y > vOtherMax.y) return false;
		if (vMax.z < vOtherMin.z || vMin.z > vOtherMax.z) return false;

		return true;
	}

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
	void PushEvent_Natural_Die(USHORT NaturalId);
	void PushEvent_Natural_Respawn(USHORT NaturalId);

public: // Etc
	void PushEvent_Etc_Player_Collision();
	void PushEvent_Etc_Animal_Collision();
	void PushEvent_Rot();
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
		m_cusLoginList.insert(login_Id);
		cout << "현재 총 접속 인원 : " << m_cusLoginList.size() << endl;
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