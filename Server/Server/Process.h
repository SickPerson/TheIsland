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
	recursive_mutex	m_rmProcessMutex;

public:
	static CPlayerpool*	m_pPlayerPool;
	static CMonsterpool* m_pMonsterPool;

	static concurrent_unordered_set<unsigned int>	m_cusLoginList; // 로그인 리스트
	static concurrent_priority_queue<Update_Event>	m_cpqEventQueue; // 이벤트 큐(Timer 이벤트)

public:
	concurrent_unordered_set<unsigned int>& GetLoginList()
	{
		lock_guard<recursive_mutex>lock(m_rmProcessMutex);
		return m_cusLoginList;
	}
	void CopyBeforeLoginList(concurrent_unordered_set<unsigned int>& _cusList)
	{
		lock_guard<recursive_mutex> lock(m_rmProcessMutex);
		_cusList = m_cusLoginList;
	}
	void DeleteLoginList(unsigned int& login_Id)
	{
		concurrent_unordered_set<unsigned int> list;
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
	void InsertLoginList(unsigned int& login_Id)
	{
		lock_guard<recursive_mutex>	lock(m_rmProcessMutex);
		m_cusLoginList.insert(login_Id);
	}
	bool ExistLoginList(unsigned int& login_Id)
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
			if (_ev.wakeup_time < std::chrono::high_resolution_clock::now())
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

