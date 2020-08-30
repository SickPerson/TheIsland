#include "MonsterProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"
#include <DirectXMath.h>

CMonsterProcess::CMonsterProcess()
{
	BindMonsterUpdate();
}


CMonsterProcess::~CMonsterProcess()
{
}

void CMonsterProcess::AttackEvent(USHORT Animal_Id, USHORT uiTarget)
{
	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[Animal_Id]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& player : loginList) {
		if (m_pObjectPool->m_cumPlayerPool[player]->ExistList(Animal_Id))
			CPacketMgr::Send_Animation_Packet(player, Animal_Id, (UINT)ANIMAL_ANIMATION_TYPE::ATTACK);
	}

	float fTarget_CurrHp = m_pObjectPool->m_cumPlayerPool[uiTarget]->GetHealth();
	float fTarget_AfterHp = fTarget_CurrHp;

	float fAnimalDamage = m_pObjectPool->m_cumAnimalPool[Animal_Id]->GetDamage();

	fTarget_AfterHp -= fAnimalDamage;

	if (fTarget_AfterHp <= 0.f)
	{
		// - Player
		fTarget_AfterHp = 0.f;
		m_pObjectPool->m_cumPlayerPool[uiTarget]->SetHealth(fTarget_AfterHp);

		// - Animal

		concurrent_unordered_set<USHORT>	loginList;
		CopyBeforeLoginList(loginList);
		concurrent_unordered_set<USHORT>	rangeList;
		InRangePlayer(loginList, rangeList, Animal_Id);

		if (rangeList.empty())
		{
			m_pObjectPool->m_cumAnimalPool[Animal_Id]->SetWakeUp(false);
			m_pObjectPool->m_cumAnimalPool[Animal_Id]->SetTarget(NO_TARGET);
			return;
		}

		USHORT NewTarget = MAX_USER;
		float fCurrDistance = 9999;
		for (auto& player : rangeList)
		{
			float d = Distance(player, Animal_Id);
			if (fCurrDistance >= d) {
				NewTarget = player;
				fCurrDistance = d;
			}
			CPacketMgr::Send_Animation_Packet(player, Animal_Id, (UINT)ANIMAL_ANIMATION_TYPE::ATTACK);
		}

		m_pObjectPool->m_cumAnimalPool[Animal_Id]->SetTarget(NewTarget);

		if (PlayerAndAnimal_CollisionSphere(NewTarget, Animal_Id, 0.2f))
		{
			PushEvent_Animal_Attack(Animal_Id, NewTarget);
		}
		else
		{
			PushEvent_Animal_Follow(Animal_Id, NewTarget);
		}
	}
	else
	{
		// - Player
		m_pObjectPool->m_cumPlayerPool[uiTarget]->SetHealth(fTarget_AfterHp);
		CPacketMgr::Send_Status_Player_Packet(uiTarget, uiTarget);

		// - Animal
		if (PlayerAndAnimal_CollisionSphere(uiTarget, Animal_Id, 0.2f))
		{
			PushEvent_Animal_Attack(Animal_Id, uiTarget);
		}
		else
		{
			PushEvent_Animal_Follow(Animal_Id, uiTarget);
		}
	}
}

void CMonsterProcess::FollowEvent(USHORT AnimalId, USHORT uiTarget)
{
	bool bWakeUp = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[AnimalId]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetLocalPos();
	Vec3 vTargetPos = m_pObjectPool->m_cumPlayerPool[uiTarget]->GetLocalPos();
	float fAnimalSpeed = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetSpeed();
	Vec3 vTargetRot = m_pObjectPool->m_cumPlayerPool[uiTarget]->GetLocalRot();

	Vec3 vDir = XMVector3Normalize(vTargetPos - vAnimalPos);
	vDir.y = 0.f;
	vAnimalPos += vDir * fAnimalSpeed * 0.05f;


	m_pObjectPool->m_cumAnimalPool[AnimalId]->SetLocalRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));	
	m_pObjectPool->m_cumAnimalPool[AnimalId]->SetLocalPos(vAnimalPos);

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);
	concurrent_unordered_set<USHORT> rangeList;
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetWakeUp(false);
		return;
	}

	for (auto& au : rangeList)
	{
		CPacketMgr::Send_Pos_Packet(au, AnimalId);
		CPacketMgr::Send_Animation_Packet(au, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::WALK);
	}

	PushEvent_Animal_Behavior(AnimalId, uiTarget);
}

void CMonsterProcess::EvastionEvent(USHORT AnimalId, USHORT uiTarget)
{
	bool bWakeUp = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[AnimalId]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetLocalPos();
	Vec3 vTargetPos = m_pObjectPool->m_cumPlayerPool[uiTarget]->GetLocalPos();

	float fAnimalSpeed = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetSpeed();
	Vec3 vDir = XMVector3Normalize(vTargetPos - vAnimalPos);
	vDir.y = 0.f;
	vAnimalPos += - vDir * fAnimalSpeed * 0.05f;
	m_pObjectPool->m_cumAnimalPool[AnimalId]->SetLocalRot(Vec3(0.f, atan2( - vDir.x, - vDir.z) + 3.141592f, 0.f));
	m_pObjectPool->m_cumAnimalPool[AnimalId]->SetLocalPos(vAnimalPos);

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);
	concurrent_unordered_set<USHORT> rangeList;
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetWakeUp(false);
		return;
	}

	for (auto& au : rangeList)
	{
		CPacketMgr::Send_Pos_Packet(au, AnimalId);
		CPacketMgr::Send_Animation_Packet(au, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::RUN);
	}

	PushEvent_Animal_Behavior(AnimalId, uiTarget);
}

