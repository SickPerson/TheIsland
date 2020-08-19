#pragma once
#include <Engine/Script.h>

class CHumanScript :
	public CScript
{
public:
	CHumanScript();
	virtual ~CHumanScript();
	
	CLONE(CHumanScript);
	virtual void Update() override;

public:
	void AnimationInfo(CAnimator3D* pAnimation);
};

