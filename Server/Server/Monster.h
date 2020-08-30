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
	bool			m_bWakeUp;

	bool			m_bBehavior;
	float			m_fCurrentTime;
	Vec3			m_vMoveDir;
	Vec3			m_vPrevPos;

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

public:
	const bool& GetWakeUp();

	const tAnimalStatus& GetAnimalStatus();
	const float&	GetHealth();
	const float&	GetSpeed();
	const float&	GetDamage();
	const BEHAVIOR_TYPE&	GetType();
	const ANIMAL_TYPE&	GetKind();
	const USHORT& GetTarget();
	const Vec3&	GetDir();
	const Vec3& GetPrevPos();
};

