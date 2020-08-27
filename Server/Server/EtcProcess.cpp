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

void CEtcProcess::Player_Collision_Event()
{
	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	for (auto& player : loginList) {
		bool bConnect = m_pObjectPool->m_cumPlayerPool[player]->GetConnect();
		if (!bConnect) continue;

		// - Animal Collision
		for (auto& animal : m_pObjectPool->m_cumAnimalPool) {
			char eState = animal.second->GetState();
			if (eState == OBJ_STATE_TYPE::OST_DIE) continue;
			if (PlayerAndAnimal_CollisionSphere(player, animal.first, 0.2f)) {
				Vec3 vPos = m_pObjectPool->m_cumPlayerPool[player]->GetLocalPos();
				Vec3 vOtherPos = m_pObjectPool->m_cumAnimalPool[animal.first]->GetLocalPos();

				float fSpeed = m_pObjectPool->m_cumPlayerPool[player]->GetSpeed();
				Vec3 vDir = XMVector3Normalize(vPos - vOtherPos);
				vDir.y = 0.f;
				vPos += vDir * fSpeed * 0.05f * 5.f;

				m_pObjectPool->m_cumPlayerPool[player]->SetLocalPos(vPos);
				CPacketMgr::Send_Pos_Player_Packet(player, player);
			}
		}
		// - Natural Collision
		for (auto& natural : m_pObjectPool->m_cumNaturalPool) {
			bool bDestroy = natural.second->GetDestroy();
			if (bDestroy) continue;
			char eType = natural.second->GetType();
			if (eType == NATURAL_TYPE::N_BUSH) continue;
			if (PlayerAndNatural_CollisionSphere(player, natural.first, 0.2f)){
				Vec3 vPos = m_pObjectPool->m_cumPlayerPool[player]->GetLocalPos();
				Vec3 vOtherPos = natural.second->GetLocalPos();

				float fSpeed = m_pObjectPool->m_cumPlayerPool[player]->GetSpeed();
				Vec3 vDir = XMVector3Normalize(vPos - vOtherPos);
				vDir.y = 0.f;
				vPos += vDir * fSpeed * 0.05f * 5.f;

				m_pObjectPool->m_cumPlayerPool[player]->SetLocalPos(vPos);
				CPacketMgr::Send_Pos_Player_Packet(player, player);
			}
		}
		// - House Collision
		/*for (auto& house : m_pHousingPool->m_cumHousingPool) {
			bool bInstall = house.second->GetInstall();
			if (!bInstall) continue;
			char eType = house.second->GetType();
			if (PlayerAndHouse_Collision(player, house.first, eType)) {

			}
		}*/
	}
}

void CEtcProcess::Animal_Collision_Event()
{
	for (auto& animal : m_pObjectPool->m_cumAnimalPool) {
		
		// - Player Collision
		concurrent_unordered_set<USHORT> loginList;
		for (auto& player : loginList) {
			bool bConnect = m_pObjectPool->m_cumPlayerPool[player]->GetConnect();
			if (!bConnect) continue;
		}
		// - Natural Collision
		// - House Collision
	}
}

void CEtcProcess::WeatherEvent()
{
	cout << "Weather" << endl;
	bool bRain = rand() % 2;

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	for (auto& user : loginList) {
		bool bConnect = m_pObjectPool->m_cumPlayerPool[user]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Weather_Packet(user, bRain);
	}

	PushEvent_Etc_Weather();
}

void CEtcProcess::TimerEvent()
{
	cout << "Timer" << endl;
	float fTime = CTimerMgr::GetInst()->GetTotalTime();

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	for (auto& user : loginList)
	{
		bool bConnect = m_pObjectPool->m_cumPlayerPool[user]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Time_Packet(user, fTime);
	}
	PushEvent_Etc_Time();
}
