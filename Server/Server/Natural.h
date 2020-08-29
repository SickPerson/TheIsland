#pragma once
#include "stdafx.h"
#include "Object.h"

class CNatural:
	public CObject
{
public:
	explicit CNatural();
	virtual ~CNatural();

public:
	bool			m_bDestroy;
	NATURAL_TYPE	m_eType;
	float			m_fHealth;

	Vec3			m_vOriginRot;
	Vec3			m_vTargetRot;
	float			m_fAngle;

public:
	shared_mutex m_smNaturalSharedMutex;

public:
	void SetDestroy(bool bDestroy);
	void SetType(NATURAL_TYPE eType);
	void SetHealth(float fHealth);

	void SetOriginRot(Vec3 vOriginRot);
	void SetTargetRot(Vec3 vTargetRot);
	void SetAngle(float fAngle);

public:
	const bool&			GetDestroy();
	const NATURAL_TYPE& GetType();
	const float&		GetHealth();

	const Vec3&			GetOriginRot();
	const Vec3&			GetTargetRot();
	const float&		GetAngle();
};

