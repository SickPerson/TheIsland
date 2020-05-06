#pragma once
#include "stdafx.h"
#include "Playerpool.h"

class CProcess
{
public:
	explicit CProcess();
	virtual ~CProcess();
public:
	recursive_mutex	m_rmProcessMutex;
public:
	static CPlayerpool*	m_pPlayerPool;
	static concurrent_unordered_set<unsigned int> m_cusLoginList;
	static concurrent_priority_queue<Object_Event>	m_cpqEventQueue;

public:
	void InitProcessData();

public:
	concurrent_unordered_set<unsigned int>& GetLoginList()
	{
		lock_guard<recursive_mutex>lock(m_rmProcessMutex);
		auto au = m_cusLoginList;
		return au;
	}
	void CopyBefore(concurrent_unordered_set<unsigned int>& _cusList)
	{
		lock_guard<recursive_mutex> lock(m_rmProcessMutex);
		_cusList = m_cusLoginList;
	}
	void DeleteList(unsigned int _usID)
	{
		concurrent_unordered_set<unsigned int> list;
		CopyBefore(list);
		for (auto au = list.begin(); au != list.end();)
		{
			if (*au == _usID)
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
	void InsertList(unsigned int _usID)
	{
		lock_guard<recursive_mutex>	lock(m_rmProcessMutex);
		m_cusLoginList.insert(_usID);
	}
	bool CheckList(unsigned int _usID)
	{
		if (m_cusLoginList.count(_usID) != 0)
			return true;
		return false;
	}
public:
	static void InitializeBeforeStart()
	{
		CProcess::m_cpqEventQueue.clear();
		CProcess::m_cusLoginList.clear();
	}
public:
	static void PostEvent(unsigned int _usID, unsigned int _usOther, EVENT_TYPE _cOverEvent, EVENT_TYPE _cEvent, const std::chrono::high_resolution_clock::time_point& _TimePoint)
	{
		m_cpqEventQueue.push(Object_Event{ _TimePoint, _cEvent, _cOverEvent, _usID, _usOther });
	}
	static bool TryPopEvent(Object_Event& _ObjectEvent)
	{
		return m_cpqEventQueue.try_pop(_ObjectEvent);
	}
	static bool StateEventQueue()
	{
		return m_cpqEventQueue.empty();
	}
	static bool CheckEventStart(Object_Event& _ObjectEvent)
	{
		if (m_cpqEventQueue.try_pop(_ObjectEvent))
		{
			if (_ObjectEvent.wakeup_time > std::chrono::high_resolution_clock::now())
			{
				CProcess::PostEvent(_ObjectEvent.m_usID, _ObjectEvent.m_usOtherID, _ObjectEvent.m_OverType, _ObjectEvent.m_EventType, _ObjectEvent.wakeup_time);
				return false;
			}
			return true;
		}
		return false;
	}
};

