#pragma once
#include "stdafx.h"
#include "Monster.h"

class CMonsterpool
{
private:
	unsigned short m_usNumObject = 0;
public:
	explicit CMonsterpool();
	virtual ~CMonsterpool();

public:
	static concurrent_unordered_map<unsigned int, CMonster*> m_cumMonsterPool;
};

