#pragma once
#include "stdafx.h"
#include "Object.h"

class CMonster :
	public CObject
{
public:
	explicit CMonster();
	virtual ~CMonster();

private:
	tAnimalStatus	m_tStatus;

	USHORT			m_uiTarget;
	volatile bool	m_bWakeUp;

	volatile bool	m_bHit;
	Vec3			m_vMoveDir;
	Vec3			m_vPrevPos;
	volatile bool	m_bIdle;
	int				m_iBehaviorCount;

public:
	shared_mutex m_smAnimalSharedMutex;

public:
	void SetWakeUp(bool bWakeUp);

	void SetAnimalStatus(tAnimalStatus& animalStatus);
	void SetHealth(float& fHealth);
	void SetSpeed(float& fSpeed);
	void SetDamage(float& fDamage);
	void SetType(BEHAVIOR_TYPE& eType);
	void SetKind(ANIMAL_TYPE& eKind);
	void SetTarget(USHORT playerId);
	void SetDir(Vec3& vDir);
	void SetPrevPos(Vec3& vPrevPos);
	void SetHit(bool bHit);

	void SetIdle(bool bIdle) { unique_lock<shared_mutex> lock(m_smAnimalSharedMutex); m_bIdle = bIdle; };
	void SetBehaviorCount(int count) { unique_lock<shared_mutex> lock(m_smAnimalSharedMutex); m_iBehaviorCount = count; };

public:
	const bool GetWakeUp();

	const tAnimalStatus& GetAnimalStatus();
	const float&	GetHealth();
	const float&	GetSpeed();
	const float&	GetDamage();
	const BEHAVIOR_TYPE&	GetType();
	const ANIMAL_TYPE&	GetKind();
	const USHORT& GetTarget();
	const Vec3&	GetDir();
	const Vec3& GetPrevPos();
	const bool	GetHit();

	const bool GetIdle() { shared_lock<shared_mutex> lock(m_smAnimalSharedMutex); return m_bIdle; };
	const int	GetBehaviorCount() { shared_lock<shared_mutex> lock(m_smAnimalSharedMutex); return m_iBehaviorCount; };

public:
	void MinusBehaviorCount() 
	{ 
		unique_lock<shared_mutex> lock(m_smAnimalSharedMutex); 
		if (m_iBehaviorCount > 0) {
			--m_iBehaviorCount;
		}
	};
};

