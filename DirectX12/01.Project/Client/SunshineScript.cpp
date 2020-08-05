#include "stdafx.h"
#include "SunshineScript.h"

#include <Engine/TimeMgr.h>
#include <Engine/Light3D.h>
#include <Engine/NaviMgr.h>
#include <Engine/Font.h>
#include <Engine/RenderMgr.h>

#include <iostream>

#define DAYCYCLE 10.f // 배속
// 아무리 빨라도 1프레임당 1초가 최대

CSunshineScript::CSunshineScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_fTime(0.f),
	m_fDarkness(0.f),
	m_iDay(0),
	m_iHour(11),
	m_iMinute(0),
	m_pSkybox(NULL),
	m_pPlayer(NULL)
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
				string strDay = "Day : " + std::to_string(m_iDay);
				m_pDay->Font()->SetString(strDay);
			}
		}
		string strTime = std::to_string(m_iHour) + " : " + std::to_string(m_iMinute);
		m_pClock->Font()->SetString(strTime);
	}
	//std::cout << "Day : " << m_iDay << " | " << m_iHour << " : " << m_iMinute << " : " << m_fTime << std::endl;

	CLight3D* pLight = Light3D();
	Vec3 vDir = Vec3(1.f, -1.f, 1.f);
	Vec3 vPos = Vec3(0.f, 0.f, 0.f);

	//if (m_iHour > 18)
	//{
	//	vDir = Vec3(0.f, 0.f, 0.f);
	//	vPos = Vec3(0.f, 6000.f, 0.f);
	//	Light3D()->SetDiffuseColor(Vec3(0.2f, 0.2f, 0.2f));
	//	float fLight = 0.0f;
	//	if (m_pSkybox)
	//	{
	//		m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
	//	}
	//}
	//else if (m_iHour < 6)
	//{
	//	vDir = Vec3(0.f, 0.f, 0.f);
	//	vPos = Vec3(0.f, 6000.f, 0.f);
	//	Light3D()->SetDiffuseColor(Vec3(0.2f, 0.2f, 0.2f));
	//	float fLight = 0.0f;
	//	if (m_pSkybox)
	//	{
	//		m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
	//	}
	//}
	//else
	{
		float fTime = (float)(m_iHour);
		//fTime += 3.f;
		//if (fTime > 24.f)
		//	fTime -= 21.f;

		fTime += ((float)(m_iMinute) / 60.f * 100.f) / 100.f;
		vDir.y = cosf((fTime * 15.f) * XM_PI / 180.f);
		vDir.x = sinf((fTime * 15.f) * XM_PI / 180.f);
		vDir.Normalize(-vDir);
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
	//std::cout << vDir.x << " | " << m_iHour << " : " << vDir.y << " : " << vDir.z << std::endl;

	//pLight->SetLightDir(vDir);
	//Transform()->SetLocalPos(vPos);
	//Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));

	Vec3 vPlayerPos = m_pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerRot = m_pPlayer->Transform()->GetLocalRot();
	Vec3 vPlayerDir = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;

	Vec3 vShadowPos = vPlayerPos + (vPlayerDir * 500.f);
	vShadowPos.y = CNaviMgr::GetInst()->GetY(vShadowPos);
	vShadowPos += vDir * -3000.f;

	//vPlayerPos += vPlayerDir * Vec3(-1000.f, 1000.f, -1000.f);
	Transform()->SetLocalPos(vShadowPos);
	Light3D()->SetLightDir(vDir);

	//Transform()->SetLocalRot(Vec3(vPlayerRot.x + XM_PI / 2.f, vPlayerRot.y + XM_PI, vPlayerRot.z));
}

void CSunshineScript::Init()
{
	tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

	m_pClock = new CGameObject;
	m_pClock->AddComponent(new CTransform);
	m_pClock->AddComponent(new CFont);
	m_pClock->Transform()->SetLocalPos(Vec3(vResolution.fWidth / 2.f - 100.f, vResolution.fHeight / 2.f - 25.f, 1.f));
	m_pClock->Transform()->SetLocalScale(Vec3(200.f, 50.f, 1.f));

	string strTime = std::to_string(m_iHour) + " : " + std::to_string(m_iMinute);
	m_pClock->Font()->SetString(strTime);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pClock);

	m_pDay = new CGameObject;
	m_pDay->AddComponent(new CTransform);
	m_pDay->AddComponent(new CFont);
	m_pDay->Transform()->SetLocalPos(Vec3(vResolution.fWidth / 2.f - 270.f, vResolution.fHeight / 2.f - 25.f, 1.f));
	m_pDay->Transform()->SetLocalScale(Vec3(100.f, 50.f, 1.f));
	m_pDay->Font()->SetFontColor(Vec4(1.f, 0.5f, 0.2f, 1.f));

	string strDay = "Day : " + std::to_string(m_iDay);
	m_pDay->Font()->SetString(strDay);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pDay);
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

void CSunshineScript::SetPlayer(CGameObject * pObject)
{
	m_pPlayer = pObject;
}
