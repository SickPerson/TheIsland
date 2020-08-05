#include "Process.h"
#include "PacketMgr.h"

CPlayerpool* CProcess::m_pPlayerPool = nullptr;
CMonsterpool*	CProcess::m_pMonsterPool = nullptr;
concurrent_unordered_set<unsigned short> CProcess::m_cusLoginList;
concurrent_priority_queue<Update_Event>	CProcess::m_cpqEventQueue;

CProcess::CProcess()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
}

CProcess::~CProcess()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
}

bool CProcess::ObjectRangeCheck(Vec3 & vFirst, Vec3 & vSecond, float fDistance)
{
	if (fDistance > sqrtf(pow(vSecond.x - vFirst.x, 2) + pow(vSecond.z - vFirst.z, 2)))
		return true;
	return false;
}