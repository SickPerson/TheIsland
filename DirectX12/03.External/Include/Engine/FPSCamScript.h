#pragma once
#include "Script.h"

class CFPSCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;

public:
	virtual void Update();

public:
	CLONE(CFPSCamScript);

public:
	CFPSCamScript();
	virtual ~CFPSCamScript();
};
