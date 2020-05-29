#include "stdafx.h"

#include "PlayerScript.h"

#include "InputScript.h"
#include "InventoryScript.h"
#include "StatusScript.h"

#include "AnimalScript.h"

#include "ItemScript.h"
#include "StuffScript.h"

#include <Engine/Camera.h>
#include <Engine/RenderMgr.h>

#include <iostream>

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(200.f)
	, m_fScaleSpeed(1.f)
	, m_bEnable(true)
	, m_pChat(NULL)
	, m_pInventory(NULL)
	, m_pStatus(NULL)
	, m_bInvincible(false)
	, m_pMainCamera(NULL)
{
	m_vCollisionObj.reserve(5);
}

CPlayerScript::~CPlayerScript()
{
	m_vCollisionObj.clear();
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

void CPlayerScript::SetMainCamera(CCamera * pCamera)
{
	m_pMainCamera = pCamera;
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
		// 채팅창도 꺼져있고 인벤토리도 꺼져있는 경우
		if (!m_pChat->GetScript<CInputScript>()->GetEnable() && !m_pInventory->GetScript<CInventoryScript>()->GetInventoryActive())
		{
			if (KEY_TAB(KEY_TYPE::KEY_LBTN))
			{
				POINT vPoint = CKeyMgr::GetInst()->GetMousePos();

				tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
				//Vec2 vPoint = Vec2(vResolution.fWidth / 2.f, vResolution.fHeight / 2.f);

				Matrix matProj = m_pMainCamera->GetProjMat();

				Vec3 vDirRay, vPosRay;
				vDirRay.x = ((2.0f * (float)vPoint.x) / vResolution.fWidth - 1.f) / matProj._11;
				vDirRay.y = ((-2.0f * (float)vPoint.y) / vResolution.fHeight + 1.f) / matProj._22;
				vDirRay.z = 1.f;

				Matrix matView = m_pMainCamera->GetViewMat();
				Matrix matViewInv = m_pMainCamera->GetViewMatInv();

				vPosRay = XMVector3TransformCoord(vPosRay, matViewInv);
				vDirRay = XMVector3TransformNormal(vDirRay, matViewInv);
				vDirRay = XMVector3Normalize(vDirRay);

				// 충돌반경 안에있는 물체들과 피킹을 수행
				for (int i = 0; i < m_vCollisionObj.size(); ++i)
				{
					if (CollisionRay(vPosRay, vDirRay, m_vCollisionObj[i]->Collider2D()))
					{
						if (m_vCollisionObj[i]->GetName() == L"Tree")
						{
							CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_WOOD);
							m_pInventory->GetScript<CInventoryScript>()->AddItem(pItem, 1);
						}
						else if (m_vCollisionObj[i]->GetName() == L"Bear")
						{
							CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_LEATHER);
							m_pInventory->GetScript<CInventoryScript>()->AddItem(pItem, 1);
						}
						else if (m_vCollisionObj[i]->GetName() == L"Deer")
						{
							CItemScript* pItem = new CStuffScript(ITEM_TYPE::ITEM_BONE);
							m_pInventory->GetScript<CInventoryScript>()->AddItem(pItem, 1);
						}
					}
				}
			}
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
	// 플레이어의 행동반경 ( 공격, 건설 등등에 들어온 오브젝트들 

	// 플레이어와 충돌한게 동물아 아닌 종류들 - 
	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Animal")->GetLayerIdx() != _pOther->GetObj()->GetLayerIdx())
	{
		// 실제의 충돌크기의 오브젝트와 부딪힌건지
		if (CollisionSphere(_pOther, _pOther->GetOffsetScale(), 0.2f))
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
	else
	{
		// 실제 동물의 크기와 부딪힌건지
		Vec3 vOffsetScale = _pOther->GetObj()->GetScript<CAnimalScript>()->GetOffsetScale();
		if (CollisionSphere(_pOther, vOffsetScale, 0.2f))
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
}

void CPlayerScript::OnCollisionEnter(CCollider2D * _pOther)
{
	// 피킹으로 공격판정을 하기 위해 반경범위에 들어오면 벡터에 수집
	m_vCollisionObj.emplace_back(_pOther->GetObj());
}

void CPlayerScript::OnCollisionExit(CCollider2D * _pOther)
{
	// 나가면 벡터에서 해제
	auto p = find(m_vCollisionObj.begin(), m_vCollisionObj.end(), _pOther->GetObj());
	if (p == m_vCollisionObj.end())
	{
		// 들어온적은 있는데 담겨있지가 않은 경우?
	}
	m_vCollisionObj.erase(p);
}

void CPlayerScript::Damage(float fDamage)
{
	if (!m_bInvincible)
	{
		m_pStatus->GetScript<CStatusScript>()->Damage(fDamage);
	}
}


bool CPlayerScript::CollisionSphere(CCollider2D* _pOther, Vec3 vOffsetScale, float fOffset)
{
	const Matrix& matCol1 = Collider2D()->GetColliderWorldMat();
	const Matrix& matCol2 = _pOther->GetColliderWorldMat();

	Vec3 vCol1 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	Vec3 vCol2 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	Vec3 vScale1 = Transform()->GetLocalScale();
	Vec3 vScale2 = _pOther->Transform()->GetLocalScale();

	Vec3 vColScale1 = Collider2D()->GetOffsetScale() * fOffset;
	Vec3 vColScale2 = vOffsetScale;

	float fDist = pow(vCol2.x - vCol1.x, 2) + pow(vCol2.y - vCol1.y, 2) + pow(vCol2.z - vCol1.z, 2);
	fDist = sqrt(fDist);

	if (fDist > fabs(vScale1.x * vColScale1.x) + fabs(vScale2.x * vColScale2.x))
		return false;

	return true;
}

bool CPlayerScript::CollisionRay(Vec3 vPosRay, Vec3 vDirRay, CCollider2D * _pOther)
{
	Matrix matWorldInv = XMMatrixIdentity();
	Vec3 vPos = _pOther->Transform()->GetLocalPos();
	vPos += _pOther->Collider2D()->GetOffsetPos();
	matWorldInv._41 = -vPos.x;
	matWorldInv._42 = -vPos.y;
	matWorldInv._43 = -vPos.z;

	vPosRay = XMVector3TransformCoord(vPosRay, matWorldInv);
	vDirRay = XMVector3TransformNormal(vDirRay, matWorldInv);

	Vec3 vv = XMVector3Dot(vDirRay, vDirRay);
	Vec3 qv = XMVector3Dot(vPosRay, vDirRay);
	Vec3 qq = XMVector3Dot(vPosRay, vPosRay);

	float r;
	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Animal")->GetLayerIdx() != _pOther->GetObj()->GetLayerIdx())
	{
		r = _pOther->Transform()->GetLocalScale().x * _pOther->GetOffsetScale().x;
	}
	else
	{
		r = _pOther->Transform()->GetLocalScale().x * _pOther->GetObj()->GetScript<CAnimalScript>()->GetOffsetScale().x;
	}

	float rr = r * r;

	if (qv.x * qv.x - vv.x * (qq.x - rr) >= 0)
	{
		return true;
	}
	return false;
}
