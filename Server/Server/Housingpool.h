#pragma once
#include "stdafx.h"
#include "Housing.h"

class CHousingpool
{
public:
	CHousingpool();
	~CHousingpool();

public:
	recursive_mutex m_rmHousingPoolMutex;

public:
	static concurrent_unordered_map<USHORT, CHousing*> m_cumHousingPool;

public:
	void Install_House(CHousing* pHouse, USHORT usHouseNum);
	void Remove_House(USHORT usHouseNum);
};

