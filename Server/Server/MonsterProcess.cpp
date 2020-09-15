#include "MonsterProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"
#include <DirectXMath.h>


random_device	rd;

uniform_real_distribution<float>	urd(0.0, 1.0);

CMonsterProcess::CMonsterProcess()
{
	BindMonsterUpdate();
}


CMonsterProcess::~CMonsterProcess()
{
}

void CMonsterProcess::BehaviorEvent(USHORT AnimalId, USHORT usTarget)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[AnimalId];
	auto& Target = m_pObjectPool->m_cumPlayerPool[usTarget];

	// ========================== 예외 처리 ==========================
	char Animal_State = Animal->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;
	concurrent_unordered_set<USHORT> rangeList;
	CopyBeforeLoginList(loginList);
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		Animal->SetTarget(NO_TARGET);
		return;
	}
	// ==============================================================

	Vec3 vPos1 = Animal->GetLocalPos();
	Vec3 vPos2 = Target->GetLocalPos();

	if (ObjectRangeCheck(vPos1, vPos2, 2000.f))
	{
		UINT uiType = Animal->GetType();

		if (uiType == (UINT)BEHAVIOR_TYPE::B_WARLIKE)
		{
			if (CollisionSphere(Animal, Target, 0.3f))
			{
				Animal->SetTarget(usTarget);
				Update_Event ev;
				ev.m_Do_Object = AnimalId;
				ev.m_EventType = EV_MONSTER_UPDATE;
				ev.m_From_Object = usTarget;
				ev.m_eObjUpdate = AUT_ATTACK;
				ev.wakeup_time = high_resolution_clock::now() + milliseconds(500);
				PushEventQueue(ev);
			}
			else {
				Animal->SetTarget(usTarget);
				Update_Event ev;
				ev.m_Do_Object = AnimalId;
				ev.m_EventType = EV_MONSTER_UPDATE;
				ev.m_From_Object = usTarget;
				ev.m_eObjUpdate = AUT_FOLLOW;
				ev.wakeup_time = high_resolution_clock::now() + milliseconds(300);
				PushEventQueue(ev);
			}
		}
		else if(uiType == (UINT)BEHAVIOR_TYPE::B_PASSIVE){
			Animal->SetTarget(NO_TARGET);
			Update_Event ev;
			ev.m_Do_Object = AnimalId;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = usTarget;
			ev.m_eObjUpdate = AUT_IDLE;
			ev.wakeup_time = high_resolution_clock::now() + milliseconds(300);
			PushEventQueue(ev);
		}
		else if (uiType == (UINT)BEHAVIOR_TYPE::B_EVASION) {
			Animal->SetTarget(usTarget);
			Update_Event ev;
			ev.m_Do_Object = AnimalId;
			ev.m_EventType = EV_MONSTER_UPDATE;
			ev.m_From_Object = usTarget;
			ev.m_eObjUpdate = AUT_EVASION;
			ev.wakeup_time = high_resolution_clock::now() + milliseconds(300);
			PushEventQueue(ev);
		}
	}
	else {
		Animal->SetTarget(NO_TARGET);
		Update_Event ev;
		ev.m_Do_Object = AnimalId;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = usTarget;
		ev.m_eObjUpdate = AUT_IDLE;
		ev.wakeup_time = high_resolution_clock::now() + milliseconds(300);
		PushEventQueue(ev);
	}
}

