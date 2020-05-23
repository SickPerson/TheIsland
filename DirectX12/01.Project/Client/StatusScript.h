#pragma once
#include <Engine/Script.h>
class CStatusScript :
	public CScript
{
	float m_fHealth;
	float m_fHungry;
	float m_fThirst;
public:
	CStatusScript();
	virtual ~CStatusScript();

public:
	virtual void Update();

public:
	CLONE(CStatusScript);
};

