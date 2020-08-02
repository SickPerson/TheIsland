#include "MonsterProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"
#include <DirectXMath.h>

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
	if (m_pMonsterPool->m_cumMonsterPool[uiID]->GetState() == OBJ_STATE_DIE) return;

	concurrent_unordered_set<unsigned int> before_loginList;
	concurrent_unordered_set<unsigned int> after_loginList;
	concurrent_unordered_set<unsigned int> before_rangeList;
	concurrent_unordered_set<unsigned int> after_rangeList;

	unsigned int monster_id = uiID + MAX_USER; // Player Login List에 넣을때는 MAX_USER를 + 해준다.
	
	CopyBeforeLoginList(before_loginList);
	InRangePlayer(before_loginList, before_rangeList, uiID);
	
	if (uiTarget == NO_TARGET || (uiTarget != NO_TARGET && m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetState() == OBJ_STATE_DIE))
	{
		Update_Event ev;
		ev.m_Do_Object = uiID;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = NO_TARGET;
		ev.m_ObjState = OBJ_STATE_IDLE;
		ev.wakeup_time = high_resolution_clock::now() + 1s;
		PushEventQueue(ev);
		return;
	}
	
	// Monster Follow 계산
	Vec3 target_pos = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetPos();

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[uiID]->GetPos();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[uiID]->GetSpeed();

	Vec3 vDir = XMVector3Normalize(target_pos - monster_pos);
	vDir.y = 0.f;

	monster_pos = vDir * monster_speed * CTimerMgr::GetInst()->GetDeltaTime();

	Vec3 vRot = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetRot();

	m_pMonsterPool->m_cumMonsterPool[uiID]->SetRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
	m_pMonsterPool->m_cumMonsterPool[uiID]->SetPos(monster_pos);

	for (auto& au : before_rangeList)
	{
		if (false == m_pPlayerPool->m_cumPlayerPool[au]->GetConnect()) return;
		CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, uiID);
	}

	CopyBeforeLoginList(after_loginList);
	InRangePlayer(after_loginList, after_rangeList, uiID);

	for (auto& au : before_rangeList)
	{
		if (0 == after_rangeList.count(au)) // after에 없을때
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiID + MAX_USER);
		}
	}

	if (m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetState() == OBJ_STATE_DIE)
	{
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetState(OBJ_STATE_IDLE);
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetTarget(NO_TARGET);
		//
		Update_Event ev;
		ev.m_Do_Object = uiID;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = NO_TARGET;
		ev.m_ObjState = OBJ_STATE_IDLE;
		ev.wakeup_time = high_resolution_clock::now() + 1s;
		PushEventQueue(ev);
	}
	else
	{
		for (auto& au : after_rangeList)
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (0 == before_rangeList.count(au)) // 없을때
			{
				CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiID);
			}
		}

		if (ObjectRangeCheck(monster_pos, target_pos, 500.f)) // 공격 범위
		{
			m_pMonsterPool->m_cumMonsterPool[uiID]->SetState(OBJ_STATE_ATTACK);
			Update_Event ev;
			ev.m_Do_Object = uiID;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = uiTarget;
			ev.m_ObjState = OBJ_STATE_ATTACK;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
			PushEventQueue(ev);
		}
		else 
		{
			Update_Event ev;
			ev.m_Do_Object = uiID;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = uiTarget;
			ev.m_ObjState = OBJ_STATE_FOLLOW;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
			PushEventQueue(ev);
		}
	}
}

void CMonsterProcess::EvastionEvent(unsigned int uiMonster, unsigned int uiTarget)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetPos();
	Vec3 monster_dir = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetDir();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetSpeed();
	monster_pos += monster_dir * monster_speed * CTimerMgr::GetInst()->GetDeltaTime();
	m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetPos(monster_pos);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, uiMonster);
	}
}

