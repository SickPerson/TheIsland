#include "stdafx.h"
#include "StatusScript.h"


CStatusScript::CStatusScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_fHealth(100.f), m_fHungry(100.f), m_fThirst(100.f)
{
}


CStatusScript::~CStatusScript()
{
}

void CStatusScript::Update()
{
	if (m_fHealth <= 0.f)
		m_fHealth = 100.f;

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
	{
		m_fHealth -= 20.f * DT;
	}
	/*m_fHealth -= 10 * DT;
	if (m_fHealth < 0.f)
		m_fHealth = 100.f;
	m_fHungry -= 15 * DT;
	if (m_fHungry < 0.f)
		m_fHungry = 100.f;*/
	m_fThirst -= 20 * DT;
	if (m_fThirst < 0.f)
		m_fThirst = 100.f;

	const vector<CGameObject *>& vecObj = GetObj()->GetChild();

	vecObj[0]->Transform()->SetLocalPos(Vec3(-0.46f + 0.46f * m_fHealth / 100.f, 0.3f, -100.f));
	vecObj[0]->Transform()->SetLocalScale(Vec3(230.f / 250.f * m_fHealth / 100.f, 30.f / 135.f, 1.f));

	vecObj[1]->Transform()->SetLocalPos(Vec3(-0.46f + 0.46f * m_fHungry / 100.f, 0.f, -100.f));
	vecObj[1]->Transform()->SetLocalScale(Vec3(230.f / 250.f * m_fHungry / 100.f, 30.f / 135.f, 1.f));

	vecObj[2]->Transform()->SetLocalPos(Vec3(-0.46f + 0.46f * m_fThirst / 100.f, -0.3f, -100.f));
	vecObj[2]->Transform()->SetLocalScale(Vec3(230.f / 250.f * m_fThirst / 100.f, 30.f / 135.f, 1.f));

	float health = 1.f - (m_fHealth / 100.f);
	vecObj[3]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &health);
}

void CStatusScript::Damage(float fDamage)
{
	m_fHealth -= fDamage;
}
