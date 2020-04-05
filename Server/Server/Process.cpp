#include "Process.h"
#include "PacketMgr.h"

CPlayerpool* m_pPlayerPool = nullptr;
concurrent_unordered_set<unsigned short> m_cusLoginList;
concurrent_priority_queue<Object_Event>	m_cpqEventQueue;
function<void(char, unsigned short)>	m_fpAttackEvent;

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
