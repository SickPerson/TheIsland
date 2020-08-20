#pragma once
#include <Engine/Script.h>

class CRainScript :
	public CScript
{
public:
	CRainScript();
	~CRainScript();

	// CScript을(를) 통해 상속됨
	CLONE( CRainScript );
	virtual void Update() override;

private:
	bool m_bActive;

	float m_fAlpha;
};

