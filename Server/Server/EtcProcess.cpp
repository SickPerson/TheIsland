#include "EtcProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"


CEtcProcess::CEtcProcess()
{
	BindEtcUpdate();
	PushEvent_Etc_Weather();
	PushEvent_Etc_Time();
}


CEtcProcess::~CEtcProcess()
{
	
}

void CEtcProcess::WeatherEvent()
{
	bool bRain = rand() % 2;

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	for (auto& user : loginList) {
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[user]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Weather_Packet(user, bRain);
	}

	PushEvent_Etc_Weather();
}

void CEtcProcess::TimerEvent()
{
	float fTime = CTimerMgr::GetInst()->GetTotalTime();

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	for (auto& user : loginList)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[user]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Time_Packet(user, fTime);
	}
	PushEvent_Etc_Time();
}
