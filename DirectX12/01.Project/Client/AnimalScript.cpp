#include "stdafx.h"
#include "AnimalScript.h"
#include "PlayerScript.h"

#include <iostream>

CAnimalScript::CAnimalScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_tStatus {
		100.f, // HP
		100.f, // Stamina
		200.f, // Speed
		10.f,  // Damage
		2.f, // BehaviorTime
		BEHAVIOR_TYPE::B_EVASION, // Type
		ANIMAL_TYPE::A_DEER // Kind
	}
	, m_vOffsetScale(Vec3(20.f, 20.f, 20.f))
	, m_bBehavior(false)
	, m_fCurrentTime(0.f)
	, m_vMoveDir(Vec3(0.f, 0.f, 0.f))
	, m_pTarget(NULL)
	, m_fAttackCoolTime(3.f)
	, m_fAttackTime(0.f)
{

}

CAnimalScript::~CAnimalScript()
{
}

void CAnimalScript::Update()
{
	if (m_fAttackTime > -1.f)
	{
		m_fAttackTime -= DT;
	}

	if (!m_bBehavior)
	{
		return;
	}

	m_fCurrentTime -= DT;
	if (m_fCurrentTime < 0.f)
	{
		m_bBehavior = false;
		m_fCurrentTime = 0.f;
	}

	// 플레이어가 시야범위를 벗어날 경우 어떤 행동을 계속 취할 것인가
	// 회피형 동물 ( 사슴 )
	if (BEHAVIOR_TYPE::B_EVASION == m_tStatus.eType)
	{
		// 마지막 방향으로 꾸준히 달림
		Vec3 vPos = Transform()->GetLocalPos();

		vPos += m_vMoveDir * m_tStatus.fSpeed * DT;

		Transform()->SetLocalPos(vPos);
	}
	else if (BEHAVIOR_TYPE::B_PASSIVE == m_tStatus.eType)
	{
		// 비선공 유형

	}
	else if (BEHAVIOR_TYPE::B_WARLIKE == m_tStatus.eType)
	{
		// 선공몹
		// 쫒아가서 팬다
		Vec3 vOtherPos = m_pTarget->Transform()->GetLocalPos();

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vTempPos = vPos;

		Vec3 vDir = XMVector3Normalize(vOtherPos - vPos);
		vDir.y = 0.f;

		vPos += vDir * m_tStatus.fSpeed * DT;

		Vec3 vRot = m_pTarget->Transform()->GetLocalRot();

		Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
		Transform()->SetLocalPos(vPos);
	}
}

void CAnimalScript::OnCollision(CCollider2D * _pOther)
{
	// 플레이어가 아닌 다른 물체 ( 환경요소 )
	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player")->GetLayerIdx() != _pOther->GetObj()->GetLayerIdx())
	{
		if (CollisionSphere(m_vOffsetScale, _pOther)) // 환경요소랑은 자기 몸통만큼 추가 충돌체크를 진행하고 막히면 반대로 튕겨나오도록
		{
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

			Vec3 vPos = Transform()->GetLocalPos();

			Vec3 vDir = XMVector3Normalize(vPos - vOtherPos);
			vDir.y = 0.f;

			vPos += vDir * m_tStatus.fSpeed * DT;

			Vec3 vRot = _pOther->Transform()->GetLocalRot();

			//Transform()->SetLocalRot(Vec3(0.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
			Transform()->SetLocalPos(vPos);
		}

		// 타입에 따른 추가 행동 요소 구현할 곳

		return;
	}


	// 플레이어가 일정 범위 안에 들어왔을때
	if (m_bBehavior)
		m_bBehavior = false; // 하던 행동 중지

	// 회피형 동물 ( 사슴 )
	if (BEHAVIOR_TYPE::B_EVASION == m_tStatus.eType)
	{
		Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

		Vec3 vPos = Transform()->GetLocalPos();

		Vec3 vDir = XMVector3Normalize(vPos - vOtherPos);
		vDir.y = 0.f;

		vPos += vDir * m_tStatus.fSpeed * DT;

		Vec3 vRot = _pOther->Transform()->GetLocalRot();

		Transform()->SetLocalRot(Vec3(0.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
		Transform()->SetLocalPos(vPos);
	}
	else if (BEHAVIOR_TYPE::B_PASSIVE == m_tStatus.eType)
	{
		// 비선공 유형 동물의 시야에 플레이어가 들어왔을때

	}
	else if (BEHAVIOR_TYPE::B_WARLIKE == m_tStatus.eType)
	{
		// 선공몹
		Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vTempPos = vPos;

		Vec3 vDir = XMVector3Normalize(vOtherPos - vPos);
		vDir.y = 0.f;

		vPos += vDir * m_tStatus.fSpeed * DT;

		Vec3 vRot = _pOther->Transform()->GetLocalRot();


		Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, atan2(vDir.x, vDir.z) + 3.141592f, 0.f));
		Transform()->SetLocalPos(vPos);

		if (CollisionSphere(m_vOffsetScale, _pOther))
		{
			if (m_fAttackTime < 0.f)
			{
				_pOther->GetObj()->GetScript<CPlayerScript>()->Damage(m_tStatus.fDamage);
				m_fAttackTime = m_fAttackCoolTime;
			}
			Transform()->SetLocalPos(vTempPos);
		}
	}
	


}

