#pragma once

#include "Entity.h"

class CResource : 
	public CEntity
{
public:
	CResource( RES_TYPE  eType );
	virtual ~CResource();

private:
	wstring		m_strPath;	// 상대경로
	RES_TYPE	m_eType;

public:
	RES_TYPE GetType();
};

