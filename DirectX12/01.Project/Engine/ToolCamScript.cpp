#include "stdafx.h"
#include "ToolCamScript.h"

#include "Camera.h"

#include <iostream>

CToolCamScript::CToolCamScript()
	: CScript( ( UINT )SCRIPT_TYPE::WORLDSCRIPT )
	, m_fSpeed( 200.f )
	, m_fScaleSpeed( 1.f )
	, m_bTool( false )
	, m_fMaxY(1000.f)
{
}

CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::SetTool( bool bTool )
{
	m_bTool = bTool;
}

void CToolCamScript::SetMaxY( float fMaxY )
{
	m_fMaxY = fMaxY;
}

void CToolCamScript::Update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();
	float fSpeed = m_fSpeed;

	if ( KEY_HOLD( KEY_TYPE::KEY_LSHIFT ) )
	{
		fSpeed *= 5.f;
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_W ) )
	{
		if ( !m_bTool )
		{
			Vec3 vFront = Transform()->GetWorldDir( DIR_TYPE::FRONT );
			vPos += vFront * fSpeed * DT;
		}

		else
		{
			Vec3 vFront = Transform()->GetWorldDir( DIR_TYPE::UP );
			vPos += vFront * fSpeed * DT;
		}
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_S ) )
	{
		if ( !m_bTool )
		{
			Vec3 vBack = -Transform()->GetWorldDir( DIR_TYPE::FRONT );
			vPos += vBack * fSpeed * DT;
		}

		else
		{
			Vec3 vBack = -Transform()->GetWorldDir( DIR_TYPE::UP );
			vPos += vBack * fSpeed * DT;
		}
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_A ) )
	{
		Vec3 vLeft = -Transform()->GetWorldDir( DIR_TYPE::RIGHT );
		vPos += vLeft * fSpeed * DT;
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_D ) )
	{
		Vec3 vRight = Transform()->GetWorldDir( DIR_TYPE::RIGHT );
		vPos += vRight * fSpeed * DT;
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_1 ) )
	{
		fScale -= m_fScaleSpeed * DT;
		Camera()->SetScale( fScale );
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_2 ) )
	{
		fScale += m_fScaleSpeed * DT;
		Camera()->SetScale( fScale );
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_RBTN ) )
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot( vRot );
	}

	if ( KEY_AWAY( KEY_TYPE::KEY_RBTN ) )
	{
		if ( m_bTool )
		{
			vPos.y = m_fMaxY;
			Transform()->SetLocalRot( Vec3( XM_PI / 2, 0.f, 0.f ) );
		}
	}

	Transform()->SetLocalPos( vPos );
}
