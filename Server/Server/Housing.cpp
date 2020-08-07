#include "Housing.h"



CHousing::CHousing()
{
}


CHousing::~CHousing()
{
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
