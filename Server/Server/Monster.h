#pragma once
#include "stdafx.h"
#include "Object.h"

enum class ANIMAL_LOCK_TYPE
{
	STATUS, HP, STAMINA, SPEED, DAMAGE, BEHAVIOR_TYPE, KIND, WAKEUP, TARGET, DIR, END
};

class CMonster :
	public CObject
{
public:
	explicit CMonster();
	virtual ~CMonster();

private:
	tAnimalStatus	m_tStatus;

	USHORT	m_uiTarget;
	bool			m_bWakeUp;

	bool			m_bBehavior;
	float			m_fCurrentTime;
	Vec3			m_vMoveDir;

	float			m_fAttackCoolTime;
	float			m_fAttackTime;
	float			m_fNpcTime = 0.f;

	float	m_fRotate = 0.f;
	shared_mutex m_smAnimalSharedMutex[(UINT)ANIMAL_LOCK_TYPE::END];
	//concurrent_priority_queue<Monster_Event>	m_MonsterEventQueue;
	chrono::high_resolution_clock::time_point	m_tpLastChangeDir;
	Vec3 vRandomPos;

public:
	void SetAnimalStatus(tAnimalStatus& animalStatus);
	void SetHealth(float& fHealth);
	void SetSpeed(float& fSpeed);
	void SetDamage(float& fDamage);
	void SetType(BEHAVIOR_TYPE& eType);
	void SetKind(ANIMAL_TYPE& eKind);
	void SetTarget(USHORT playerId);
	void SetWakeUp(bool bWakeUp);
	void SetDir(Vec3& vDir);

public:
	tAnimalStatus& GetAnimalStatus();
	float&	GetHealth();
	float&	GetSpeed();
	float&	GetDamage();
	BEHAVIOR_TYPE&	GetType();
	ANIMAL_TYPE&	GetKind();
	USHORT& GetTarget();
	bool& GetWakeUp();
	Vec3&	GetDir();
};

