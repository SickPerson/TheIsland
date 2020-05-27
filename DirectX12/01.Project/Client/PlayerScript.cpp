#include "stdafx.h"

#include "PlayerScript.h"

#include "InputScript.h"
#include "InventoryScript.h"
#include "StatusScript.h"

#include <Engine/Camera.h>


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
	, m_bEnable(true)
	, m_pChat(NULL)
	, m_pInventory(NULL)
	, m_pStatus(NULL)
	, m_bInvincible(false)
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

void CPlayerScript::SetStatusObject(CGameObject * pObj)
{
	m_pStatus = pObj;
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

void CPlayerScript::OnCollision(CCollider2D * _pOther)
{
	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Animal")->GetLayerIdx() != _pOther->GetObj()->GetLayerIdx())
	{
		Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

		Vec3 vPos = Transform()->GetLocalPos();

		Vec3 vDir = XMVector3Normalize(vPos - vOtherPos);
		vDir.y = 0.f;

		if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
		{
			vPos += vDir * m_fSpeed * DT * 5.f;
		}
		else
		{
			vPos += vDir * m_fSpeed * DT;
		}

		Vec3 vRot = _pOther->Transform()->GetLocalRot();

		Transform()->SetLocalPos(vPos);
	}
}

void CPlayerScript::OnCollisionEnter(CCollider2D * _pOther)
{
}

void CPlayerScript::OnCollisionExit(CCollider2D * _pOther)
{
}

void CPlayerScript::Damage(float fDamage)
{
	if (!m_bInvincible)
	{
		m_pStatus->GetScript<CStatusScript>()->Damage(fDamage);
	}
}