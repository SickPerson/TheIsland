#include "stdafx.h"
#include "BuildScript.h"

CBuildScript::CBuildScript(HOUSING_TYPE eType) :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_bBuild(false),
	m_bCollision(false),
	m_eType(eType)
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
	if (_pOther->GetObj()->GetLayerIdx() != CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx())
		return;

	switch (m_eType)
	{
	case HOUSING_FOUNDATION:
	{
		if (_pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType() == HOUSING_FOUNDATION)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();
			Vec3 vOtherScale = _pOther->Transform()->GetLocalScale();

			Vec3 vDiff = vPos - vOtherPos;
			vDiff.Normalize(vDiff);
			// vOtherPos -> vPos

			Vec3 vOffset = Vec3(0.f, 0.f, 0.f);
			float fLean = vDiff.z / vDiff.x;

			if (fLean < 1.f && fLean > -1.f)
			{
				if (vDiff.x > 0.f)
					vOffset = Vec3(1.f, 0.f, 0.f);
				else
					vOffset = Vec3(-1.f, 0.f, 0.f);
			}
			else
			{
				if (vDiff.z > 0.f)
					vOffset = Vec3(0.f, 0.f, 1.f);
				else
					vOffset = Vec3(0.f, 0.f, -1.f);
			}

			vPos = vOtherPos + (vOffset * 400.f * vOtherScale);
			Transform()->SetLocalPos(vPos);
		}
	}
		break;
	case HOUSING_WALL:
		break;
	case HOUSING_DOOR:
		break;
	case HOUSING_WINDOW:
		break;
	case HOUSING_FLOOR:
		break;
	default:
		break;
	}
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

HOUSING_TYPE CBuildScript::GetHousingType()
{
	return m_eType;
}

Vec3 CBuildScript::GetOffsetScale()
{
	Vec3 vScale;
	switch (m_eType)
	{
	case HOUSING_FOUNDATION:
		vScale = Vec3(200.f, 200.f, 200.f);
		break;
	case HOUSING_WALL:
		break;
	case HOUSING_DOOR:
		break;
	case HOUSING_WINDOW:
		break;
	case HOUSING_FLOOR:
		break;
	default:
		break;
	}

	return Transform()->GetLocalScale() * vScale;
}
