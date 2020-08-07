#include "stdafx.h"
#include "Playerpool.h"

concurrent_unordered_map<USHORT, CPlayer*>CPlayerpool::m_cumPlayerPool;

CPlayerpool::CPlayerpool()
{
	for (int i = 0; i < MAX_USER; ++i)
		CPlayerpool::m_cumPlayerPool.insert(make_pair(i, new CPlayer()));
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
