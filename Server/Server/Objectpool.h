#pragma once
#include "stdafx.h"

#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Housing.h"
#include "Natural.h"

class CObjectpool
{
public:
	CObjectpool();
	~CObjectpool();

private:
	mutex mu;

private:
	void Initalize();
	void Release();

private:
	void Init_PlayerPool();
	void Init_AnimalPool();
	void Init_NaturalPool();
	void Init_HousingPool();

	void Release_PlayerPool();
	void Release_AnimalPool();
	void Release_NaturalPool();
	void Release_HousingPool();

public:
	static concurrent_unordered_map<USHORT, CPlayer*> m_cumPlayerPool;
	static concurrent_unordered_map<USHORT, CMonster*> m_cumAnimalPool;
	static concurrent_unordered_map<USHORT, CNatural*> m_cumNaturalPool;
	static concurrent_unordered_map<USHORT, CHousing*> m_cumHousingPool;

	USHORT GetLoginID();
	void Install_House(CHousing* pHouse, USHORT usIndex);
	void Remove_House(USHORT usIndex);
};