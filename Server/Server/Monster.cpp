#include "Monster.h"

CMonster::CMonster() :
	m_uiTarget(NO_TARGET),
	m_bWakeUp(false)
{
}


CMonster::~CMonster()
{
}

void CMonster::SetAnimalStatus(tAnimalStatus & animalStatus)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::STATUS]);
	m_tStatus.eKind = animalStatus.eKind;
	m_tStatus.eType = animalStatus.eType;
	m_tStatus.fDamage = animalStatus.fDamage;
	m_tStatus.fHealth = animalStatus.fHealth;
	m_tStatus.fSpeed = animalStatus.fSpeed;
	m_tStatus.fStamina = animalStatus.fStamina;
}

void CMonster::SetHealth(float & fHealth)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::HP]);
	m_tStatus.fHealth = fHealth;
}

void CMonster::SetSpeed(float & fSpeed)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::SPEED]);
	m_tStatus.fSpeed = fSpeed;
}

void CMonster::SetDamage(float & fDamage)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::DAMAGE]);
	m_tStatus.fDamage = fDamage;
}

void CMonster::SetType(BEHAVIOR_TYPE & eType)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::BEHAVIOR_TYPE]);
	m_tStatus.eType = eType;
}

void CMonster::SetKind(ANIMAL_TYPE & eKind)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::KIND]);
	m_tStatus.eKind = eKind;
}

void CMonster::SetTarget(USHORT playerId)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::TARGET]);
	m_uiTarget = playerId;
}

void CMonster::SetWakeUp(bool bWakeUp)
{
	unique_lock<shared_mutex>lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::WAKEUP]);
	m_bWakeUp = bWakeUp;
}

void CMonster::SetDir(Vec3 & vDir)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::DIR]);
	m_vMoveDir = vDir;
}

tAnimalStatus & CMonster::GetAnimalStatus()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::STATUS]);
	return m_tStatus;
}

float & CMonster::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::HP]);
	return m_tStatus.fHealth;
}

float & CMonster::GetSpeed()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::SPEED]);
	return m_tStatus.fHealth;
}

float & CMonster::GetDamage()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::DAMAGE]);
	return m_tStatus.fDamage;
}

BEHAVIOR_TYPE & CMonster::GetType()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::BEHAVIOR_TYPE]);
	return m_tStatus.eType;
}

ANIMAL_TYPE & CMonster::GetKind()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::KIND]);
	return m_tStatus.eKind;
}

USHORT & CMonster::GetTarget()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::TARGET]);
	return m_uiTarget;
}

bool & CMonster::GetWakeUp()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::WAKEUP]);
	return m_bWakeUp;
}

Vec3 & CMonster::GetDir()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::DIR]);
	return m_vMoveDir;
}