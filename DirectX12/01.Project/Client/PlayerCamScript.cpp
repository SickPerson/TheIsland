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

	/*vPos.x = vPos.x + (cosf(XMConvertToRadians(vRot.y)) * 150.f);
	vPos.z = vPos.z + (sinf(XMConvertToRadians(vRot.y)) * 150.f);*/

	vPos.y = 25.f;

	vPos.z += 150.f;

	Transform()->SetLocalPos(vPos);
	//Transform()->SetLocalRot(Vec3(0.f, vRot.y + 180.f, 0.f));
}
