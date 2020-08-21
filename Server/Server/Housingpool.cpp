#include "Housingpool.h"

concurrent_unordered_map<USHORT, CHousing*> CHousingpool::m_cumHousingPool;


CHousingpool::CHousingpool()
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

void CHousingpool::Install_House(CHousing * pHouse, USHORT usHouseIndex)
{
	lock_guard<recursive_mutex>	lock(m_rmHousingPoolMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::INSERT]);
	m_cumHousingPool.insert(make_pair(usHouseIndex, pHouse));
}

void CHousingpool::Remove_House(USHORT usHouseIndex)
{
	lock_guard<recursive_mutex>	lock(m_rmHousingPoolMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::REMOVE]);
	m_cumHousingPool.unsafe_erase(usHouseIndex);
}
