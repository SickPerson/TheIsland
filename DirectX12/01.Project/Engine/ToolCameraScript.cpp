#include "stdafx.h"
#include "ToolCameraScript.h"

#include "Camera.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Transform.h"

CToolCameraScript::CToolCameraScript() : 
	CScript(0),
	m_fSpeed(200.f),
	m_fScaleSpeed(1.f)
{
}


CToolCameraScript::~CToolCameraScript()
{
}

void CToolCameraScript::Update()
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
		Vec3 vFront = Transform()->GetWorldDir( DIR_TYPE::FRONT );
		vPos += vFront * fSpeed * DT;
	}

	if ( KEY_HOLD( KEY_TYPE::KEY_S ) )
	{
		Vec3 vBack = -Transform()->GetWorldDir( DIR_TYPE::FRONT );
		vPos += vBack * fSpeed * DT;
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
		Vec2 vDrag = GET_SINGLE(CKeyManager)->GetDragDir();
		Vec3 vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 3.f;
		vRot.y += vDrag.x * DT * 1.5f;

		Transform()->SetLocalRot( vRot );
	}

	Transform()->SetLocalPos( vPos );
}
