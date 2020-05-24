#include "stdafx.h"

#include "PlayerScript.h"

#include "InputScript.h"
#include "InventoryScript.h"

#include <Engine/Camera.h>


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
	, m_bEnable(true)
	, m_pChat(NULL)
	, m_pInventory(NULL)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::SetChatObject(CGameObject * pObj)
{
	m_pChat = pObj;
}

void CPlayerScript::SetInventoryObject(CGameObject * pObj)
{
	m_pInventory = pObj;
}

bool CPlayerScript::GetEnable()
{
	return m_bEnable;
}

void CPlayerScript::Awake()
{

}

void CPlayerScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fSpeed = m_fSpeed;

	if (m_pChat && m_pInventory)
	{
		if (!m_pChat->GetScript<CInputScript>()->GetEnable() && !m_pInventory->GetScript<CInventoryScript>()->GetInventoryActive())
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
			{
				fSpeed *= 5.f;
			}

			if (KEY_HOLD(KEY_TYPE::KEY_W))
			{
				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				vPos += vFront * fSpeed * DT;
				vPos.y = 0.f;
			}

			if (KEY_HOLD(KEY_TYPE::KEY_S))
			{
				Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
				vPos += vBack * fSpeed * DT;
				vPos.y = 0.f;
			}

			if (KEY_HOLD(KEY_TYPE::KEY_A))
			{
				Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				vPos += vLeft * fSpeed * DT;
				vPos.y = 0.f;
			}

			if (KEY_HOLD(KEY_TYPE::KEY_D))
			{
				Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				vPos += vRight * fSpeed * DT;
				vPos.y = 0.f;
			}

			Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
			Vec3 vRot = Transform()->GetLocalRot();

			vRot.y += vDrag.x * DT * 1.5f;

			if (vRot.y > 360.f)
				vRot.y -= 360.f;

			Transform()->SetLocalRot(Vec3(0.f, vRot.y, 0.f));

			Transform()->SetLocalPos(vPos);
			m_bEnable = true;
		}
		else
			m_bEnable = false;
	}
	else
	{
		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			fSpeed *= 5.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vFront * fSpeed * DT;
			vPos.y = 0.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vBack * fSpeed * DT;
			vPos.y = 0.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vLeft * fSpeed * DT;
			vPos.y = 0.f;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vRight * fSpeed * DT;
			vPos.y = 0.f;
		}

		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		vRot.y += vDrag.x * DT * 1.5f;

		if (vRot.y > 360.f)
			vRot.y -= 360.f;

		Transform()->SetLocalRot(Vec3(0.f, vRot.y, 0.f));

		Transform()->SetLocalPos(vPos);
		m_bEnable = true;
	}
}
