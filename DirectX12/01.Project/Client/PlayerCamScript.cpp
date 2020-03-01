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
	Vec3 vPos = m_pPlayer->Transform()->GetLocalPos();
	Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();
	/*float fRot = vRot.y;
	vRot.y = vRot.y / 180.f * 3.141592f;
	vPos.z = vPos.z + cosf(vRot.y) * 150.f;
	vPos.x = vPos.x + sinf(vRot.y) * 150.f;*/
	vPos.z += 150.f;

	Transform()->SetLocalPos(vPos);
	//Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));
}
