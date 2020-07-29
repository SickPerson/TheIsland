#pragma once
#include "stdafx.h"
#include "Playerpool.h"
#include "Monsterpool.h"

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
<<<<<<< HEAD
	recursive_mutex	m_rmProcessMutex;
=======
	static CPlayerpool*	m_pPlayerPool;
	static concurrent_unordered_set<unsigned int> m_cusLoginList;
	static concurrent_priority_queue<Object_Event>	m_cpqEventQueue;
>>>>>>> master

public:
	static CPlayerpool*	m_pPlayerPool;
	static CMonsterpool* m_pMonsterPool;
	//static concurrent_unordered_map<unsigned int, wstring> m_cumLoginList; // 로그인 리스트
	static concurrent_unordered_set<unsigned int>	m_cusLoginList; // 로그인 리스트
	static concurrent_priority_queue<Update_Event>	m_cpqEventQueue; // 이벤트 큐(Timer 이벤트)

public:
	concurrent_unordered_set<unsigned int>& GetLoginList()
	{
		lock_guard<recursive_mutex>lock(m_rmProcessMutex);
		return m_cusLoginList;
	}
<<<<<<< HEAD
	void CopyBeforeLoginList(concurrent_unordered_set<unsigned int>& _cusList)
=======
	void CopyBefore(concurrent_unordered_set<unsigned int>& _cusList)
>>>>>>> master
	{
		lock_guard<recursive_mutex> lock(m_rmProcessMutex);
		_cusList = m_cusLoginList;
	}
<<<<<<< HEAD
	void DeleteLoginList(unsigned int& login_Id)
	{
		concurrent_unordered_set<unsigned int> list;
		CopyBeforeLoginList(list);
=======
	void DeleteList(unsigned int _usID)
	{
		concurrent_unordered_set<unsigned int> list;
		CopyBefore(list);
>>>>>>> master
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
<<<<<<< HEAD
	void InsertLoginList(unsigned int& login_Id)
=======
	void InsertList(unsigned int _usID)
>>>>>>> master
	{
		lock_guard<recursive_mutex>	lock(m_rmProcessMutex);
		m_cusLoginList.insert(login_Id);
	}
<<<<<<< HEAD
	bool ExistLoginList(unsigned int& login_Id)
=======
	bool CheckList(unsigned int _usID)
>>>>>>> master
	{
		if (m_cusLoginList.count(login_Id) != 0)
			return true;
		return false;
	}
	static unsigned int GetNewID();
public:
	static void PushEventQueue(Update_Event& _ev)
	{
<<<<<<< HEAD
		m_cpqEventQueue.push(_ev);
	}
	static bool TryPopEventQueue(Update_Event& _ev)
=======
		CProcess::m_cpqEventQueue.clear();
		CProcess::m_cusLoginList.clear();
	}
public:
	static void PostEvent(unsigned int _usID, unsigned int _usOther, EVENT_TYPE _cOverEvent, EVENT_TYPE _cEvent, const std::chrono::high_resolution_clock::time_point& _TimePoint)
>>>>>>> master
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

