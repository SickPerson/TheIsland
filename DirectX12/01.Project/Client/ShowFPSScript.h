#pragma once
#include <Engine/Script.h>

class CShowFPSScript :
	public CScript
{
public:
	CShowFPSScript();
	virtual ~CShowFPSScript();

	CLONE(CShowFPSScript);
	virtual void Update() override;
};

