#include "Process.h"
#include "PacketMgr.h"

CPlayerpool* CProcess::m_pPlayerPool = nullptr;
concurrent_unordered_set<unsigned int> CProcess::m_cusLoginList;
concurrent_priority_queue<Object_Event>	CProcess::m_cpqEventQueue;

CProcess::CProcess()
{
}


CProcess::~CProcess()
{
	InitProcessData();
}

void CProcess::InitProcessData()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
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
