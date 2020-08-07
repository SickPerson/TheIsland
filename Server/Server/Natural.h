#pragma once
#include "stdafx.h"
#include "Object.h"

enum class NATURAL_LOCK_TYPE
{
	TYPE,
	HEALTH,
	DESTROY,
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

public:
	shared_mutex m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::END];

public:
	void SetType(NATURAL_TYPE eType);
	void SetHealth(float fHealth);
	void SetDestroy(bool bDestroy);

public:
	NATURAL_TYPE& GetType();
	float&	GetHealth();
	bool&	GetDestroy();
};

