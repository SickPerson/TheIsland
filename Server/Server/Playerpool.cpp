#include "Playerpool.h"
#include "Player.h"

concurrent_unordered_map<unsigned short, CPlayer*>CPlayerpool::m_cumPlayerPool;

CPlayerpool::CPlayerpool()
{
	for (unsigned short i = 0; i < MAX_USER; ++i)
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
