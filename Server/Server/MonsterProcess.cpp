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

void CMonsterProcess::AttackEvent(USHORT Animal_Id, USHORT uiTarget)
{
	UINT Animal_State = CProcess::m_pMonsterPool->m_cumMonsterPool[Animal_Id]->GetState();
	if (Animal_State == (UINT)ANIMAL_STATE_TYPE::DIE || 
		Animal_State == (UINT)ANIMAL_STATE_TYPE::RESPAWN)
		return;

	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);

	InRangePlayer(login_list, range_list, Animal_Id);

	if (PlayerAndAnimal_CollisionSphere(uiTarget, Animal_Id, 0.2f))
	{
		float player_HP = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetHealth();
		Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetLocalPos();
		float player_AfterHP = player_HP;

		Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[Animal_Id]->GetLocalPos();
		float monster_Damage = m_pMonsterPool->m_cumMonsterPool[Animal_Id]->GetDamage();
		// 플레이어에게 데미지를 입힌다.
		player_AfterHP -= monster_Damage;

		// 플레이어 체력이 0이 되었을때
		if (player_AfterHP <= 0)
		{
			// 플레이어 처리 (플레이어 사망)
			// 플레이어 사망 패킷 보내기
			CPacketMgr::GetInst()->Send_Death_Player_Packet(uiTarget);
			// 플레이어 사망 했다는 애니메이션 보내기
			//CPacketMgr::GetInst()->Send_Animation_Player_Packet(uiTarget, )
			// ANIMAL IDLE 상태로
			PushEvent_Idle(Animal_Id);
			
			for (auto& au : range_list) {
				bool isConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// Player Status 패킷 보내기
				// Animation - 플레이어가 맞을때
				//CPacketMgr::GetInst()->Send_Animation_Player_Packet(uiTarget, )
			}
		}
		else { // 플레이어 체력이 0이 안되었을 경우
			m_pPlayerPool->m_cumPlayerPool[uiTarget]->SetHealth(player_AfterHP);
			// 플레이어 패킷 보내기

			for (auto& au : range_list) {
				bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
				if (!isConnect) continue;
				// 
			}
		}
	}
	else
	{
		PushEvent_Idle(Animal_Id);
	}
}

void CMonsterProcess::FollowEvent(USHORT AnimalId, USHORT uiTarget)
{
	UINT Animal_State = CProcess::m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetState();
	if (Animal_State == (UINT)ANIMAL_STATE_TYPE::RESPAWN)
		return;

	concurrent_unordered_set<USHORT> before_loginList;
	concurrent_unordered_set<USHORT> after_loginList;

	concurrent_unordered_set<USHORT> before_rangeList;
	concurrent_unordered_set<USHORT> after_rangeList;

	USHORT monster_id = AnimalId + MAX_USER; // Player Login List에 넣을때는 MAX_USER를 + 해준다.
	
	CopyBeforeLoginList(before_loginList);
	InRangePlayer(before_loginList, before_rangeList, AnimalId);
	
	// 만약 타겟이 없거나 죽었다면
	if (uiTarget == NO_TARGET || (uiTarget != NO_TARGET && Animal_State == (UINT)ANIMAL_STATE_TYPE::DIE))
	{
		PushEvent_Idle(AnimalId);
	}
	// 타켓이 존재한다면
	// Monster Follow 계산
	Vec3 target_pos = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetLocalPos();
	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetLocalPos();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetSpeed();

	Vec3 vDir = XMVector3Normalize(target_pos - monster_pos);
	vDir.y = 0.f;

	monster_pos = vDir * monster_speed * 0.02f;

	Vec3 vRot = m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetLocalRot();

	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetLocalRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetLocalPos(monster_pos);

	CopyBeforeLoginList(after_loginList);
	InRangePlayer(after_loginList, after_rangeList, AnimalId);

	for (auto& au : before_rangeList)
	{
		if (0 == after_rangeList.count(au)) // after에 없을때
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, AnimalId);
		}
		else
		{
			CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, AnimalId);
		}
	}

	if (m_pPlayerPool->m_cumPlayerPool[uiTarget]->GetState() == (UINT)ANIMAL_STATE_TYPE::DIE)
	{
		PushEvent_Idle(AnimalId);
	}
	else
	{
		if (ObjectRangeCheck(monster_pos, target_pos, 100.f)) // 공격 범위
		{
			PushEvent_Attack(AnimalId, uiTarget);
		}
		else if (ObjectRangeCheck(monster_pos, target_pos, MONSTER_BETWEEN_RANGE))
		{
			PushEvent_Follow(AnimalId, uiTarget);
		}
		else 
		{
			PushEvent_Idle(AnimalId);
		}
	}
}

