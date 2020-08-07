#pragma once
#include "stdafx.h"
#include "Natural.h"
class CNaturalpool
{
public:
	CNaturalpool();
	~CNaturalpool();

public:
	static concurrent_unordered_map<USHORT, CNatural*> m_cumNaturalPool;
};