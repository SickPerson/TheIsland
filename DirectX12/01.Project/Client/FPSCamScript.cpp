#include "stdafx.h"
#include "FPSCamScript.h"

#include "PlayerScript.h"

#include <Engine/Camera.h>
#include <Engine/RenderMgr.h>

#include <iostream>

CFPSCamScript::CFPSCamScript()
	: CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT)
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
		if (!m_pPlayer->GetScript<CPlayerScript>()->GetEnable())
			return;

		Vec3 vPos = m_pPlayer->Transform()->GetLocalPos();
		Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();
		Vec3 vDir = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);

		Vec3 vScale = Transform()->GetLocalScale();
		float fOffset = 20.f;
		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			fOffset = 60.f;
		}
		vPos += -vDir * fOffset;
		vPos.y += 100.f;
		Transform()->SetLocalPos(vPos);

		POINT vMousePos = CKeyMgr::GetInst()->GetMousePos();
		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		Vec2 vCenter = Vec2(vResolution.fWidth / 2.f, vResolution.fHeight / 2.f);

		Vec2 vDrag = Vec2((float)(vMousePos.x - vCenter.x), (float)(vCenter.y - vMousePos.y));

		Vec3 vCameraRot = Transform()->GetLocalRot();

		//vDrag.y -= 41.f;
		vCameraRot.x -= vDrag.y * DT * 3.f;

		if(vCameraRot.x > -XM_PI / 2.f && vCameraRot.x < XM_PI / 2.f)
			Transform()->SetLocalRot(Vec3(vCameraRot.x, vRot.y + 3.141592f, 0.f));

	}
}

void CFPSCamScript::SetPlayer(CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}
