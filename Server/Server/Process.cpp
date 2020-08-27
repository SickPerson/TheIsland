#include "Process.h"
#include "PacketMgr.h"
#include "TimerMgr.h"

CPlayerpool*	CProcess::m_pPlayerPool = nullptr;
CMonsterpool*	CProcess::m_pMonsterPool = nullptr;
CNaturalpool*	CProcess::m_pNaturalPool = nullptr;
CHousingpool*	CProcess::m_pHousingPool = nullptr;
USHORT	CProcess::m_houseNum = 0;
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

bool CProcess::PlayerAndAnimal_CollisionSphere(USHORT playerId, USHORT animalId, float fOffset)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[animalId]->GetLocalPos();

	Vec3 vPlayerScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();
	Vec3 vAnimalScale = m_pPlayerPool->m_cumPlayerPool[animalId]->GetLocalScale();

	Vec3 vPlayerColScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale() * fOffset;
	Vec3 vAnimalColScale = m_pPlayerPool->m_cumPlayerPool[animalId]->GetLocalScale();

	float fDist = powf(vAnimalPos.x - vPlayerPos.x, 2) + powf(vAnimalPos.y - vAnimalPos.y, 2) + powf(vAnimalPos.z - vPlayerPos.z, 2);
	fDist = sqrtf(fDist);

	if (fDist > fabsf(vPlayerScale.x * vPlayerColScale.x) + fabsf(vAnimalScale.x * vAnimalScale.x))
		return false;

	return true;
}

bool CProcess::PlayerAndNatural_CollisionSphere(USHORT playerId, USHORT naturalId, float fOffset)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[naturalId]->GetLocalPos();

	Vec3 vPlayerScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();
	Vec3 vAnimalScale = m_pPlayerPool->m_cumPlayerPool[naturalId]->GetLocalScale();

	Vec3 vPlayerColScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale() * fOffset;
	Vec3 vAnimalColScale = m_pPlayerPool->m_cumPlayerPool[naturalId]->GetLocalScale();

	float fDist = powf(vAnimalPos.x - vPlayerPos.x, 2) + powf(vAnimalPos.y - vPlayerPos.y, 2) + powf(vAnimalPos.z - vPlayerPos.z, 2);
	fDist = sqrtf(fDist);

	if (fDist > fabsf(vPlayerScale.x * vPlayerColScale.x) + fabsf(vAnimalScale.x * vAnimalColScale.x))
		return false;

	return true;
}

bool CProcess::PlayerAndHouse_Collision(USHORT playerId, USHORT houseId, char eType)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vHousePos = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalPos();

	Vec3 vPlayerScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();
	Vec3 vHouseScale = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalScale();

	Vec3 vPlayerColScale = Vec3(20.f, 60.f, 20.f);
	Vec3 vHouseColScale = Vec3(1.f, 1.f, 1.f);

	HOUSING_TYPE Type = (HOUSING_TYPE)eType;

	Vec3 vHouseOffsetScale = m_pHousingPool->m_cumHousingPool[houseId]->GetOffsetScale();
	switch (Type)
	{
	case HOUSING_FOUNDATION:
		
		vHouseScale = vHouseOffsetScale;
		break;
	case HOUSING_WALL:
	case HOUSING_WINDOW:
	{
		Vec3 vRot = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalRot();
		if (vRot.y != 0.f)
		{
			vHouseColScale = Vec3(vHouseOffsetScale.z, vHouseOffsetScale.y, vHouseOffsetScale.x);
			//std::cout << "Turn" << vOffsetScale.x << std::endl;
		}
		else
		{
			vHouseColScale = Vec3(vHouseOffsetScale.x, vHouseOffsetScale.y, vHouseOffsetScale.z);
		}
	}
	break;
	case HOUSING_DOOR:
	{
		Vec3 vRot = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalRot();
		Vec3 vOffsetPos = Vec3(0.f, 0.f, 0.f);
		if (vRot.y != 0.f)
		{
			vHouseColScale = Vec3(vHouseOffsetScale.z / 3.5f, vHouseOffsetScale.y / 3.5f, vHouseOffsetScale.x);
			vOffsetPos = Vec3(80.f, 0.f, 0.f);
		}
		else
		{
			// vOffsetScale.x = 20.f;
			vHouseColScale = Vec3(vHouseOffsetScale.x, vHouseOffsetScale.y / 3.5f, vHouseOffsetScale.z / 3.5f);
			vOffsetPos = Vec3(0.f, 0.f, 80.f);
		}
		if (PlayerAndHouse_Collision_Door(playerId, houseId, vHouseColScale, vOffsetPos))
			return true;
		if (PlayerAndHouse_Collision_Door(playerId, houseId, vHouseColScale, -vOffsetPos))
			return true;

		return false;
	}
	break;
	case HOUSING_FLOOR:
	{
		vHouseColScale = vHouseOffsetScale;
	}
	break;
	default:
		vHouseColScale = vHouseOffsetScale;
		break;
	}

	vPlayerScale *= vPlayerColScale;
	vHouseScale *= vHouseColScale;

	Vec3 vMax, vOtherMax;
	Vec3 vMin, vOtherMin;

	Vec3 vPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	vPos.y += 50.f;

	vMax = vPos + vPlayerScale;
	vMin = vPos - vPlayerScale;
	vOtherMax = vHousePos + vHouseScale;
	vOtherMin = vHousePos - vHouseScale;

	if (vMax.x < vOtherMin.x || vMin.x > vOtherMax.x) return false;
	if (vMax.y < vOtherMin.y || vMin.y > vOtherMax.y) return false;
	if (vMax.z < vOtherMin.z || vMin.z > vOtherMax.z) return false;

	return true;
}

