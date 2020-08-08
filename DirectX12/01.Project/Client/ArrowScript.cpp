#include "stdafx.h"
#include "ArrowScript.h"

#include "BuildScript.h"
#include "AnimalScript.h"

#include <Engine/NaviMgr.h>

#include <iostream>

CArrowScript::CArrowScript() :
	CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT),
	m_vDir(Vec3(0.f, 0.f, 0.f)),
	m_vRot(Vec3(0.f, 0.f, 0.f)),
	m_vDiffPos(Vec3(0.f, 0.f, 0.f)),
	m_fDamage(0.f),
	m_fSpeed(800.f),
	m_bCollision(false),
	m_bShoot(false),
	m_pHost(NULL),
	m_fRemainTime(ARROW_REMAIN_TIME),
	m_pCollisionObj(NULL)
{
}


CArrowScript::~CArrowScript()
{
}

void CArrowScript::Update()
{
	if (!m_bShoot)
		return;

	if (m_bCollision)
	{
		m_fRemainTime -= DT;

		if (m_pCollisionObj)
		{
			Vec3 vOtherPos = m_pCollisionObj->Transform()->GetLocalPos();
			Vec3 vPos = vOtherPos += m_vDiffPos;
			Transform()->SetLocalPos(vPos);
		}

		if (m_fRemainTime < 0.f)
		{
			tEvent tEv;
			tEv.eType = EVENT_TYPE::DELETE_OBJECT;
			tEv.wParam = (DWORD_PTR)GetObj();
			CEventMgr::GetInst()->AddEvent(tEv);
		}
		return;
	}

	Vec3 vPos = Transform()->GetLocalPos();

	m_vDir -= Vec3(0.f, 0.25f * DT, 0.f);

	if(m_vRot.x > -1.65f)
		m_vRot.x -= (XM_PI / 4.f) * DT;

	//m_vDir.Normalize(m_vDir);\

	std::cout << m_vRot.x << std::endl;


	vPos += m_vDir * m_fSpeed * DT;

	float fHeight = CNaviMgr::GetInst()->GetY(vPos);
	if (fHeight > vPos.y)
	{
		vPos.y = fHeight;
		m_bCollision = true;
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(m_vRot);
}

void CArrowScript::OnCollision(CCollider2D * _pOther)
{
	if (m_bCollision)
	{
		return;
	}

	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx() == _pOther->GetObj()->GetLayerIdx())
	{
		// 건물과 부딪힌건지 건물은 AABB로 충돌체크 수행
		HOUSING_TYPE eType = _pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType();
		if (eType >= HOUSING_TYPE::HOUSING_FOUNDATION && eType < HOUSING_TYPE::HOUSING_END)
		{
			bool bCollision = false;
			bCollision = CollisionHouse(_pOther, _pOther->GetObj()->GetScript<CBuildScript>()->GetOffsetScale(), eType);
			if (bCollision)
			{
				//Vec3 vPos = Transform()->GetWorldPos();
				//Vec3 vBuildPos = _pOther->Transform()->GetLocalPos();
				//Transform()->SetLocalPos(m_vPrevPos);

				Collision(_pOther->GetObj());
			}
		}
	}
}

void CArrowScript::OnCollisionEnter(CCollider2D * _pOther)
{
}

void CArrowScript::OnCollisionExit(CCollider2D * _pOther)
{
}

void CArrowScript::Init(CGameObject* pHost, float fDamage)
{
	m_pHost = pHost;
	m_fDamage = fDamage;
}

void CArrowScript::Charge()
{
	if(m_fSpeed < 3600.f)
		m_fSpeed += 1200.f * DT;
}

void CArrowScript::Shoot(Vec3 vDir, Vec3 vRot, Vec3 vPos)
{
	m_bShoot = true;

	m_vDir = vDir;

	m_vRot = vRot;
	m_vRot.y += XM_PI;
	m_vRot.x *= -1.f;

	Transform()->SetLocalRot(m_vRot);
	Transform()->SetLocalPos(vPos);

	tEvent evt = {};

	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)GetObj();
	evt.lParam = ((DWORD_PTR)7 << 16 | (DWORD_PTR)true);

	CEventMgr::GetInst()->AddEvent(evt);
}

void CArrowScript::Collision(CGameObject * pOther)
{
	if (m_bCollision)
		return;

	if (pOther->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Animal")->GetLayerIdx())
	{
		tEvent tEv;
		tEv.eType = EVENT_TYPE::DELETE_OBJECT;
		tEv.wParam = (DWORD_PTR)GetObj();
		CEventMgr::GetInst()->AddEvent(tEv);
		return;
	}

	m_pCollisionObj = pOther;
	m_vDiffPos = Transform()->GetLocalPos() - pOther->Transform()->GetLocalPos();
	m_bCollision = true;

	//tEvent tEv;
	//tEv.eType = EVENT_TYPE::DELETE_OBJECT;
	//tEv.wParam = (DWORD_PTR)GetObj();
	//CEventMgr::GetInst()->AddEvent(tEv);
}


bool CArrowScript::CollisionHouse(CCollider2D* _pOther, Vec3 vHouseOffsetScale, UINT iType)
{
	const Matrix& matCol1 = Collider2D()->GetColliderWorldMat();
	const Matrix& matCol2 = _pOther->GetColliderWorldMat();

	Vec3 vCol1 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	Vec3 vCol2 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	Vec3 vScale1 = Transform()->GetLocalScale();
	Vec3 vScale2 = _pOther->Transform()->GetLocalScale();

	Vec3 vColScale1 = Collider2D()->GetOffsetScale();
	Vec3 vColScale2 = Vec3(1.f, 1.f, 1.f);

	HOUSING_TYPE eType = (HOUSING_TYPE)iType;

	switch (eType)
	{
	case HOUSING_FOUNDATION:
		vColScale2 = vHouseOffsetScale;
		break;
	case HOUSING_WALL:
	case HOUSING_WINDOW:
	case HOUSING_DOOR:
	{
		Vec3 vRot = _pOther->Transform()->GetLocalRot();
		if (vRot.y != 0.f)
		{
			vColScale2 = Vec3(vHouseOffsetScale.z, vHouseOffsetScale.y, vHouseOffsetScale.x);
			//std::cout << "Turn" << vOffsetScale.x << std::endl;
		}
		else
		{
			vColScale2 = Vec3(vHouseOffsetScale.x, vHouseOffsetScale.y, vHouseOffsetScale.z);
		}
	}
	break;
	case HOUSING_FLOOR:
	{
		vColScale2 = vHouseOffsetScale;
	}
	break;
	default:
		vColScale2 = vHouseOffsetScale;
		break;
	}

	vScale1 *= vColScale1;
	vScale2 *= vColScale2;

	Vec3 vMax, vOtherMax;
	Vec3 vMin, vOtherMin;

	Vec3 vPos = Transform()->GetLocalPos();
	vPos.y += 50.f;

	vMax = vPos + vScale1;
	vMin = vPos - vScale1;
	vOtherMax = vCol2 + vScale2;
	vOtherMin = vCol2 - vScale2;

	//std::cout << "Player : " << vPos.x << " || " << vPos.y << " || " << vPos.z << " || " << std::endl;
	//std::cout << "House : " << vCol2.x << " || " << vCol2.y << " || " << vCol2.z << " || " << std::endl;

	if (vMax.x < vOtherMin.x || vMin.x > vOtherMax.x) return false;
	if (vMax.y < vOtherMin.y || vMin.y > vOtherMax.y) return false;
	if (vMax.z < vOtherMin.z || vMin.z > vOtherMax.z) return false;

	return true;
}