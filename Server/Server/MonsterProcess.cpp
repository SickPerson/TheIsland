#include "MonsterProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"
#include <DirectXMath.h>

CMonsterProcess::CMonsterProcess()
{
	if (!CProcess::m_pMonsterPool)
		CProcess::m_pMonsterPool = new class CMonsterpool();
	BindMonsterUpdate();
}


CMonsterProcess::~CMonsterProcess()
{
	if (m_pMonsterPool)
	{
		delete m_pMonsterPool;
		m_pMonsterPool = nullptr;
	}
}

void CMonsterProcess::AttackEvent(USHORT monster_id, USHORT target_id)
{
	if (CProcess::m_pMonsterPool->m_cumMonsterPool[monster_id]->GetState() == (UINT)ANIMAL_UPDATE_TYPE::DIE) return;

	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);

	InRangePlayer(login_list, range_list, monster_id);

	// 타켓이 없을때
	if (target_id == NO_TARGET) {
		m_pMonsterPool->m_cumMonsterPool[monster_id]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
		m_pMonsterPool->m_cumMonsterPool[monster_id]->SetTarget(NO_TARGET);
		PushEvent_Idle(monster_id, target_id);
	}
	else // 타켓이 있을때
	{
		float player_HP = m_pPlayerPool->m_cumPlayerPool[target_id]->GetHealth();
		Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[target_id]->GetLocalPos();
		float player_AfterHP = player_HP;

		Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetLocalPos();
		float monster_Damage = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetDamage();
		// 플레이어에게 데미지를 입힌다.
		player_AfterHP -= monster_Damage;
		
		// 플레이어 체력이 0이 되었을때
		if (player_AfterHP <= 0) 
		{
			// 플레이어 처리 (플레이어 사망)
			// 플레이어 사망 패킷 보내기
			CPacketMgr::GetInst()->Send_Death_Player_Packet(target_id);

			// ANIMAL IDLE 상태로
			m_pMonsterPool->m_cumMonsterPool[monster_id]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
			m_pMonsterPool->m_cumMonsterPool[monster_id]->SetTarget(NO_TARGET);
			PushEvent_Idle(monster_id, target_id);

			//
			for (auto& au : range_list) {
				bool isConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// Player Status 패킷 보내기
				// Animation - 플레이어가 맞을때
			}
		}
		else { // 플레이어 체력이 0이 안되었을 경우
			m_pPlayerPool->m_cumPlayerPool[target_id]->SetHealth(player_AfterHP);
			// 플레이어 패킷 보내기

			for (auto& au : range_list) {
				bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// 
			}
		}
	}
}