void CAnimalScript::OnCollisionEnter(CCollider2D * _pOther)
{

}

void CAnimalScript::OnCollisionExit(CCollider2D * _pOther)
{
	// 플레이어가 시야반경에서 벗어나도 일정 행동을 수행하도록
	if (CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Player")->GetLayerIdx() == _pOther->GetObj()->GetLayerIdx())
	{
		// 회피형 동물 ( 사슴 )
		if (BEHAVIOR_TYPE::B_EVASION == m_tStatus.eType)
		{
			m_vMoveDir = _pOther->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();
			Vec3 vPos = Transform()->GetLocalPos();

			m_vMoveDir = XMVector3Normalize(vPos - vOtherPos);
			m_vMoveDir.y = 0.f;

			m_bBehavior = true;
			m_fCurrentTime = m_tStatus.fBehaviorTime;
		}
		else if (BEHAVIOR_TYPE::B_PASSIVE == m_tStatus.eType)
		{
			// 비선공 유형 동물의 시야에 플레이어가 들어왔을때

		}
		else if (BEHAVIOR_TYPE::B_WARLIKE == m_tStatus.eType)
		{
			m_pTarget = _pOther->GetObj();

			m_vMoveDir = _pOther->Transform()->GetLocalDir(DIR_TYPE::FRONT);
			m_bBehavior = true;
			m_fCurrentTime = m_tStatus.fBehaviorTime;
		}
	}
}

bool CAnimalScript::CollisionSphere(Vec3 vOffsetScale, CCollider2D* _pOther)
{
	const Matrix& matCol1 = Collider2D()->GetColliderWorldMat();
	const Matrix& matCol2 = _pOther->GetColliderWorldMat();
		
	Vec3 vCol1 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol1);
	Vec3 vCol2 = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), matCol2);

	Vec3 vScale1 = Transform()->GetLocalScale();
	Vec3 vScale2 = _pOther->Transform()->GetLocalScale();

	Vec3 vColScale1 = vOffsetScale;
	Vec3 vColScale2 = _pOther->Collider2D()->GetOffsetScale();

	float fDist = pow(vCol2.x - vCol1.x, 2) + pow(vCol2.y - vCol1.y, 2) + pow(vCol2.z - vCol1.z, 2);
	fDist = sqrt(fDist);

	if (fDist > fabs(vScale1.x * vColScale1.x) + fabs(vScale2.x * vColScale2.x))
		return false;

	return true;
}

void CAnimalScript::SetAnimalStatus(tAnimalStatus tStatus)
{
	m_tStatus = tStatus;
}

tAnimalStatus CAnimalScript::GetAnimalStatus()
{
	return m_tStatus;
}

void CAnimalScript::SetOffsetScale(Vec3 vScale)
{
	m_vOffsetScale = vScale;
}

void CAnimalScript::SetBehaviorType(BEHAVIOR_TYPE eType)
{
	m_tStatus.eType = eType;
}

void CAnimalScript::SetAnimalType(ANIMAL_TYPE eKind)
{
	m_tStatus.eKind = eKind;
}

void CAnimalScript::SetAttackCoolTime(float fTime)
{
	m_fAttackCoolTime = fTime;
	m_fAttackTime = fTime;
}