void CMonsterProcess::AttackEvent(USHORT Animal_Id, USHORT usTarget)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[Animal_Id];
	auto& Target = m_pObjectPool->m_cumPlayerPool[usTarget];

	// ========================== 예외 처리 ==========================
	char Animal_State = Animal->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;
	concurrent_unordered_set<USHORT> rangeList;
	CopyBeforeLoginList(loginList);
	InRangePlayer(loginList, rangeList, Animal_Id);

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		Animal->SetTarget(NO_TARGET);
		return;
	}
	// ==============================================================

	if (true == CollisionSphere(Animal, Target, 0.3f)) {
		for (auto& player : rangeList) {
			if (m_pObjectPool->m_cumPlayerPool[player]->ExistList(Animal_Id)) {
				CPacketMgr::Send_Pos_Packet(player, Animal_Id);
				CPacketMgr::Send_Animation_Packet(player, Animal_Id, (UINT)ANIMAL_ANIMATION_TYPE::ATTACK);
			}
		}

		float fTarget_CurrHp = Target->GetHealth();
		float fTarget_AfterHp = fTarget_CurrHp;

		float fAnimalDamage = Animal->GetDamage();

		fTarget_AfterHp -= fAnimalDamage;

		if (fTarget_AfterHp <= 0.f)
		{
			// - Player
			fTarget_AfterHp = 0.f;
			Target->SetHealth(fTarget_AfterHp);
			Target->SetState(OST_DIE);
			CPacketMgr::Send_Status_Player_Packet(usTarget);

			// New Target
			USHORT NewTarget = NO_TARGET;
			float fDist = PLAYER_VIEW_RANGE;
			for (auto user : rangeList) {
				if (m_pObjectPool->m_cumPlayerPool[user]->GetState() == OST_DIE)
					continue;
				Vec3 vPos1 = Animal->GetLocalPos();
				Vec3 vPos2 = m_pObjectPool->m_cumPlayerPool[user]->GetLocalPos();
				if (ObjectRangeCheck(vPos1, vPos2, 2000.f)) {
					float currDist = CalculationDistance(vPos1, vPos2);
					if (fDist >= currDist) {
						fDist = currDist;
						NewTarget = user;
					}
				}
			}
			if (NewTarget == NO_TARGET) {
				Animal->SetWakeUp(false);
				Animal->SetTarget(NO_TARGET);
				return;
			}
			else {
				Animal->SetTarget(NewTarget);
				PushEvent_Animal_Follow(Animal_Id, NewTarget);
			}
		}
		else
		{
			// - Player
			Target->SetHealth(fTarget_AfterHp);
			CPacketMgr::Send_Status_Player_Packet(usTarget);

			// - Animal
			PushEvent_Animal_Behavior(Animal_Id, usTarget);
		}
	}
	else {
		PushEvent_Animal_Follow(Animal_Id, usTarget);
	}
}

void CMonsterProcess::FollowEvent(USHORT AnimalId, USHORT usTarget)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[AnimalId];
	auto& User = m_pObjectPool->m_cumPlayerPool[usTarget];

	// ========================== 예외 처리 ==========================
	bool bWakeUp = Animal->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[AnimalId]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;
	concurrent_unordered_set<USHORT> rangeList;
	CopyBeforeLoginList(loginList);
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		return;
	}
	// ==============================================================

	/*for (auto au : rangeList) {
		if (!m_pObjectPool->m_cumPlayerPool[au]->ExistList(AnimalId)) {
			CPacketMgr::Send_Pos_Packet(au, AnimalId);
			CPacketMgr::Send_Animation_Packet(au, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::WALK);
		}
	}*/

	if (CollisionSphere(Animal, User, 0.2f)) {
		PushEvent_Animal_Attack(AnimalId, usTarget);
	}
	else {
		Vec3 vAnimalPos = Animal->GetLocalPos();
		Animal->SetPrevPos(vAnimalPos);

		Vec3 vTargetPos = User->GetLocalPos();
		float fAnimalSpeed = Animal->GetSpeed();
		Vec3 vTargetRot = User->GetLocalRot();

		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetPrevPos(vAnimalPos);

		Vec3 vDir = XMVector3Normalize(vTargetPos - vAnimalPos);
		vDir.y = 0.f;
		vAnimalPos += vDir * fAnimalSpeed * 0.03f;


		Animal->SetLocalRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
		Animal->SetLocalPos(vAnimalPos);

		concurrent_unordered_set<USHORT> loginList;
		CopyBeforeLoginList(loginList);
		concurrent_unordered_set<USHORT> rangeList;
		InRangePlayer(loginList, rangeList, AnimalId);

		for (auto& au : rangeList)
		{
			CPacketMgr::Send_Pos_Packet(au, AnimalId);
			CPacketMgr::Send_Animation_Packet(au, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::WALK);
		}

		if (rangeList.empty()) {
			Animal->SetWakeUp(false);
			Animal->SetTarget(NO_TARGET);
			return;
		}
		PushEvent_Animal_Behavior(AnimalId, usTarget);
	}
}

