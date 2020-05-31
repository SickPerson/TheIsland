#include "stdafx.h"
#include "MonsterScript.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::ANIMALSCRIPT)
	, m_iDir(1)
	, m_bUD(true)
{
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::Update()
{
	// Transform 월드 좌표정보 얻기
	Vec3 vPos = Transform()->GetLocalPos();

	if (m_bUD)
	{
		if (vPos.x > 600.f)
			m_iDir = -1;
		else if (vPos.x < -600.f)
			m_iDir = 1;

		vPos.x += DT * 100.f * m_iDir;
	}
	else
	{
		if (vPos.y > 600.f)
			m_iDir = -1;
		else if (vPos.y < -600.f)
			m_iDir = 1;

		vPos.y += DT * 100.f * m_iDir;
	}

	// 수정된 좌표를 다시 세팅하기.
	Transform()->SetLocalPos(vPos);
}

void CMonsterScript::OnCollisionEnter(CCollider2D * _pOther)
{
	// 충돌이 발생하고, 상대 물체가 총일이면 스스로를 삭제
	if (L"Bullet Object" == _pOther->GetObj()->GetName())
	{		
		DeleteObject(GetObj());	// -->삭제 이벤트 등록	
	}	
}

void CMonsterScript::OnCollisionExit(CCollider2D * _pOther)
{	
}

void CMonsterScript::SetUpDown(bool bUD)
{
	m_bUD = bUD;
}
