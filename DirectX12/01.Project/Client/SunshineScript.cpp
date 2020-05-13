#include "stdafx.h"
#include "SunshineScript.h"

#include <Engine/TimeMgr.h>
#include <Engine/Light3D.h>

#define DAYCYCLE 1.f

CSunshineScript::CSunshineScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_fTime(0.f),
	m_fDarkness(0.f),
	m_pSkybox(NULL)
{
}


CSunshineScript::~CSunshineScript()
{
}

void CSunshineScript::Update()
{
	m_fTime += DT * DAYCYCLE;
	
	CLight3D* pLight = Light3D();
	Vec3 vDir = Vec3(0.f, 0.f, 1.f);

	vDir.y += cosf(m_fTime);
	vDir.x += sinf(m_fTime);

	if (m_pSkybox)
	{
		m_fDarkness += DT;
		if (m_fDarkness > 1.f)
			m_fDarkness = 0.f;

		m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_fDarkness);
	}

	pLight->SetLightDir(vDir);
}

float CSunshineScript::GetTime()
{
	return m_fTime;
}

void CSunshineScript::SetTime(float fTime)
{
	m_fTime = fTime;
}

void CSunshineScript::SetSkybox(CGameObject* pObject)
{
	m_pSkybox = pObject;
}
