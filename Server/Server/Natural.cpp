#include "Natural.h"

CNatural::CNatural()
{
}

CNatural::~CNatural()
{
}

void CNatural::SetType(NATURAL_TYPE eType)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	m_eType = eType;
	
	switch (eType)
	{
	case N_TREE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		m_fHealth = 150.f;
		break;
	case N_STONE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		m_fHealth = 150.f;
		break;
	case N_BUSH:
	case N_NONE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		m_fHealth = 1.f;
		break;
	default:
		break;
	}
}

void CNatural::SetHealth(float fHealth)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	m_fHealth = fHealth;
}

void CNatural::SetDestroy(bool bDestroy)
{
	unique_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	m_bDestroy = bDestroy;
}

void CNatural::SetOriginRot(Vec3 vOriginRot)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	m_vOriginRot = vOriginRot;
}

void CNatural::SetTargetRot(Vec3 vTargetRot)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	m_vTargetRot = vTargetRot;
}

void CNatural::SetAngle(float fAngle)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	m_fAngle = fAngle;
}

const NATURAL_TYPE & CNatural::GetType()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	return m_eType;
}

const float & CNatural::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	return m_fHealth;
}

const bool & CNatural::GetDestroy()
{
	shared_lock<shared_mutex> lock(m_smNaturalSharedMutex);
	return m_bDestroy;
}

const Vec3 & CNatural::GetOriginRot()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	return m_vOriginRot;
}

const Vec3 & CNatural::GetTargetRot()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	return m_vTargetRot;
}

const float & CNatural::GetAngle()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex);
	return m_fAngle;
}