void CMonsterProcess::IdleEvent(unsigned int monsterId)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monsterId);

	if (range_list.empty()) {
		//CProcess::m_pMonsterPool->m_cumMonsterPool[monsterId]->ResPawn();
		return;
	}

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetPos();
	BEHAVIOR_TYPE animal_type = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetType();

	unsigned int target_id = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetTarget();
	
	for (auto& au : range_list) {
		bool isConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect)continue;
		//CPacketMgr::Send_Packet(au, &) // IDLE Animation 패킷 보내기
		Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[au]->GetPos();
		if (ObjectRangeCheck(player_pos, monster_pos, MONSTER_BETWEEN_RANGE) && target_id == NO_TARGET)
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetTarget(au);
			target_id = au;
		}
	}

	// Animal의 Target이 없을 경우
	if (NO_TARGET == target_id)
	{
		m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState(OBJ_STATE_IDLE);
		Update_Event ev;
		ev.m_Do_Object = monsterId;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_ObjState = OBJ_STATE_IDLE;
		ev.m_From_Object = NO_TARGET;
		ev.wakeup_time = high_resolution_clock::now() + 10s;
		PushEventQueue(ev);
	}
	else if(target_id < MAX_USER)
	{
		Update_Event ev;
		ev.m_Do_Object = monsterId;
		ev.m_EventType = EV_MONSTER_UPDATE;
		if (B_WARLIKE == animal_type) // 선공
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState(OBJ_STATE_FOLLOW);
	
			ev.m_ObjState = OBJ_STATE_FOLLOW;
			ev.m_From_Object = target_id;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		else if (B_PASSIVE == animal_type) // 비선공
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState(OBJ_STATE_IDLE);
			
			ev.m_ObjState = OBJ_STATE_IDLE;
			ev.m_From_Object = NO_TARGET;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		else if (B_EVASION == animal_type) // 회피(도망)
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState(OBJ_STATE_EVASION);
			ev.m_ObjState = OBJ_STATE_EVASION;
			ev.m_From_Object = target_id;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		PushEventQueue(ev);
	}
}

void CMonsterProcess::DieEvent(unsigned int uiMonster)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	Update_Event ev;
	ev.m_Do_Object = uiMonster;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = OBJ_STATE_RESPAWN;
	ev.wakeup_time = high_resolution_clock::now() + 30s;
	PushEventQueue(ev);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiMonster);
	}
}

void CMonsterProcess::RespawnEvent(unsigned int uiMonster)
{
	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	//CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->ResPawn();

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
		CPacketMgr::GetInst()->Send_Put_Npc_Packet(au, uiMonster);
	}
}

void CMonsterProcess::HealEvent(unsigned int uiMonster)
{
	char monster_state = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetState();
	if (monster_state == OBJ_STATE_ATTACK || monster_state == OBJ_STATE_FOLLOW || monster_state == OBJ_STATE_FOLLOW) return;

	concurrent_unordered_set<unsigned int> login_list;
	concurrent_unordered_set<unsigned int> range_list;

	unsigned int monster_id = uiMonster + MAX_USER;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);
	float monster_hp = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetHP();
	float limit_hp = 100.f;

	if (monster_hp < limit_hp)
	{
		float percent_hp = limit_hp * 0.01f;
		monster_hp += percent_hp;
	}
	else if (monster_hp >= limit_hp)
	{
		monster_hp = limit_hp;
	}
	m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetHP(monster_hp);

	// 몬스터 관련 넘겨줘야합니당
	if (range_list.empty())
	{
		return;
	}
		//m_pMonsterPool->m_cumMonsterPool[uiMonster]->ResPawn();
	else
	{
		if (monster_hp != limit_hp)
		{
			Update_Event ev;
			ev.m_Do_Object = uiMonster;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = NO_TARGET;
			ev.m_ObjState = OBJ_STATE_HEAL;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
			PushEventQueue(ev);
		}
		for (auto& au : range_list)
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			// Status 패킷 보내기
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
		if(ObjectRangeCheck(player_pos, monster_pos, MONSTER_BETWEEN_RANGE))
			cusList.insert(au);
	}
}
