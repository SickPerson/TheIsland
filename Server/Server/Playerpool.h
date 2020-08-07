#pragma once
#include "stdafx.h"
#include "Player.h"


class CPlayerpool
{
private:

public:
	CPlayerpool();
	virtual ~CPlayerpool();

public:
	static concurrent_unordered_map<USHORT, CPlayer*> m_cumPlayerPool;
};

