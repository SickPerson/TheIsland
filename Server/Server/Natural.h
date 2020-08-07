#pragma once
#include "stdafx.h"
#include "Object.h"

enum NATURAL_TYPE
{
	NATURAL_TREE,
	NAUTRAL_STONE,
	NATURAL_BUSH,
	NATURAL_NONE,
	NATURAL_END
};

enum class NATURAL_LOCK_TYPE
{
	HEALTH,
	TPYE,
	DESTROY,
	END
};

class CNatural:
	public CObject
{
public:
	CNatural();
	~CNatural();

private:
	float	m_fHealth;
	float	m_fAngle;
	NATURAL_TYPE	m_eType;
	bool	m_bDestroy;
	shared_mutex m_smNaturalSharedMutex[(UINT)NATURAL_LOCK_TYPE::END];

public:
	void SetHealth(float& fHealth);
	void SetType(NATURAL_TYPE& eType);
	void SetDestroy(bool bDestroy);

public:
	float&	GetHealth();
	NATURAL_TYPE& GetType();
	bool GetDestroy();
};

