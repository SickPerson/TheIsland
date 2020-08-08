#include "Process.h"
#include "PacketMgr.h"

CPlayerpool* CProcess::m_pPlayerPool = nullptr;
CMonsterpool*	CProcess::m_pMonsterPool = nullptr;
CNaturalpool*	CProcess::m_pNaturalPool = nullptr;
CHousingpool*	CProcess::m_pHousingPool = nullptr;
concurrent_unordered_set<USHORT> CProcess::m_cusLoginList;
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

void CProcess::Weather_Event()
{
	for (auto& au : m_cusLoginList)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		bool bRain = rand() % 2;
		CPacketMgr::GetInst()->Send_Weather_Packet(au, bRain);
	}
}
