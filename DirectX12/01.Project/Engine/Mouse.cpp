#include "stdafx.h"
#include "Mouse.h"

#include "KeyMgr.h"
#include "Transform.h"

CMouse::CMouse()
	: CComponent( COMPONENT_TYPE::MOUSE )
{
}


CMouse::~CMouse()
{
}

void CMouse::Update()
{
	POINT tPt = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vPos = Vec3( tPt.x, tPt.y, 0.f );

	Vec3 vLocalPos = Transform()->GetLocalPos();
	vLocalPos += vPos;
	Transform()->SetLocalPos(vPos);
}

void CMouse::SaveToScene( FILE * _pFile )
{
}

void CMouse::LoadFromScene( FILE * _pFile )
{
}