void CMonsterProcess::IdleEvent(USHORT AnimalId)
{	
	bool bWakeUp = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[AnimalId]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, AnimalId);

	if (range_list.empty()) {
		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetWakeUp(false);
		return;
	}
	else // 몬스터 범위 내에 있을 경우 타겟을 찾는다.
	{
		for (auto& au : range_list)
		{
			bool isConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
			if (!isConnect)continue;
			m_pObjectPool->m_cumAnimalPool[AnimalId]->SetTarget(au);
		}
	}

	USHORT target_id = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetTarget();
	BEHAVIOR_TYPE eType = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetType();
	// 만약 타겟이 범위내에 있다면?
	switch (eType) 
	{
	case BEHAVIOR_TYPE::B_EVASION:
	{
		PushEvent_Animal_Evastion(AnimalId, target_id);
		break;
	}
	case BEHAVIOR_TYPE::B_PASSIVE:
	{
		PushEvent_Animal_Idle(AnimalId, target_id);
		break;
	}	
	case BEHAVIOR_TYPE::B_WARLIKE:
	{
		if (PlayerAndAnimal_CollisionSphere(target_id, AnimalId, 0.2f))
		{
			PushEvent_Animal_Attack(AnimalId, target_id);
		}
		else
		{
			PushEvent_Animal_Follow(AnimalId, target_id);
		}
		break;
	}
	}
}

void CMonsterProcess::DieEvent(USHORT uiMonster)
{
	concurrent_unordered_set<USHORT> login_list;
	//concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	//InRangePlayer(login_list, range_list, uiMonster);

	//PushEvent_Respawn(uiMonster);

	/*for (auto& au : range_list)
	{
		bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::GetInst()->Send_Remove_Npc_Packet(au, uiMonster);
	}*/
}

void CMonsterProcess::RespawnEvent(USHORT uiMonster)
{
	concurrent_unordered_set<USHORT> login_list;
	concurrent_unordered_set<USHORT> range_list;

	CopyBeforeLoginList(login_list);
	InRangePlayer(login_list, range_list, uiMonster);

	if (range_list.empty())
		return;

	CProcess::m_pObjectPool->m_cumAnimalPool[uiMonster]->SetWakeUp(true);
	USHORT monster_id = MAX_USER + uiMonster;

	Vec3 monster_pos = CProcess::m_pObjectPool->m_cumAnimalPool[uiMonster]->GetLocalPos();

	PushEvent_Animal_Idle(uiMonster, NO_TARGET);

	for (auto& au : range_list) {
		bool isConnect = CProcess::m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		CPacketMgr::GetInst()->Send_Pos_Packet(au, uiMonster);
	}
}

void CMonsterProcess::DamageEvent(USHORT AnimalId, USHORT playerId)
{
	float fHealth = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetHealth();
	char cType = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetType();


	if (fHealth > 0.f)
	{
		if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			Vec3 vMoveDir = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalDir(DIR_TYPE::FRONT);
			m_pObjectPool->m_cumAnimalPool[AnimalId]->SetDir(vMoveDir);
			Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
			Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetLocalPos();

			vMoveDir = XMVector3Normalize(vAnimalPos - vPlayerPos);
			vMoveDir.y = 0.f;
		}
		else if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			m_pObjectPool->m_cumAnimalPool[AnimalId]->SetTarget(playerId);
		}
		else if (BEHAVIOR_TYPE::B_EVASION == cType)
		{
			m_pObjectPool->m_cumAnimalPool[AnimalId]->SetTarget(playerId);

			Vec3 vMoveDir = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalDir(DIR_TYPE::FRONT);
			m_pObjectPool->m_cumAnimalPool[AnimalId]->SetDir(vMoveDir);
		}
	}
	else // 체력이 0 이하 일때
	{
		PushEvent_Animal_Die(AnimalId, NO_TARGET);
	}
}

void CMonsterProcess::InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT uiMonster)
{
	Vec3 monster_pos = CProcess::m_pObjectPool->m_cumAnimalPool[uiMonster]->GetLocalPos();
	for (auto& au : cusLogin) {
		bool isConnect = CProcess::m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!isConnect) continue;
		Vec3 player_pos = CProcess::m_pObjectPool->m_cumPlayerPool[au]->GetLocalPos();
		if(ObjectRangeCheck(player_pos, monster_pos, PLAYER_VIEW_RANGE))
			cusList.insert(au);
	}
}

float CMonsterProcess::Distance(USHORT first, USHORT second)
{
	Vec3 vFirst = m_pObjectPool->m_cumPlayerPool[first]->GetLocalPos();
	Vec3 vSecond = m_pObjectPool->m_cumAnimalPool[second]->GetLocalPos();

	float fDistance = sqrtf(powf(vSecond.x - vFirst.x, 2) + powf(vSecond.z - vSecond.z, 2));

	return fDistance;
}
