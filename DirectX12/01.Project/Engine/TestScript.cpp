#include "stdafx.h"
#include "TestScript.h"

CTestScript::CTestScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_iLifeCount(2),
	m_pTestObject(nullptr)
{
}

CTestScript::~CTestScript()
{
}

void CTestScript::Update()
{
	Vec3 vPos = m_pTestObject->Transform()->GetLocalPos();
	Vec3 vRot = m_pTestObject->Transform()->GetLocalRot();

	/*static float angle = 0.f;
	angle += 0.1f;
	vPos.x = vPos.x + cosf(XMConvertToRadians(angle)) * 150.f;
	vPos.z = vPos.z + sinf(XMConvertToRadians(angle)) * 150.f;*/

	vPos.x = vPos.x + cosf(XMConvertToRadians(vRot.y + 90.f)) * 150.f;
	vPos.z = vPos.z + sinf(XMConvertToRadians(vRot.y + 90.f)) * 150.f;

	vPos.y = 25.f;

	//vPos.z += 150.f;

	Transform()->SetLocalPos(vPos);
	//Transform()->SetLocalRot(Vec3(0.f, vRot.y + 180.f, 0.f));
}

void CTestScript::OnCollisionEnter(CCollider2D * _pOther)
{
	// DeleteObject(GetObj());	
	// AddChild(_pOther->GetObj());
	// ClearParent();
}

void CTestScript::OnCollisionExit(CCollider2D * _pOther)
{
	int a = 1;
}
