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
