#include "Natural.h"



CNatural::CNatural()
{
}


CNatural::~CNatural()
{
}

void CNatural::SetHealth(float & fHealth)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::HEALTH]);
	m_fHealth = fHealth;
}

void CNatural::SetType(NATURAL_TYPE & eType)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TPYE]);
	m_eType = eType;
}

void CNatural::SetDestroy(bool bDestroy)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::DESTROY]);
	m_bDestroy = bDestroy;
}

float & CNatural::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TPYE]);
	return m_fHealth;
}

NATURAL_TYPE& CNatural::GetType()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TPYE]);
	return m_eType;
}

bool CNatural::GetDestroy()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::DESTROY]);
	return m_bDestroy;
}
