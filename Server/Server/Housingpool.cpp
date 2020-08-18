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

void CHousingpool::Install_House(CHousing * pHouse, USHORT usHouseNum)
{
	lock_guard<recursive_mutex>	lock(m_rmHousingPoolMutex);
	m_cumHousingPool.insert(make_pair(usHouseNum, pHouse));
}

void CHousingpool::Remove_House(USHORT usHouseNum)
{
	lock_guard<recursive_mutex>	lock(m_rmHousingPoolMutex);
	m_cumHousingPool.unsafe_erase(usHouseNum);
}
