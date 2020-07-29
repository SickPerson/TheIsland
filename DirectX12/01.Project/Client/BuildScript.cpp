#include "stdafx.h"
#include "BuildScript.h"

CBuildScript::CBuildScript(HOUSING_TYPE eType) :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_bBuild(false),
	m_eType(eType)
{
	switch (eType)
	{
	case HOUSING_FOUNDATION:
		m_bCollision = false;
		break;
	case HOUSING_WALL:
	case HOUSING_DOOR:
	case HOUSING_WINDOW:
	case HOUSING_FLOOR:
	{
		m_bCollision = true;
	}
		break;
	default:
		m_bCollision = false;
		break;
	}
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

	HOUSING_TYPE eType = _pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType();
	switch (m_eType)
	{
	case HOUSING_FOUNDATION:
	{
		if (eType == HOUSING_FOUNDATION)
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
	case HOUSING_DOOR:
	case HOUSING_WINDOW:
		if (eType == HOUSING_FOUNDATION)
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
				{
					vOffset = Vec3(1.f, 0.f, 0.f);
					Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
				}
				else
				{
					vOffset = Vec3(-1.f, 0.f, 0.f);
					Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
				}
			}
			else
			{
				if (vDiff.z > 0.f)
				{
					vOffset = Vec3(0.f, 0.f, 1.f);
					Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, -XM_PI / 2.f, 0.f));
				}
				else
				{
					vOffset = Vec3(0.f, 0.f, -1.f);
					Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, -XM_PI / 2.f, 0.f));
				}
			}

			vPos = vOtherPos + (vOffset * 190.f * vOtherScale);
			Transform()->SetLocalPos(vPos);

			m_bCollision = false;
			int test = -1;
			for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
			{
				MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
			}
		}
		else if (eType > HOUSING_FOUNDATION && eType < HOUSING_END)
		{

		}
		else
		{
			m_bCollision = true;
			int test = -2;
			for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
			{
				MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
			}
		}
		break;
	case HOUSING_FLOOR:
		if (eType > HOUSING_FOUNDATION && eType < HOUSING_FLOOR)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();
			Vec3 vOtherScale = _pOther->Transform()->GetLocalScale();

			Vec3 vDiff = vPos - vOtherPos;
			vDiff.Normalize(vDiff);
			// vOtherPos -> vPos

			Vec3 vOffset = Vec3(0.f, 0.f, 0.f);
			Vec3 vRot = _pOther->Transform()->GetLocalRot();

			if (vRot.y != 0.f)
			{
				if (vDiff.z > 0.f)
				{
					vOffset = Vec3(0.f, 0.f, 1.f);
				}
				else
				{
					vOffset = Vec3(0.f, 0.f, -1.f);
				}
			}
			else
			{
				if (vDiff.x > 0.f)
				{
					vOffset = Vec3(1.f, 0.f, 0.f);
				}
				else
				{
					vOffset = Vec3(-1.f, 0.f, 0.f);
				}
			}

			vPos = vOtherPos + (vOffset * 200.f * vOtherScale);
			vPos.y += 240.f;
			Transform()->SetLocalPos(vPos);

			m_bCollision = false;
			int test = -1;
			for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
			{
				MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
			}
		}
		else if (eType == HOUSING_FOUNDATION)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

			vPos = vOtherPos;
			vPos.y += 240.f;
			Transform()->SetLocalPos(vPos);

			m_bCollision = false;
			int test = -1;
			for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
			{
				MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
			}
		}
		else
		{
			/*m_bCollision = true;
			int test = -2;
			for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
			{
				MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
			}*/
		}
		break;
	case HOUSING_ETC:
		if (eType == HOUSING_FOUNDATION)
		{
			Vec3 vPos = Transform()->GetLocalPos();
			Vec3 vOtherPos = _pOther->Transform()->GetLocalPos();

			vPos.y = vOtherPos.y;

			Transform()->SetLocalPos(vPos);
		}
		break;
	default:
		break;
	}
}

void CBuildScript::OnCollisionEnter(CCollider2D * _pOther)
{
	switch (m_eType)
	{
	case HOUSING_FOUNDATION:
	{
		m_bCollision = true;
		int test = -2;
		for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
		{
			MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
		}
	}
		break;
	case HOUSING_WALL:
	case HOUSING_DOOR:
	case HOUSING_WINDOW:
	{
		if (_pOther->GetObj()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx())
		{
			return;
		}
	}
		break;
	case HOUSING_FLOOR:
	{
		if (_pOther->GetObj()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx())
		{
			return;
		}
	}
		break;
	default:
		break;
	}
}

void CBuildScript::OnCollisionExit(CCollider2D * _pOther)
{
	switch (m_eType)
	{
	case HOUSING_FOUNDATION:
	{
		m_bCollision = false;
		int test = -1;
		for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
		{
			MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
		}
	}
		break;
	case HOUSING_WALL:
	case HOUSING_DOOR:
	case HOUSING_WINDOW:
	{
		if (_pOther->GetObj()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx())
		{
			if (_pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType() == HOUSING_FOUNDATION)
			{
				m_bCollision = true;
				int test = -2;
				for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
				{
					MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
				}
			}
			return;
		}
	}
	break;
	case HOUSING_FLOOR:
	{
		if (_pOther->GetObj()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"House")->GetLayerIdx())
		{
			if (_pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType() > HOUSING_FOUNDATION &&
				_pOther->GetObj()->GetScript<CBuildScript>()->GetHousingType() < HOUSING_END)
			{
				m_bCollision = true;
				int test = -2;
				for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
				{
					MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
				}
			}
			return;
		}
	}
		break;
	default:
		break;
	}
}

void CBuildScript::Init()
{
	if (m_bCollision)
	{
		int test = -2;
		for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
		{
			MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
		}
	}
	else
	{
		int test = -1;
		for (int i = 0; i < MeshRender()->GetMaterialCount(); ++i)
		{
			MeshRender()->GetSharedMaterial(i)->SetData(SHADER_PARAM::INT_3, &test);
		}
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
	case HOUSING_DOOR:
	case HOUSING_WINDOW:
		vScale = Vec3(20.f, 200.f, 200.f);
		break;
	case HOUSING_FLOOR:
		vScale = Vec3(200.f, 20.f, 200.f);
		break;
	default:
		break;
	}

	return vScale;
}
