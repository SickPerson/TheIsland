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

	wchar_t			m_strPath[33];
public:
	shared_mutex m_smNaturalSharedMutex;

public:
	void SetDestroy(bool bDestroy);
	void SetType(NATURAL_TYPE eType);
	void SetHealth(float fHealth);

	void SetOriginRot(Vec3 vOriginRot);
	void SetTargetRot(Vec3 vTargetRot);
	void SetAngle(float fAngle);

	void SetstrPath(wchar_t* strPath);

public:
	const bool&			GetDestroy();
	const NATURAL_TYPE& GetType();
	const float&		GetHealth();

	const Vec3&			GetOriginRot();
	const Vec3&			GetTargetRot();
	const float&		GetAngle();

	const wchar_t*		GetstrPath();
};

