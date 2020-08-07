#include "Naturalpool.h"

concurrent_unordered_map<USHORT, CNatural*> CNaturalpool::m_cumNaturalPool;

CNaturalpool::CNaturalpool()
{
}


CNaturalpool::~CNaturalpool()
{
	for (auto& au : m_cumNaturalPool)
	{
		if (au.second)
		{
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumNaturalPool.clear();
}