void CMonsterProcess::EvastionEvent(USHORT uiMonster, USHORT uiTarget)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	Vec3 monster_pos = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetLocalPos();
	Vec3 monster_dir = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetDir();
	float monster_speed = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetSpeed();
	monster_pos += monster_dir * monster_speed * 0.02f;
	m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetLocalPos(monster_pos);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Pos_Npc_Packet(au, uiMonster);
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
		PushEvent_Idle(monsterId);
	}
	else if(target_id < MAX_USER)
	{
		Update_Event ev;
		ev.m_Do_Object = monsterId;
		ev.m_EventType = EV_MONSTER_UPDATE;
		if (B_WARLIKE == animal_type) // 선공
		{
			PushEvent_Follow(monsterId, target_id);
		}
		else if (B_PASSIVE == animal_type) // 비선공
		{
			PushEvent_Idle(monsterId);
		}
		else if (B_EVASION == animal_type) // 회피(도망)
		{
			PushEvent_Evastion(monsterId, target_id);
		}
	}
}

void CMonsterProcess::DieEvent(USHORT uiMonster)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	PushEvent_Respawn(uiMonster);

	for (auto& au : range_list)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiMonster);
	}
}

void CMonsterProcess::RespawnEvent(USHORT uiMonster)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	if (range_list.empty())
		return;

	CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetWakeUp(true);
	USHORT monster_id = MAX_USER + uiMonster;

	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetLocalPos();

	PushEvent_Idle(uiMonster);

	for (auto& au : range_list) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		CPacketMgr::GetInst()->Send_Put_Npc_Packet(au, uiMonster);
	}
}

void CMonsterProcess::HealEvent(USHORT uiMonster)
{
	char Animal_State = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetState();
	if (Animal_State == (UINT)ANIMAL_STATE_TYPE::ATTACK || 
		Animal_State == (UINT)ANIMAL_STATE_TYPE::FOLLOW)
		return;

	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	USHORT monster_id = uiMonster + MAX_USER;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, monster_id);
	float monster_hp = m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetHealth();
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
	m_pMonsterPool->m_cumMonsterPool[uiMonster]->SetHealth(monster_hp);

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

void CMonsterProcess::DamageEvent(USHORT AnimalId, USHORT playerId)
{
	float fHealth = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetHealth();
	char cType = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetType();

	if (fHealth > 0.f)
	{
		if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			Vec3 vMoveDir = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalDir(DIR_TYPE::FRONT);
			m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetDir(vMoveDir);
			Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
			Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetLocalPos();

			vMoveDir = XMVector3Normalize(vAnimalPos - vPlayerPos);
			vMoveDir.y = 0.f;
		}
		else if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(playerId);
		}
		else if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(playerId);

			Vec3 vMoveDir = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalDir(DIR_TYPE::FRONT);
			m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetDir(vMoveDir);
		}
	}
	else // 체력이 0 이하 일때
	{
		PushEvent_Die(AnimalId);
	}
}

void CMonsterProcess::PushEvent_Attack(USHORT AnmimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnmimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::ATTACK);
	m_pMonsterPool->m_cumMonsterPool[AnmimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnmimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::ATTACK;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Follow(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::FOLLOW);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::FOLLOW;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Evastion(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::EVASION);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::EVASION;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Idle(USHORT AnimalId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::IDLE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::IDLE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Die(USHORT AnimalId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::DIE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::DIE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Respawn(USHORT AnimalId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::RESPAWN);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::RESPAWN;
	ev.wakeup_time = high_resolution_clock::now() + 30s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Heal(USHORT AnimalId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::HEAL);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::HEAL;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CMonsterProcess::PushEvent_Damage(USHORT AnimalId, USHORT playerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState((UINT)ANIMAL_UPDATE_TYPE::DAMAGE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(playerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = playerId;
	ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::DAMAGE;
	ev.wakeup_time = high_resolution_clock::now();
	PushEventQueue(ev);
}

void CMonsterProcess::InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT uiMonster)
{
	Vec3 monster_pos = CProcess::m_pMonsterPool->m_cumMonsterPool[uiMonster]->GetLocalPos();
	for (auto& au : cusLogin) {
		bool isConnect = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetLocalPos();
		if(ObjectRangeCheck(player_pos, monster_pos, PLAYER_BETWEEN_RANGE))
			cusList.insert(au);
	}
}
