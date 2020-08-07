#include "Natural.h"

CNatural::CNatural()
{
}

CNatural::~CNatural()
{
}

void CNatural::SetType(NATURAL_TYPE eType)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TYPE]);
	m_eType = eType;
}

void CNatural::SetHealth(float fHealth)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::HEALTH]);
	m_fHealth = fHealth;
}

void CNatural::SetDestroy(bool bDestroy)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::DESTROY]);
	m_bDestroy = bDestroy;
}

NATURAL_TYPE & CNatural::GetType()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TYPE]);
	return m_eType;
}

float & CNatural::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::HEALTH]);
	return m_fHealth;
}

bool & CNatural::GetDestroy()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::DESTROY]);
	return m_bDestroy;
}
