#pragma once
#include <Engine/Script.h>
#define INVINCIBLE_TIME 3.f

class CStatusScript :
	public CScript
{
	float m_fHealth;
	float m_fArmor;
	float m_fHungry;
	float m_fThirst;

	bool m_bGameOver;

	CGameObject* m_pArmor;
public:
	CStatusScript();
	virtual ~CStatusScript();

public:
	virtual void Update();

public:
	CLONE(CStatusScript);

public:
	void Damage(float fDamage);

	void SetIncreaseHealth(float fAmount);
	void SetIncreaseHungry(float fAmount);
	void SetIncreasefThirst(float fAmount);

	bool GetGameOver();

	void EquipArmor(float fArmor);
};