void CMonsterProcess::EvastionEvent(USHORT AnimalId, USHORT usTarget)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[AnimalId];
	auto& User = m_pObjectPool->m_cumPlayerPool[usTarget];

	// ========================== 예외 처리 ==========================
	bool bWakeUp = Animal->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = CProcess::m_pObjectPool->m_cumAnimalPool[AnimalId]->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;
	concurrent_unordered_set<USHORT> rangeList;
	CopyBeforeLoginList(loginList);
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		Animal->SetTarget(NO_TARGET);
		return;
	}
	// ==============================================================

	Vec3 vAnimalPos = Animal->GetLocalPos();
	Animal->SetPrevPos(vAnimalPos);

	Vec3 vTargetPos = User->GetLocalPos();

	float fAnimalSpeed = Animal->GetSpeed();
	Vec3 vDir = XMVector3Normalize(vTargetPos - vAnimalPos);
	vDir.y = 0.f;
	vAnimalPos += - vDir * fAnimalSpeed * 0.03f;
	Animal->SetLocalRot(Vec3(0.f, atan2( - vDir.x, - vDir.z) + 3.141592f, 0.f));
	Animal->SetLocalPos(vAnimalPos);

	for (auto user : rangeList) {
		CPacketMgr::Send_Pos_Packet(user, AnimalId);
		CPacketMgr::Send_Animation_Packet(user, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::RUN);
	}

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		Animal->SetTarget(NO_TARGET);
		return;
	}

	PushEvent_Animal_Behavior(AnimalId, usTarget);
}

