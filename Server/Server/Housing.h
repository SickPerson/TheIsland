#pragma once
#include "stdafx.h"
#include "Object.h"

class CHousing:
	public CObject
{
public:
	explicit CHousing();
	virtual ~CHousing();

public:
	USHORT			m_usIndex;
	HOUSING_TYPE	m_eType;
	bool			m_bInstall;
	UINT			m_iGrade;

private:
	shared_mutex m_smHousingSharedMutex;

public:
	void SetType(HOUSING_TYPE eType);
	void SetInstall(bool bInstall);
	void SetUpgrade();

public:
	const HOUSING_TYPE&	GetType();
	const bool&			GetInstall();
	const int&			GetUpgrade();
};