void CMonsterProcess::FollowEvent(USHORT uiID, USHORT target_id)
{
	if (m_pMonsterPool->m_cumMonsterPool[uiID]->GetState() == (UINT)ANIMAL_UPDATE_TYPE::DIE) return;

	concurrent_unordered_set<USHORT> before_loginList;
	concurrent_unordered_set<USHORT> after_loginList;
	concurrent_unordered_set<USHORT> before_rangeList;
	concurrent_unordered_set<USHORT> after_rangeList;

	USHORT monster_id = uiID + MAX_USER; // Player Login List에 넣을때는 MAX_USER를 + 해준다.
	
	CopyBeforeLoginList(before_loginList);
	InRangePlayer(before_loginList, before_rangeList, uiID);
	
	// 만약 타겟이 없거나 죽었다면
	if (target_id == NO_TARGET || (target_id != NO_TARGET && m_pPlayerPool->m_cumPlayerPool[target_id]->GetState() == (UINT)ANIMAL_UPDATE_TYPE::DIE))
	{
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetTarget(NO_TARGET);
		Update_Event ev;
		ev.m_Do_Object = uiID;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = NO_TARGET;
		ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
		ev.wakeup_time = high_resolution_clock::now() + 16ms;
		PushEventQueue(ev);
		return;
	}
	// 타켓이 존재한다면
	// Monster Follow 계산
	Vec3 target_pos = m_pPlayerPool->m_cumPlayerPool[target_id]->GetLocalPos();
	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[uiID]->GetLocalPos();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[uiID]->GetSpeed();

	Vec3 vDir = XMVector3Normalize(target_pos - monster_pos);
	vDir.y = 0.f;

	monster_pos = vDir * monster_speed * 0.02f;

	Vec3 vRot = m_pPlayerPool->m_cumPlayerPool[target_id]->GetLocalRot();

	m_pMonsterPool->m_cumMonsterPool[uiID]->SetLocalRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
	m_pMonsterPool->m_cumMonsterPool[uiID]->SetLocalPos(monster_pos);

	CopyBeforeLoginList(after_loginList);
	InRangePlayer(after_loginList, after_rangeList, uiID);

	for (auto& au : before_rangeList)
	{
		if (0 == after_rangeList.count(au)) // after에 없을때
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiID);
		}
		else
		{
			CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, uiID);
		}
	}

	if (m_pPlayerPool->m_cumPlayerPool[target_id]->GetState() == (UINT)ANIMAL_UPDATE_TYPE::DIE)
	{
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
		m_pMonsterPool->m_cumMonsterPool[uiID]->SetTarget(NO_TARGET);
		//
		Update_Event ev;
		ev.m_Do_Object = uiID;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = NO_TARGET;
		ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
		ev.wakeup_time = high_resolution_clock::now() + 1s;
		PushEventQueue(ev);
	}
	else
	{
		if (ObjectRangeCheck(monster_pos, target_pos, 100.f)) // 공격 범위
		{
			m_pMonsterPool->m_cumMonsterPool[uiID]->SetState((UINT)ANIMAL_UPDATE_TYPE::ATTACK);
			Update_Event ev;
			ev.m_Do_Object = uiID;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = target_id;
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::ATTACK;
			ev.wakeup_time = high_resolution_clock::now() + 4s;
			PushEventQueue(ev);
		}
		else if (ObjectRangeCheck(monster_pos, target_pos, MONSTER_BETWEEN_RANGE))
		{
			m_pMonsterPool->m_cumMonsterPool[uiID]->SetState((UINT)ANIMAL_UPDATE_TYPE::FOLLOW);
			Update_Event ev;
			ev.m_Do_Object = uiID;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = target_id;
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::FOLLOW;
			ev.wakeup_time = high_resolution_clock::now() + 16ms;
			PushEventQueue(ev);
		}
		else 
		{
			m_pMonsterPool->m_cumMonsterPool[uiID]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
			m_pMonsterPool->m_cumMonsterPool[uiID]->SetTarget(NO_TARGET);
			Update_Event ev;
			ev.m_Do_Object = uiID;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = NO_TARGET;
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
			ev.wakeup_time = high_resolution_clock::now() + 16ms;
			PushEventQueue(ev);
		}
	}
}

void CMonsterProcess::EvastionEvent(USHORT monster_id, USHORT target_id)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetLocalPos();
	Vec3 monster_dir = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetDir();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetSpeed();
	monster_pos += monster_dir * monster_speed * 0.02f;
	m_pMonsterPool->m_cumMonsterPool[monster_id]->SetLocalPos(monster_pos);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, monster_id);
	}
}

void CMonsterProcess::IdleEvent(USHORT monsterId)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monsterId);

	if (range_list.empty()) {
		//CProcess::m_pMonsterPool->m_cumMonsterPool[monsterId]->ResPawn();
		return;
	}

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetLocalPos();
	BEHAVIOR_TYPE animal_type = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetType();

	USHORT target_id = m_pMonsterPool->m_cumMonsterPool[monsterId]->GetTarget();
	
	for (auto& au : range_list) {
		bool isConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect)continue;
		//CPacketMgr::Send_Packet(au, &) // IDLE Animation 패킷 보내기
		Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[au]->GetLocalPos();
		if (ObjectRangeCheck(player_pos, monster_pos, MONSTER_BETWEEN_RANGE) && target_id == NO_TARGET)
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetTarget(au);
			target_id = au;
		}
	}

	// Animal의 Target이 없을 경우
	if (NO_TARGET == target_id)
	{
		m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
		Update_Event ev;
		ev.m_Do_Object = monsterId;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
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
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState((UINT)ANIMAL_UPDATE_TYPE::FOLLOW);
	
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::FOLLOW;
			ev.m_From_Object = target_id;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		else if (B_PASSIVE == animal_type) // 비선공
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
			
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
			ev.m_From_Object = NO_TARGET;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		else if (B_EVASION == animal_type) // 회피(도망)
		{
			m_pMonsterPool->m_cumMonsterPool[monsterId]->SetState((UINT)ANIMAL_UPDATE_TYPE::EVASION);
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::EVASION;
			ev.m_From_Object = target_id;
			ev.wakeup_time = high_resolution_clock::now() + 1s;
		}
		PushEventQueue(ev);
	}
}

