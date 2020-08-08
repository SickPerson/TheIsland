#pragma once
#include "stdafx.h"
#include "Housing.h"

enum class HOUSINGPOOL_LOCK_TYPE
{
	NUM,
	INSERT,
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
	volatile USHORT m_HousingNum;
	shared_mutex m_smHusingPoolSharedMutex[(UINT)HOUSINGPOOL_LOCK_TYPE::END];

public:
	static concurrent_unordered_map<USHORT, CHousing*> m_cumHousingPool;

public:
	USHORT GetNum();
public:
	void InsertHousing(UINT eType, Vec3 vPos, Vec3 vRot, Vec3 vScale);
};

