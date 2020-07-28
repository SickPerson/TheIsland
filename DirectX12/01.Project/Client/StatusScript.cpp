#include "stdafx.h"
#include "StatusScript.h"

#include "GameOverScript.h"

CStatusScript::CStatusScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_fHealth(100.f), m_fHungry(100.f), m_fThirst(100.f),
	m_bGameOver(false)
{
}


CStatusScript::~CStatusScript()
{
}

void CStatusScript::Update()
{
	//if (m_fHealth <= 0.f)
	//	m_fHealth = 100.f;

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
	{
		Damage(20.f * DT);
	}
	//m_fHealth -= 10 * DT;
	//if (m_fHealth < 0.f)
	//	m_fHealth = 100.f;
	m_fHungry -= 15 * DT;
	if (m_fHungry < 0.f)
		m_fHungry = 100.f;
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
	if (m_fHealth <= 0.f && m_bGameOver == false)
	{
		CGameObject* pObject = new CGameObject;
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CGameOverScript);
		pObject->SetName(L"GameOver");
		pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		pObject->GetScript<CGameOverScript>()->Init();
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_bGameOver = true;
		ShowCursor(true);
	}
}

void CStatusScript::SetIncreaseHealth(float fAmount)
{
	m_fHealth += fAmount;
	if (m_fHealth > 100.f)
		m_fHealth = 100.f;
}

void CStatusScript::SetIncreaseHungry(float fAmount)
{
	m_fHungry += fAmount;
	if (m_fHungry > 100.f)
		m_fHungry = 100.f;
}

void CStatusScript::SetIncreasefThirst(float fAmount)
{
	m_fThirst += fAmount;
	if (m_fThirst > 100.f)
		m_fThirst = 100.f;
}

bool CStatusScript::GetGameOver()
{
	return m_bGameOver;
}
