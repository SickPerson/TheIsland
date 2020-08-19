#include "stdafx.h"
#include "HumanScript.h"

#include <Engine/Animator3D.h>

CHumanScript::CHumanScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT)
{
}


CHumanScript::~CHumanScript()
{
}

void CHumanScript::Update()
{
}

void CHumanScript::AnimationInfo(CAnimator3D * pAnimation)
{
	pAnimation->AddClip(L"Walk", 0, 24, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Run", 25, 42, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Idle1", 43, 193, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Idle2", 200, 379, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Die", 380, 442, ANIMATION_MODE::ONCE_STOP);
	pAnimation->AddClip(L"TakeWeapon", 443, 477, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Attack1", 478, 554, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Attack2", 555, 624, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Attack3", 625, 695, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Hit1", 696, 730, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Hit2", 731, 778, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->AddClip(L"Jump", 779, 830, ANIMATION_MODE::ONCE_RETURN);
	pAnimation->SetDefaultKey(L"Idle1");
}
