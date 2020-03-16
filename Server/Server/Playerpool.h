#pragma once
#include "Player.h"

class CPlayerpool
{
public:
	CPlayerpool();
	virtual ~CPlayerpool();

public:
	static concurrency::concurrent_unordered_map<unsigned short, CPlayer*> m_cumPlayerPool;
};

