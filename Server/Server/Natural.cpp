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
	
	switch (eType)
	{
	case NATURAL_TREE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		this->SetHealth(150.f);
		m_fHealth = 150.f;
		break;
	case NATURAL_STONE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		m_fHealth = 150.f;
		break;
	case NATURAL_BUSH:
	case NATURAL_NONE:
		m_vOriginRot = Vec3(-3.141592654f / 2.f, 0.f, 0.f);
		m_fHealth = 1.f;
		break;
	default:
		break;
	}
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

void CNatural::SetOriginRot(Vec3 vOriginRot)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::ORIGIN_ROT]);
	m_vOriginRot = vOriginRot;
}

void CNatural::SetTargetRot(Vec3 vTargetRot)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TARGET_ROT]);
	m_vTargetRot = vTargetRot;
}

void CNatural::SetAngle(float fAngle)
{
	unique_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::ANGLE]);
	m_fAngle = fAngle;
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

Vec3 & CNatural::GetOriginRot()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::ORIGIN_ROT]);
	return m_vOriginRot;
}

Vec3 & CNatural::GetTargetRot()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::TARGET_ROT]);
	return m_vTargetRot;
}

float & CNatural::GetAngle()
{
	shared_lock<shared_mutex>	lock(m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::ANGLE]);
	return m_fAngle;
}