void CMonsterProcess::IdleEvent(USHORT AnimalId)
{	
	auto& Animal = m_pObjectPool->m_cumAnimalPool[AnimalId];

// ========================== 예외 처리 ==========================
	bool bWakeUp = Animal->GetWakeUp();
	if (!bWakeUp) return;

	char Animal_State = Animal->GetState();
	if (Animal_State == OBJ_STATE_TYPE::OST_DIE)	return;

	concurrent_unordered_set<USHORT> loginList;
	concurrent_unordered_set<USHORT> rangeList;
	CopyBeforeLoginList(loginList);
	InRangePlayer(loginList, rangeList, AnimalId);

	if (rangeList.empty()) {
		Animal->SetWakeUp(false);
		return;
	}

	// ==============================================================

	int iCount = Animal->GetBehaviorCount();

	if (iCount == 0) {
		int random = rand() % 2;

		if (random == 0) {
			Animal->SetIdle(false);
			Animal->SetBehaviorCount(rand()%5);
			Vec3 vDir = Vec3(urd(rd), 0.f, rand() / urd(rd));
			vDir = XMVector3Normalize(vDir);
			Animal->SetDir(vDir);
		}
		else if (random == 1) {
			Animal->SetIdle(true);
			Animal->SetBehaviorCount(rand() % 5);
		}
	}
	else {
		Animal->MinusBehaviorCount();

		bool bIdle = Animal->GetIdle();

		if (bIdle) {

		}
		else {
			Vec3	vPos = Animal->GetLocalPos();
			Animal->SetPrevPos(vPos);

			/*Vec3 vDir = Vec3(rand() / (float)RAND_MAX, 0.f, rand() / (float)RAND_MAX);
			vDir = XMVector3Normalize(vDir);
			Animal->SetDir(vDir);*/
			Vec3 vDir = Animal->GetDir();
			float fSpeed = Animal->GetSpeed();
			vPos += vDir * fSpeed * 0.03f;

			char eType = Animal->GetKind();

			if (A_BEAR == eType || A_BOAR == eType)
				Animal->SetLocalRot(Vec3(-3.141592654f / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
			else {
				if (A_WOLF == eType)
					Animal->SetLocalRot(Vec3(0.f, atan2(vDir.x, vDir.z), 0.f));
				else
					Animal->SetLocalRot(Vec3(0.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
			}

			Animal->SetLocalPos(vPos);
		}
	}

	// Animation 보내기, 주변 타겟 찾기

	USHORT NewTarget = NO_TARGET;
	float fDist = PLAYER_VIEW_RANGE;
	for (auto user : loginList) {
		auto& Player = m_pObjectPool->m_cumPlayerPool[user];

		Vec3 vPos1 = Animal->GetLocalPos();
		Vec3 vPos2 = Player->GetLocalPos();
		if (ObjectRangeCheck(vPos1, vPos2, PLAYER_VIEW_RANGE)) {
			bool bIdle = Animal->GetIdle();
			if (bIdle) {

			}
			else {
				CPacketMgr::Send_Pos_Packet(user, AnimalId);
				CPacketMgr::Send_Animation_Packet(user, AnimalId, (UINT)ANIMAL_ANIMATION_TYPE::WALK);
			}
			float currDist = CalculationDistance(vPos1, vPos2);
			if (fDist >= currDist) {
				fDist = currDist;
				NewTarget = user;
			}
		}
	}

	//USHORT usNewTarget = FindTarget(AnimalId);
	if (NewTarget == NO_TARGET) {
		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetTarget(NO_TARGET);
		PushEvent_Animal_Idle(AnimalId, NO_TARGET);
	}
	else {
		m_pObjectPool->m_cumAnimalPool[AnimalId]->SetTarget(NewTarget);
		PushEvent_Animal_Behavior(AnimalId, NewTarget);
	}
}

void CMonsterProcess::DieEvent(USHORT Animal_Id)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[Animal_Id];

	if (Animal->GetState() == OST_LIVE)
		return;

	Vec3 AnimalPos = Animal->GetLocalPos();
	Animal->SetPrevPos(AnimalPos);

	concurrent_unordered_set<USHORT> login_list;
	CopyBeforeLoginList(login_list);
	for (auto& user : login_list) {
		Vec3 PlayerPos = m_pObjectPool->m_cumPlayerPool[user]->GetLocalPos();
		if (ObjectRangeCheck(PlayerPos, AnimalPos, PLAYER_VIEW_RANGE)) {
			CPacketMgr::Send_Animation_Packet(user, Animal_Id, (UINT)ANIMAL_ANIMATION_TYPE::DIE);
			CPacketMgr::Send_Remove_Packet(user, Animal_Id);
		}
	}

	PushEvent_Animal_Respawn(Animal_Id);
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
	USHORT monster_id = uiMonster;

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

USHORT CMonsterProcess::FindTarget(USHORT Animal_Id)
{
	auto& Animal = m_pObjectPool->m_cumAnimalPool[Animal_Id];

	concurrent_unordered_set<USHORT>	loginList;
	CopyBeforeLoginList(loginList);

	USHORT NewTarget = NO_TARGET;
	float fDist = PLAYER_VIEW_RANGE;
	for (auto user : loginList) {
		auto& Player = m_pObjectPool->m_cumPlayerPool[user];

		Vec3 vPos1 = Animal->GetLocalPos();
		Vec3 vPos2 = Player->GetLocalPos();
		if (ObjectRangeCheck(vPos1, vPos2, 2000.f)) {
			float currDist = CalculationDistance(vPos1, vPos2);
			if (fDist >= currDist) {
				fDist = currDist;
				NewTarget = user;
			}
		}
	}
	return NewTarget;
}
