#pragma once
#include "Script.h"

class CToolCameraScript :
	public CScript
{
public:
	CToolCameraScript();
	virtual ~CToolCameraScript();
	
	CLONE( CToolCameraScript );

private:
	float		m_fSpeed;
	float		m_fScaleSpeed;

public:
	virtual void Update();
};

