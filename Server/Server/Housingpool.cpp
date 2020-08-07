#include "Housingpool.h"

concurrent_unordered_map<USHORT, CHousing*> CHousingpool::m_cumHousingPool;


CHousingpool::CHousingpool():
	m_HousingNum(0)
{
}


CHousingpool::~CHousingpool()
{
	for (auto& au : m_cumHousingPool)
	{
		if (au.second)
		{
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumHousingPool.clear();
}

USHORT CHousingpool::GetNum()
{
	shared_lock<shared_mutex> lock(m_smHusingPoolSharedMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::NUM]);
	return m_HousingNum;
}

void CHousingpool::InsertHousing(UINT eType, Vec3 vPos, Vec3 vRot, Vec3 vScale)
{
	unique_lock<shared_mutex> lock(m_smHusingPoolSharedMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::INSERT]);
	CHousing*	Housing;
	Housing->SetType((HOUSING_TYPE)eType);
	Housing->SetLocalPos(vPos);
	Housing->SetLocalRot(vRot);
	Housing->SetLocalScale(vScale);
	m_cumHousingPool.insert(make_pair(m_HousingNum++, Housing));
}
