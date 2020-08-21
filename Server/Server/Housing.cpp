#include "Housing.h"



CHousing::CHousing():
	m_eType(HOUSING_END),
	m_bInstall(false)

{
}


CHousing::~CHousing()
{
}

void CHousing::SetIndex(USHORT & usIndex)
{
	unique_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::INDEX]);
	m_usIndex = usIndex;
}

void CHousing::SetType(HOUSING_TYPE eType)
{
	unique_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::TYPE]);
	m_eType = eType;
}

void CHousing::SetInstall(bool bInstall)
{
	unique_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::INSTALL]);
	m_bInstall = bInstall;
}

USHORT & CHousing::GetIndex()
{
	shared_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::INDEX]);
	return m_usIndex;
}

HOUSING_TYPE & CHousing::GetType()
{
	shared_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::TYPE]);
	return m_eType;
}

bool & CHousing::GetInstall()
{
	shared_lock<shared_mutex> lock(m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::INSTALL]);
	return m_bInstall;
}
