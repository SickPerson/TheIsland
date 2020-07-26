#include "stdafx.h"
#include "BuildScript.h"

CBuildScript::CBuildScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_bBuild(false),
	m_bCollision(false)
{
}


CBuildScript::~CBuildScript()
{
}

void CBuildScript::Update()
{
}

void CBuildScript::OnCollision(CCollider2D * _pOther)
{
}

void CBuildScript::OnCollisionEnter(CCollider2D * _pOther)
{
	m_bCollision = true;
	int test = -2;
	for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
	{
		MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
	}
}

void CBuildScript::OnCollisionExit(CCollider2D * _pOther)
{
	m_bCollision = false;
	int test = -1;
	for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
	{
		MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
	}
}

void CBuildScript::Init()
{
	int test = -1;
	for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
	{
		MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
	}
}

bool CBuildScript::Build()
{
	if (m_bCollision)
		return false;

	//int test = 0;
	for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
	{
		MeshRender()->GetCloneMaterial(i)->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
		//SetData(SHADER_PARAM::INT_3, &test);
	}
	return true;
}