void CMonsterProcess::DieEvent(USHORT monster_id)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);

	// 몬스터 상태 DIE로 변환
	m_pMonsterPool->m_cumMonsterPool[monster_id]->SetState((UINT)ANIMAL_UPDATE_TYPE::DIE);
	// 몬스터 리스폰 
	PushEvent_Respawn(monster_id, NO_TARGET);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, monster_id);
	}
}

void CMonsterProcess::RespawnEvent(USHORT monster_id)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	//CProcess::m_pMonsterPool->m_cumMonsterPool[monster_id]->ResPawn();

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);

	if (range_list.empty())
		return;

	CProcess::m_pMonsterPool->m_cumMonsterPool[monster_id]->SetWakeUp(true);
	USHORT Animal_ID = MAX_USER + monster_id;

	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[monster_id]->GetLocalPos();

	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_From_Object = NO_TARGET;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;

	CProcess::PushEventQueue(ev);

	for (auto& au : range_list) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		CPacketMgr::GetInst()->Send_Put_Npc_Packet(au, monster_id);
	}
}

void CMonsterProcess::HealEvent(USHORT monster_id)
{
	char monster_state = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetState();
	if (monster_state == (UINT)ANIMAL_UPDATE_TYPE::ATTACK || monster_state == (UINT)ANIMAL_UPDATE_TYPE::FOLLOW || monster_state == (UINT)ANIMAL_UPDATE_TYPE::FOLLOW) return;

	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	USHORT Animal_ID = monster_id + MAX_USER;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);
	float monster_hp = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetHealth();
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
	m_pMonsterPool->m_cumMonsterPool[monster_id]->SetHealth(monster_hp);

	// 몬스터 관련 넘겨줘야합니당
	if (range_list.empty())
	{
		return;
	}
		//m_pMonsterPool->m_cumMonsterPool[monster_id]->ResPawn();
	else
	{
		if (monster_hp != limit_hp)
		{
			Update_Event ev;
			ev.m_Do_Object = monster_id;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = NO_TARGET;
			ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::HEAL;
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

void CMonsterProcess::PushEvent_Attack(USHORT monster_id, USHORT target_id)
{
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::ATTACK;
	ev.wakeup_time = high_resolution_clock::now() + 3s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Follow(USHORT monster_id, USHORT target_id)
{
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::FOLLOW;
	ev.wakeup_time = high_resolution_clock::now() + 16ms;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Evastion(USHORT monster_id, USHORT target_id)
{
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::EVASION;
	ev.wakeup_time = high_resolution_clock::now() + 16ms;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Idle(USHORT monster_id, USHORT target_id)
{
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
	ev.wakeup_time = high_resolution_clock::now() + 16ms;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Die(USHORT monster_id, USHORT target_id)
{
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::DIE;
	ev.wakeup_time = high_resolution_clock::now() + 16ms;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Respawn(USHORT monster_id, USHORT target_id)
{
	UINT eType = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetType();
	Update_Event ev;
	ev.m_Do_Object = monster_id;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = target_id;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::RESPAWN;
	if (B_WARLIKE == eType)
		ev.wakeup_time = high_resolution_clock::now() + 60s;
	else if (B_PASSIVE == eType)
		ev.wakeup_time = high_resolution_clock::now() + 50s;
	else if (B_EVASION == eType)
		ev.wakeup_time = high_resolution_clock::now() + 40s;
	else
		return;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Heal(USHORT monster_id, USHORT target_id)
{
}

void CMonsterProcess::InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT monster_id)
{
	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[monster_id]->GetLocalPos();
	for (auto& au : cusLogin) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetLocalPos();
		if(ObjectRangeCheck(player_pos, monster_pos, PLAYER_BETWEEN_RANGE))
			cusList.insert(au);
	}
}
