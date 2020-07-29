#include "Process.h"
#include "PacketMgr.h"

CPlayerpool* CProcess::m_pPlayerPool = nullptr;
<<<<<<< HEAD
CMonsterpool*	CProcess::m_pMonsterPool = nullptr;
concurrent_unordered_set<unsigned int> CProcess::m_cusLoginList;
concurrent_priority_queue<Update_Event>	CProcess::m_cpqEventQueue;
=======
concurrent_unordered_set<unsigned int> CProcess::m_cusLoginList;
concurrent_priority_queue<Object_Event>	CProcess::m_cpqEventQueue;
>>>>>>> master

CProcess::CProcess()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
}

CProcess::~CProcess()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
<<<<<<< HEAD
}
=======
}

unsigned int CProcess::GetNewID()
{
	for (int i = 0; i < MAX_USER; ++i) {
		auto id = m_cusLoginList.find(i);
		if (id == m_cusLoginList.end())
			return i;
		else
			continue;
	}
	return -1;
}
>>>>>>> master
