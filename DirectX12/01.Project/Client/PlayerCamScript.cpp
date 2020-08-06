#include "stdafx.h"
#include "PlayerCamScript.h"


CPlayerCamScript::CPlayerCamScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_pPlayer(nullptr)
{
}

CPlayerCamScript::~CPlayerCamScript()
{
}

void CPlayerCamScript::Update()
{
	Vec3 vDir = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec3 vPos = m_pPlayer->Transform()->GetLocalPos();
	Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();

	vPos += -vDir * 100.f;
	vPos.y += 70.f;

	vRot.y = vRot.y;

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}
