#pragma once
#include "stdafx.h"
#include "Object.h"

enum class NATURAL_LOCK_TYPE
{
	TYPE,
	HEALTH,
	DESTROY,
	ORIGIN_ROT,
	TARGET_ROT,
	ANGLE,
	END
};

class CNatural:
	public CObject
{
public:
	explicit CNatural();
	virtual ~CNatural();

public:
	NATURAL_TYPE	m_eType;
	float			m_fHealth;
	bool			m_bDestroy;
	Vec3			m_vOriginRot;
	Vec3			m_vTargetRot;
	float			m_fAngle;

public:
	shared_mutex m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::END];

public:
	void SetType(NATURAL_TYPE eType);
	void SetHealth(float fHealth);
	void SetDestroy(bool bDestroy);
	void SetOriginRot(Vec3 vOriginRot);
	void SetTargetRot(Vec3 vTargetRot);
	void SetAngle(float fAngle);

public:
	NATURAL_TYPE& GetType();
	float&	GetHealth();
	bool&	GetDestroy();
	Vec3&	GetOriginRot();
	Vec3&	GetTargetRot();
	float&	GetAngle();
};

