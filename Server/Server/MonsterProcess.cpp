#include "MonsterProcess.h"
#include "PacketMgr.h"


CMonsterProcess::CMonsterProcess()
{
	if (!CProcess::m_pMonsterPool)
		CProcess::m_pMonsterPool = new class CMonsterpool();
}


CMonsterProcess::~CMonsterProcess()
{
	if (m_pMonsterPool)
	{
		delete m_pMonsterPool;
		m_pMonsterPool = nullptr;
	}
}

bool CMonsterProcess::ObjectRangeCheck(Vec3 & player, Vec3 & other, float fDistance)
{
	if (fDistance < abs(player.x - other.x)) return false;
	if (fDistance < abs(player.z - other.z)) return false;
	return true;
}

void CMonsterProcess::AttackEvent(unsigned int uiMonster, unsigned int uiTarget)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CopyBeforeLoginList(login_list);

	InRangePlayer(login_list, range_list, uiMonster);

	float player_HP = CProcess::m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetPlayerStatus().fHP;
	Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetPos();

	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetPos();

	if (uiTarget == NO_TARGET) {
		CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetState(OBJ_STATE_IDLE);
		Update_Event ev;
		ev.m_EventType = EVENT_TYPE::EV_MONSTER_UPDATE;
		ev.m_ObjState = OBJ_STATE_TYPE::OBJ_STATE_IDLE;
		ev.m_Do_Object = uiMonster;
		ev.m_From_Object = NO_TARGET;
		CProcess::PushEventQueue(ev);
		return;
	}

	if (range_list.empty())
		m_pMonsterPool->m_cumMonsterPool[uiMonster]->ResPawn();
	else
	{
		float monster_Damage = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetDamage();

		float player_AfterHP = player_HP;

		player_AfterHP -= monster_Damage;
		
		// 플레이어 체력이 0이 되었을때
		if (player_AfterHP <= 0) {
			// 몬스터
			m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetState(OBJ_STATE_IDLE);
			m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetTarget(NO_TARGET);


			for (auto& au : range_list) {
				bool isConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// 
			}
		}
		else { // 플레이어 체력이 0이 안되었을때

			CProcess::m_pPlayerPool->m_cumPlayerPool[uiTarget]->SetHP(player_AfterHP);

			for (auto& au : range_list) {
				bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// 
			}
		}
	}
}

void CMonsterProcess::FollowEvent(unsigned int uiID, unsigned int uiTarget)
{
}

void CMonsterProcess::IdleEvent(unsigned int uiID)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiID);

	if (range_list.empty()) {
		CProcess::m_pMonsterPool->m_cumMonsterPool[uiID]->ResPawn();
		return;
	}

	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiID]->GetPos();

	unsigned int target_id = CProcess::m_pMonsterPool->m_cumMonsterPool[uiID]->GetTarget();
	for (auto& au : range_list) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect)continue;
		//CPacketMgr::Send_Packet(au, &) // IDLE Animation 패킷 보내기
		Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPos();
		if (ObjectRangeCheck(player_pos, monster_pos, 1000.f) && target_id == NO_TARGET)
		{
			CProcess::m_pMonsterPool->m_cumMonsterPool[uiID]->SetTarget(au);
			target_id = au;
		}
	}

	if (target_id != NO_TARGET) {
		CProcess::m_pMonsterPool->m_cumMonsterPool[uiID]->SetState(OBJ_STATE_FOLLOW);
		Update_Event ev;
		// TARGET이 있을 경우 설정 필요
		CProcess::PushEventQueue(ev);
	}
	else {
		Update_Event ev;
		// NO TARGET일 경우 설정 필요
		CProcess::PushEventQueue(ev);
	}
}

void CMonsterProcess::RespawnEvent(unsigned int uiMonster)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->ResPawn();

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	if (range_list.empty())
		return;

	CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetWakeUp(true);
	unsigned int monster_id = MAX_USER + uiMonster;

	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetPos();

	Update_Event ev;
	ev.m_Do_Object = uiMonster;
	ev.m_From_Object = NO_TARGET;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_ObjState = OBJ_STATE_IDLE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;

	CProcess::PushEventQueue(ev);

	for (auto& au : range_list) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		if (!CProcess::m_pPlayerPool->m_cumPlayerPool[au]->ExistList(monster_id)) {
			m_pPlayerPool->m_cumPlayerPool[au]->InsertList(monster_id);
		}
	}
}

void CMonsterProcess::InRangePlayer(concurrent_unordered_set<unsigned int>& cusLogin, concurrent_unordered_set<unsigned int>& cusList, unsigned int uiMonster)
{
	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetPos();
	for (auto& au : cusLogin) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPos();
		if(ObjectRangeCheck(player_pos, monster_pos, 1000.f))
			cusList.insert(au);
	}
		
}
