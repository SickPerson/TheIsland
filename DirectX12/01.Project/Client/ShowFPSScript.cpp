#include "stdafx.h"
#include "ShowFPSScript.h"

#include <Engine/TimeMgr.h>
#include <Engine/Font.h>

CShowFPSScript::CShowFPSScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT)
{
}


CShowFPSScript::~CShowFPSScript()
{
}

void CShowFPSScript::Update() 
{
	string strText = "FPS : " + std::to_string(CTimeMgr::GetInst()->GetFrame());

	Font()->SetString(strText);
}