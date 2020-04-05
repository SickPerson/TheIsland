#pragma once
#include "stdafx.h"

class CPlayer;
class CPlayerpool
{
private:

public:
	CPlayerpool();
	virtual ~CPlayerpool();

public:
	static concurrent_unordered_map<unsigned short, CPlayer*> m_cumPlayerPool;
};

