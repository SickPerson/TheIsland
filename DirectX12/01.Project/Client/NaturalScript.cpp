#include "stdafx.h"
#include "NaturalScript.h"

#include <Engine/ParticleSystem.h>

#include <iostream>

CNaturalScript::CNaturalScript(NATURAL_TYPE eType) :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_fTime(0.f),
	m_eType(eType),
	m_bDestory(false),
	m_bRotate(false),
	m_pParticleObj(NULL),
	m_fAngle(0.f)
{
	switch (eType)
	{
	case NATURAL_TREE:
		m_vOrginRot = Vec3(-XM_PI / 2.f, 0.f, 0.f);
		m_fHealth = 150.f;
		break;
	case NATURAL_STONE:
		m_vOrginRot = Vec3(-XM_PI / 2.f, 0.f, 0.f);
		m_fHealth = 150.f;
		break;
	case NATURAL_BUSH:
	case NATURAL_NONE:
		m_vOrginRot = Vec3(-XM_PI / 2.f, 0.f, 0.f);
		m_fHealth = 1.f;
		break;
	default:
		break;
	}
}

CNaturalScript::~CNaturalScript()
{
}

void CNaturalScript::Update()
{
	if (m_pParticleObj != NULL)
	{
		m_fParticleTime -= DT;

		if (m_fParticleTime < 0.f)
		{
			tEvent tEv;
			tEv.eType = EVENT_TYPE::DELETE_OBJECT;
			tEv.wParam = (DWORD_PTR)m_pParticleObj;
			CEventMgr::GetInst()->AddEvent(tEv);
			m_pParticleObj = NULL;
		}
	}

	if (!m_bDestory)
		return;


	switch (m_eType)
	{
	case NATURAL_TREE:
	{
		if (!m_bRotate)
		{
			m_fAngle += DT * 0.5f;
			if (m_fAngle >= XM_PI / 2.f)
			{
				m_bRotate = true;
				m_fTime = NATURAL_RESPAWN_TIME;
				m_fAngle = XM_PI / 2.f;

				tEvent evt = {};
				evt.eType = EVENT_TYPE::TRANSFER_LAYER;
				evt.wParam = (DWORD_PTR)GetObj();
				evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);
				CEventMgr::GetInst()->AddEvent(evt);
			}
			Vec3 vRot = m_vOrginRot + Vec3(0.f, m_vTargetRot.y, 0.f);
			vRot.x -= m_fAngle;
			Transform()->SetLocalRot(vRot);
		}
		else
		{
			m_fTime -= DT;
			if (m_fTime <= 0.f)
			{
				Respawn();
			}
		}
	}
		break;
	case NATURAL_STONE:
	case NATURAL_BUSH:
		{
			m_fTime -= DT;
			if (m_fTime <= 0.f)
			{
				Respawn();
			}
		}
		break;
	case NATURAL_NONE:
		break;
	default:
		break;
	}
}

void CNaturalScript::OnCollision(CCollider2D * _pOther)
{
}

void CNaturalScript::OnCollisionEnter(CCollider2D * _pOther)
{
}

void CNaturalScript::OnCollisionExit(CCollider2D * _pOther)
{
}

void CNaturalScript::SaveToScene( FILE * _pFile )
{
	fwrite( &m_eType, sizeof( NATURAL_TYPE ), 1, _pFile );
}

void CNaturalScript::LoadFromScene( FILE * _pFile )
{
	fread( &m_eType, sizeof( NATURAL_TYPE ), 1, _pFile );
}

bool CNaturalScript::Damage(CGameObject* pObj, float fDamage)
{
	if (m_eType == NATURAL_NONE)
		return false;

	m_fHealth -= fDamage;

	if (m_fHealth <= 0.f)
	{
		m_bDestory = true;
		m_fTime = NATURAL_RESPAWN_TIME;
		m_fAngle = 0.f;
		m_vTargetRot = pObj->Transform()->GetLocalRot();

		if (m_eType != NATURAL_TREE)
		{
			tEvent evt = {};
			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = (DWORD_PTR)GetObj();
			evt.lParam = ((DWORD_PTR)29 << 16 | (DWORD_PTR)true);
			CEventMgr::GetInst()->AddEvent(evt);
		}
	}

	if (m_pParticleObj == NULL)
	{
		// ====================
		// Particle Object »ý¼º
		// ====================
		m_pParticleObj = new CGameObject;
		m_pParticleObj->SetName(L"Particle");
		m_pParticleObj->AddComponent(new CTransform);
		m_pParticleObj->AddComponent(new CParticleSystem);

		m_pParticleObj->ParticleSystem()->SetStartColor(Vec4(1.f, 0.7f, 0.f, 1.f));
		m_pParticleObj->ParticleSystem()->SetEndColor(Vec4(0.5f, 0.1f, 0.f, 0.6f));

		m_pParticleObj->ParticleSystem()->SetStartSpeed(200.f);
		m_pParticleObj->ParticleSystem()->SetEndSpeed(200.f);

		Vec3 vPos = Transform()->GetLocalPos();
		Vec3 vScale = Transform()->GetLocalScale();

		m_pParticleObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
		m_pParticleObj->Transform()->SetLocalScale(Vec3(1.2f, 1.2f, 1.2f));

		GetObj()->AddChild(m_pParticleObj);

		tEvent tEv;
		tEv.eType = EVENT_TYPE::CREATE_OBJECT;
		tEv.wParam = (DWORD_PTR)m_pParticleObj;
		tEv.lParam = 3;
		CEventMgr::GetInst()->AddEvent(tEv);
	}
	m_fParticleTime = 1.f;

	return false;
}

bool CNaturalScript::GetDestroy()
{
	return m_bDestory;
}

NATURAL_TYPE CNaturalScript::GetNaturalType()
{
	return m_eType;
}

void CNaturalScript::Respawn()
{
	Transform()->SetLocalRot(m_vOrginRot);
	m_bDestory = false;
	m_bRotate = false;

	switch (m_eType)
	{
	case NATURAL_TREE:
		m_fHealth = 150.f;
		break;
	case NATURAL_STONE:
		m_fHealth = 150.f;
		break;
	case NATURAL_BUSH:
	case NATURAL_NONE:
		m_fHealth = 1.f;
		break;
	default:
		break;
	}

	tEvent evt = {};
	evt.eType = EVENT_TYPE::TRANSFER_LAYER;
	evt.wParam = (DWORD_PTR)GetObj();
	evt.lParam = ((DWORD_PTR)3 << 16 | (DWORD_PTR)true);
	CEventMgr::GetInst()->AddEvent(evt);
}