bool CProcess::PlayerAndHouse_Collision_Door(USHORT playerId, USHORT houseId, Vec3 vOffsetScale, Vec3 vOffsetPos)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vDoorPos = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalPos();

	Vec3 vScale1 = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();
	Vec3 vScale2 = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalScale();

	Vec3 vColScale1 = Vec3(20.f, 60.f, 20.f);

	vScale1 *= vColScale1;
	vScale2 *= vOffsetScale;

	Vec3 vMax, vOtherMax;
	Vec3 vMin, vOtherMin;

	Vec3 vPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	vPos.y += 50.f;

	vDoorPos += vOffsetPos;

	vMax = vPos + vScale1;
	vMin = vPos - vScale1;
	vOtherMax = vDoorPos + vScale2;
	vOtherMin = vDoorPos - vScale2;

	if (vMax.x < vOtherMin.x || vMin.x > vOtherMax.x) return false;
	if (vMax.y < vOtherMin.y || vMin.y > vOtherMax.y) return false;
	if (vMax.z < vOtherMin.z || vMin.z > vOtherMax.z) return false;

	return true;
}

bool CProcess::AnimalAndPlayer_CollisionSphere(USHORT AnimalId, USHORT PlayerId, float fOffset)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[PlayerId]->GetLocalPos();
	Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetLocalPos();

	Vec3 vPlayerScale = m_pPlayerPool->m_cumPlayerPool[PlayerId]->GetLocalScale();
	Vec3 vAnimalScale = m_pPlayerPool->m_cumPlayerPool[AnimalId]->GetLocalScale();

	Vec3 vPlayerColScale = m_pPlayerPool->m_cumPlayerPool[PlayerId]->GetLocalScale() * fOffset;
	Vec3 vAnimalColScale = m_pPlayerPool->m_cumPlayerPool[AnimalId]->GetLocalScale();

	float fDist = powf(vAnimalPos.x - vPlayerPos.x, 2) + powf(vAnimalPos.y - vAnimalPos.y, 2) + powf(vAnimalPos.z - vPlayerPos.z, 2);
	fDist = sqrtf(fDist);

	if (fDist > fabsf(vPlayerScale.x * vPlayerColScale.x) + fabsf(vAnimalScale.x * vAnimalScale.x))
		return false;

	return true;
}

bool CProcess::AnimalAndNatural_CollisionSphere(USHORT AnimalId, USHORT NaturalId, float fOffset)
{
	return false;
}

bool CProcess::AnimalAndHouse_Collision(USHORT AnimalId, USHORT HouseId, float fOffset)
{
	return false;
}

bool CProcess::AnimalAndHouse_Collision_Door(USHORT AnimalId, USHORT HouseId, Vec3 vOffsetScale, Vec3 vOffsetPos)
{
	return false;
}

void CProcess::PushEvent_Animal_Behavior(USHORT AnimalId, USHORT PlayerId)
{
	/*UINT uiType = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetType();

	if (uiType == (UINT)BEHAVIOR_TYPE::B_WARLIKE)
	{
		if (AnimalAndPlayer_CollisionSphere(AnimalId, PlayerId, (UINT)OBJ_TYPE::OT_PLAYER, 0.2f))
		{
			PushEvent_Animal_Attack(AnimalId, PlayerId);
		}
		else
		{
			PushEvent_Animal_Follow(AnimalId, PlayerId);
		}
	}
	else if (uiType == (UINT)BEHAVIOR_TYPE::B_PASSIVE)
	{
		PushEvent_Animal_Idle(AnimalId, NO_TARGET);
	}
	else if (uiType == (UINT)BEHAVIOR_TYPE::B_EVASION)
	{
		PushEvent_Animal_Evastion(AnimalId, PlayerId);
	}*/
}

