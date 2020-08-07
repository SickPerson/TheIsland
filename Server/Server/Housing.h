#pragma once
#include "stdafx.h"
#include "Object.h"

enum class HOUSING_LOCK_TYPE
{
	TYPE,
	INSTALL,
	END
};

class CHousing:
	public CObject
{
public:
	explicit CHousing();
	virtual ~CHousing();

public:
	HOUSING_TYPE	m_eType;
	bool			m_bInstall;

public:
	shared_mutex m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::END];

public:
	void SetType(HOUSING_TYPE eType);
	void SetInstall(bool bInstall);

public:
	HOUSING_TYPE&	GetType();
	bool&			GetInstall();
};

