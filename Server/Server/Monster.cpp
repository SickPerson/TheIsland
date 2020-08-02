#include "Monster.h"

CMonster::CMonster():
	m_uiTarget(NO_TARGET),
	m_bWakeUp(false)
{
}


CMonster::~CMonster()
{
}

void CMonster::SetAnimalStatus( tAnimalStatus & animalStatus)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_STATUS]);
	m_tStatus.eKind = animalStatus.eKind;
	m_tStatus.eType = animalStatus.eType;
	m_tStatus.fBehaviorTime = animalStatus.fBehaviorTime;
	m_tStatus.fDamage = animalStatus.fDamage;
	m_tStatus.fHP = animalStatus.fHP;
	m_tStatus.fSpeed = animalStatus.fSpeed;
	m_tStatus.fStamina = animalStatus.fStamina;
}

void CMonster::SetHP( float & fHP)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_HP]);
	m_tStatus.fHP = fHP;
}

void CMonster::SetSpeed( float & fSpeed)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_SPEED]);
	m_tStatus.fSpeed = fSpeed;
}

void CMonster::SetDamage( float & fDamage)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_DAMAGE]);
	m_tStatus.fDamage = fDamage;
}

void CMonster::SetBehaviorTime( float & fTime)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_SPEED]);
	m_tStatus.fBehaviorTime = fTime;
}

void CMonster::SetType(BEHAVIOR_TYPE & eType)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_SPEED]);
	m_tStatus.eType = eType;
}

void CMonster::SetKind(ANIMAL_TYPE & eKind)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_SPEED]);
	m_tStatus.eKind = eKind;
}

void CMonster::SetTarget( unsigned int playerId)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_TARGET]);
	m_uiTarget = playerId;
}

void CMonster::SetWakeUp(bool bWakeUp)
{
	unique_lock<shared_mutex>lock(m_smAnimalSharedMutex[ANIMAL_LOCK_WAKEUP]);
	m_bWakeUp = bWakeUp;
}

void CMonster::SetDir(Vec3 & vDir)
{
	unique_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_DIR]);
	m_vMoveDir = vDir;
}

tAnimalStatus & CMonster::GetAnimalStatus()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_STATUS]);
	return m_tStatus;
}

float & CMonster::GetHP()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_HP]);
	return m_tStatus.fHP;
}

float & CMonster::GetSpeed()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_HP]);
	return m_tStatus.fHP;
}

float & CMonster::GetDamage()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_DAMAGE]);
	return m_tStatus.fDamage;
}

float & CMonster::GetBehaviorTime()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_BEHAVIORTIME]);
	return m_tStatus.fBehaviorTime;
}

BEHAVIOR_TYPE & CMonster::GetType()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_BEHAVIOR]);
	return m_tStatus.eType;
}

ANIMAL_TYPE & CMonster::GetKind()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_KIND]);
	return m_tStatus.eKind;
}

unsigned int & CMonster::GetTarget()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_TARGET]);
	return m_uiTarget;
}

bool & CMonster::GetWakeUp()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_WAKEUP]);
	return m_bWakeUp;
}

Vec3 & CMonster::GetDir()
{
	shared_lock<shared_mutex> lock(m_smAnimalSharedMutex[ANIMAL_LOCK_DIR]);
	return m_vMoveDir;
}