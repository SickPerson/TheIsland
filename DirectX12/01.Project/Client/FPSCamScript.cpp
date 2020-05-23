#include "stdafx.h"
#include "FPSCamScript.h"

#include <Engine/Camera.h>

CFPSCamScript::CFPSCamScript()
	: CScript(0)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
	, m_pPlayer(NULL)
{
}

CFPSCamScript::~CFPSCamScript()
{
}

void CFPSCamScript::Update()
{
	if (m_pPlayer)
	{
		Vec3 vPos = m_pPlayer->Transform()->GetLocalPos();
		Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();

		Vec3 vScale = Transform()->GetLocalScale();
		vPos.y = 100.f;
		Transform()->SetLocalPos(vPos);

		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vCameraRot = Transform()->GetLocalRot();

		vCameraRot.x -= vDrag.y * DT * 3.f;

		if (vCameraRot.x > 360.f)
			vCameraRot.x -= 360.f;

		Transform()->SetLocalRot(Vec3(vCameraRot.x, vRot.y, 0.f));
	}
}

void CFPSCamScript::SetPlayer(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}
