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

	Transform()->SetLocalRot(vRot);

	Vec3 vScale = Transform()->GetLocalScale();
	vPos.y = vScale.y / 2.f;
	Transform()->SetLocalPos(vPos);
}

void CPlayerScript::SetCamera(CCamera * pCamera)
{
	m_pMainCam = pCamera;
}
