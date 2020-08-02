#pragma once
#include "stdafx.h"
#include "Object.h"

enum ANIMAL_LOCK_TYPE {
	ANIMAL_LOCK_STATUS,
	ANIMAL_LOCK_HP,
	ANIMAL_LOCK_STAMINA,
	ANIMAL_LOCK_SPEED,
	ANIMAL_LOCK_DAMAGE,
	ANIMAL_LOCK_BEHAVIORTIME,
	ANIMAL_LOCK_BEHAVIOR,
	ANIMAL_LOCK_KIND,
	ANIMAL_LOCK_WAKEUP,
	ANIMAL_LOCK_TARGET,
	LAST_CHANGE_DIR_LOCK,
	ANIMAL_LOCK_DIR,
	RANDOM_POS_LOCK,
	STATUS_LOCK,
	MONSTER_LOCK_END
};

class CMonster:
	public CObject
{
public:
	explicit CMonster();
	virtual ~CMonster();

private:
	tAnimalStatus	m_tStatus;

	unsigned int	m_uiTarget;
	bool			m_bWakeUp;

	bool			m_bBehavior;
	float			m_fCurrentTime;
	Vec3			m_vMoveDir;

	float			m_fAttackCoolTime;
	float			m_fAttackTime;
	float			m_fNpcTime = 0.f;

	float	m_fRotate = 0.f;
	shared_mutex m_smAnimalSharedMutex[MONSTER_LOCK_END];
	//concurrent_priority_queue<Monster_Event>	m_MonsterEventQueue;
	chrono::high_resolution_clock::time_point	m_tpLastChangeDir;
	Vec3 vRandomPos;

public:
	void SetAnimalStatus( tAnimalStatus& animalStatus);
	void SetHP( float& fHP);
	void SetSpeed( float& fSpeed);
	void SetDamage( float& fDamage);
	void SetBehaviorTime( float& fTime);
	void SetType(BEHAVIOR_TYPE& eType);
	void SetKind(ANIMAL_TYPE& eKind);
	void SetTarget( unsigned int playerId);
	void SetWakeUp(bool bWakeUp);
	void SetDir(Vec3& vDir);

public:
	tAnimalStatus& GetAnimalStatus();
	float&	GetHP();
	float&	GetSpeed();
	float&	GetDamage();
	float&	GetBehaviorTime();
	BEHAVIOR_TYPE&	GetType();
	ANIMAL_TYPE&	GetKind();
	unsigned int& GetTarget();
	bool& GetWakeUp();
	Vec3&	GetDir();
public:
	//const unsigned int GetFollow() { unique_lock<shared_mutex>lock(m_smAnimalSharedMutex[FOLLOW_LOCK]); return m_iTarget; };
	//bool GetWakeUp() { std::shared_lock<std::shared_mutex>lock(m_smAnimalSharedMutex[ANIMAL_LOCK_WAKEUP]); return m_bWakeUp; };

public:
	/*void PushEvent(Vec3& vToTarget, float& fRotate, OBJ_STATE eState) {
		m_MonsterEventQueue.push(Monster_Event{ vToTarget, fRotate, eState });
	}
	bool PopEvent(Monster_Event& Event) {
		if (m_MonsterEventQueue.try_pop(Event))
			return true;
		return false;
	}
	void ClearEvent() { m_MonsterEventQueue.clear(); }*/
};

