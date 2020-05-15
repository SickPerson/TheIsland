#include "stdafx.h"
#include "SunshineScript.h"

#include <Engine/TimeMgr.h>
#include <Engine/Light3D.h>

#define DAYCYCLE 10000.f // 배속
// 아무리 빨라도 1프레임당 1초가 최대

CSunshineScript::CSunshineScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_fTime(0.f),
	m_fDarkness(0.f),
	m_iDay(0),
	m_iHour(8),
	m_iMinute(0),
	m_pSkybox(NULL)
{
}


CSunshineScript::~CSunshineScript()
{
}

void CSunshineScript::Update()
{
	m_fTime += DT * DAYCYCLE;
	if (m_fTime > 60.f)
	{
		m_fTime = 0.f;
		m_iMinute++;
		if (m_iMinute >= 60)
		{
			m_iHour++;
			m_iMinute = 0;
			if (m_iHour >= 24)
			{
				m_iHour = 0.f;
				m_iDay++;
			}
		}
	}
	//std::cout << "Day : " << m_iDay << " | " << m_iHour << " : " << m_iMinute << " : " << m_fTime << std::endl;

	CLight3D* pLight = Light3D();
	Vec3 vDir = Vec3(0.f, 0.f, 0.f);

	if (m_iHour > 18)
	{
		vDir = Vec3(0.f, 0.f, 0.f);
		Light3D()->SetDiffuseColor(Vec3(0.f, 0.f, 0.f));
		float fLight = 0.0f;
		if (m_pSkybox)
		{
			m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
		}
	}
	else if (m_iHour < 6)
	{
		vDir = Vec3(0.f, 0.f, 0.f);
		Light3D()->SetDiffuseColor(Vec3(0.f, 0.f, 0.f));
		float fLight = 0.0f;
		if (m_pSkybox)
		{
			m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
		}
	}
	else
	{
		float fTime = (float)(m_iHour);
		fTime += ((float)(m_iMinute) / 60.f * 100.f) / 100.f;
		vDir.y += cosf((fTime * 15.f) * XM_PI / 180.f);
		vDir.x += sinf((fTime * 15.f) * XM_PI / 180.f);
		float fLight = 0.f;
		if (m_iHour < 12)
		{
			fLight = (float)(fTime - 6.f) / 6.f;
			if (m_pSkybox)
			{
				m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
			}
		}
		else 
		{
			fLight = (float)(18 - fTime) / 6.f;
			if (m_pSkybox)
			{
				m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
			}
		}
		Light3D()->SetDiffuseColor(Vec3(fLight, fLight, fLight));
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
