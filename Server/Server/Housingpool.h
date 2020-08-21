#pragma once
#include "stdafx.h"
#include "Housing.h"

enum class HOUSINGPOOL_LOCK_TYPE
{
	INSERT,
	REMOVE,
	END
};

class CHousingpool
{
public:
	CHousingpool();
	~CHousingpool();

public:
	recursive_mutex m_rmHousingPoolMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::END];

public:
	static concurrent_unordered_map<USHORT, CHousing*> m_cumHousingPool;

public:
	void Install_House(CHousing* pHouse, USHORT usHouseIndex);
	void Remove_House(USHORT usHouseIndex);
};

