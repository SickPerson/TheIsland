#pragma once
#include "stdafx.h"
#include "Monster.h"

class CMonsterpool
{
private:
	USHORT m_usNumObject = 0;
public:
	explicit CMonsterpool();
	virtual ~CMonsterpool();

public:
	static concurrent_unordered_map<USHORT, CMonster*> m_cumMonsterPool;
};

