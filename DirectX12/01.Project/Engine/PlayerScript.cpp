#include "stdafx.h"
#include "PlayerScript.h"
#include "Camera.h"


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pMainCam(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Awake()
{

}

void CPlayerScript::Update()
{
	Vec3 vPos = m_pMainCam->Transform()->GetLocalPos();
	Vec3 vRot = m_pMainCam->Transform()->GetLocalRot();

	Transform()->SetLocalRot(Vec3(0.f, vRot.y, 0.f));

	Vec3 vScale = Transform()->GetLocalScale();
	vPos.y = vScale.y / 2.f;
	Transform()->SetLocalPos(vPos);

	Transform()->SetLocalRot(Vec3(0.f, -XM_PI / 2.f, 0.f));
}

void CPlayerScript::SetCamera(CCamera * pCamera)
{
	m_pMainCam = pCamera;
}
