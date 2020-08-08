#include "Process.h"
#include "PacketMgr.h"

CPlayerpool*	CProcess::m_pPlayerPool = nullptr;
CMonsterpool*	CProcess::m_pMonsterPool = nullptr;
CNaturalpool*	CProcess::m_pNaturalPool = nullptr;
CHousingpool*	CProcess::m_pHousingPool = nullptr;
USHORT	CProcess::m_housingNum = 0;
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
	Vec3 vAnimalColScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();

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

	float fDist = powf(vAnimalPos.x - vPlayerPos.x, 2) + powf(vAnimalPos.y - vAnimalPos.y, 2) + powf(vAnimalPos.z - vPlayerPos.z, 2);
	fDist = sqrtf(fDist);

	if (fDist > fabsf(vPlayerScale.x * vPlayerColScale.x) + fabsf(vAnimalScale.x * vAnimalScale.x))
		return false;

	return true;
}

bool CProcess::PlayerAndHouse_Collision(USHORT playerId, USHORT houseId, UINT uiType)
{
	Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vHousePos = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalPos();

	Vec3 vPlayerScale = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalScale();
	Vec3 vHouseScale = m_pHousingPool->m_cumHousingPool[houseId]->GetLocalScale();

	Vec3 vPlayerColScale = Vec3(20.f, 60.f, 20.f);
	Vec3 vHouseColScale = Vec3(1.f, 1.f, 1.f);

	HOUSING_TYPE eType = (HOUSING_TYPE)uiType;

	Vec3 vHouseOffsetScale = m_pHousingPool->m_cumHousingPool[houseId]->GetOffsetScale();
	switch (eType)
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
