#pragma once
#include "Script.h"

class CCamera;

class CPlayerScript :
	public CScript
{
private:
	CCamera* m_pMainCam;
	
public:
	virtual void Awake();	
	virtual void Update();

	void SetCamera(CCamera* pCamera);

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};

