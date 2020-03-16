#include "stdafx.h"
#include "Playerpool.h"

//concurrency::concurrent_unordered_map<unsigned short, CPlayer*>CPlayerpool::m_cumPlayerPool;

CPlayerpool::CPlayerpool()
{
	for (unsigned short i = 0; i < MAX_USER; ++i)
		CPlayerpool::m_cumPlayerPool.insert(std::make_pair(i, new CPlayer()));
}


CPlayerpool::~CPlayerpool()
{
	for (auto& au : m_cumPlayerPool)
	{
		if (au.second) {
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumPlayerPool.clear();
}
