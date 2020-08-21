#pragma once
#include "stdafx.h"
#include "Object.h"

enum class HOUSING_LOCK_TYPE
{
	INDEX,
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
	USHORT			m_usIndex;
	HOUSING_TYPE	m_eType;
	bool			m_bInstall;

private:
	std::shared_mutex m_smHousingSharedMutex[(UINT)HOUSING_LOCK_TYPE::END];

public:
	void SetIndex(USHORT& usIndex);
	void SetType(HOUSING_TYPE eType);
	void SetInstall(bool bInstall);

public:
	USHORT&			GetIndex();
	HOUSING_TYPE&	GetType();
	bool&			GetInstall();
};

