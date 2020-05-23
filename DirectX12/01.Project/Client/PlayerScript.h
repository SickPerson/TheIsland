#pragma once
#include <Engine/Script.h>

class CCamera;

class CPlayerScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	
public:
	virtual void Awake();	
	virtual void Update();

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};