void CProcess::PushEvent_Animal_Attack(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_ATTACK);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_eObjUpdate = AUT_ATTACK;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Animal_Follow(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_FOLLOW);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_eObjUpdate = AUT_FOLLOW;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Animal_Evastion(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_EVASION);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_eObjUpdate = AUT_EVASION;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Animal_Idle(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_IDLE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = AUT_IDLE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Animal_Die(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_DIE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = AUT_DIE;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Animal_Respawn(USHORT AnimalId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(OBJ_STATE_TYPE::OST_DIE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(NO_TARGET);
	BEHAVIOR_TYPE eType = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetType();
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = AUT_RESPAWN;

	if (eType == BEHAVIOR_TYPE::B_WARLIKE)
		ev.wakeup_time = high_resolution_clock::now() + 60s;
	else if (eType == BEHAVIOR_TYPE::B_PASSIVE)
		ev.wakeup_time = high_resolution_clock::now() + 50s;
	else if (eType == BEHAVIOR_TYPE::B_EVASION)
		ev.wakeup_time = high_resolution_clock::now() + 40s;
	PushEventQueue(ev);
}

void CProcess::PushEvnet_Animal_Damage(USHORT AnimalId, USHORT PlayerId)
{
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetState(AUT_DAMAGE);
	m_pMonsterPool->m_cumMonsterPool[AnimalId]->SetTarget(PlayerId);
	Update_Event ev;
	ev.m_Do_Object = AnimalId;
	ev.m_EventType = EV_MONSTER_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_eObjUpdate = AUT_DAMAGE;
	ev.wakeup_time = high_resolution_clock::now() + 16ms;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Natural_Respawn(USHORT NaturalId)
{
	m_pNaturalPool->m_cumNaturalPool[NaturalId]->SetDestroy(true);
	m_pNaturalPool->m_cumNaturalPool[NaturalId]->SetAngle(0.f);
	Update_Event ev;
	ev.m_Do_Object = NaturalId;
	ev.m_EventType = EV_NATURAL_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = NUT_RESPAWN;
	ev.wakeup_time = high_resolution_clock::now() + 10s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Natural_Damage(USHORT NaturalId, USHORT PlayerId)
{
	Update_Event ev;
	ev.m_Do_Object = NaturalId;
	ev.m_EventType = EV_NATURAL_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_eObjUpdate = NUT_DAMAGE;
	ev.wakeup_time = high_resolution_clock::now();
	PushEventQueue(ev);
}

void CProcess::PushEvent_Etc_Player_Collision()
{
	Update_Event ev;
	ev.m_Do_Object = 9996;
	ev.m_EventType = EV_ETC;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = EUT_PLAYER_COLLISION;
	ev.wakeup_time = high_resolution_clock::now() + 50ms;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Etc_Animal_Collision()
{
	Update_Event ev;
	ev.m_Do_Object = 9997;
	ev.m_EventType = EV_ETC;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = EUT_ANIMAL_COLLISION;
	ev.wakeup_time = high_resolution_clock::now() + 50ms;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Etc_Weather()
{
	Update_Event ev;
	ev.m_Do_Object = 9998;
	ev.m_EventType = EV_ETC;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = EUT_WEATHER;
	ev.wakeup_time = high_resolution_clock::now() + 30s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_Etc_Time()
{
	Update_Event ev;
	ev.m_Do_Object = 9999;
	ev.m_EventType = EV_ETC;
	ev.m_From_Object = NO_TARGET;
	ev.m_eObjUpdate = EUT_TIMER;
	ev.wakeup_time = high_resolution_clock::now() + 1s;
	PushEventQueue(ev);
}

void CProcess::PushEvent_DB_UserSave()
{
	for (auto& player : m_pPlayerPool->m_cumPlayerPool) {
		bool bConnect = player.second->GetConnect();
		if (!bConnect) continue;
		DB_Event ev;
		ev.state = EV_DB;
		ev.fHealth = player.second->GetHealth();
		ev.fHungry = player.second->GetHungry();
		ev.fThirst = player.second->GetThirst();
		Vec3 vPos = player.second->GetLocalPos();
		ev.fX = vPos.x;
		ev.fY = vPos.y;
		ev.fZ = vPos.z;
	}
}
