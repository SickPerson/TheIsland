#include "stdafx.h"
#include "SunshineScript.h"

#include <Engine/TimeMgr.h>
#include <Engine/Light3D.h>
#include <Engine/NaviMgr.h>
#include <Engine/Font.h>
#include <Engine/RenderMgr.h>
#include <Engine/PlayerScript.h>
#include <Engine/ParticleSystem.h>

#include <iostream>
#include "Network.h"

CSunshineScript::CSunshineScript() :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_fTime(0.f),
	m_fDarkness(0.f),
	m_iDay(0),
	m_iHour(11),
	m_iMinute(0),
	m_pSkybox(NULL),
	m_pPlayer(NULL),
	DAYCYCLE(60.f),
	m_pRain(NULL),
	m_fRainTime(3600.f),
	m_bRain(false)
{
}


CSunshineScript::~CSunshineScript()
{
}

void CSunshineScript::Update()
{
#ifdef NETWORK_ON
	m_fTime = CNetwork::GetInst()->GetTime();

	int  iTime = ( int )m_fTime % 60;

	if ( iTime == 0 && m_fTime > 1.f)
	{
		m_iMinute++;

		if ( m_iMinute >= 60 )
		{
			m_iHour++;
			m_iMinute = 0;
			if ( m_iHour >= 24 )
			{
				m_iHour = 0.f;
				m_iDay++;
				string strDay = "Day : " + std::to_string( m_iDay );
				m_pDay->Font()->SetString( strDay );
			}
		}
		string strTime = std::to_string( m_iHour ) + " : " + std::to_string( m_iMinute );
		m_pClock->Font()->SetString( strTime );
	}

#else
	m_fTime += DT * DAYCYCLE;
	if ( m_fTime > 60.f )
	{
		m_iMinute++;
		m_fTime = 0.f;
		if ( m_iMinute >= 60 )
		{
			m_iHour++;
			m_iMinute = 0;
			if ( m_iHour >= 24 )
			{
				m_iHour = 0.f;
				m_iDay++;
				string strDay = "Day : " + std::to_string( m_iDay );
				m_pDay->Font()->SetString( strDay );
			}
		}
		string strTime = std::to_string( m_iHour ) + " : " + std::to_string( m_iMinute );
		m_pClock->Font()->SetString( strTime );
	}
#endif	
	

	/*if ( !m_bRain )
	{
		if ( m_iDay % 4 == 1 )
		{
			m_bRain = true;

			tEvent evt = {};

			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = ( DWORD_PTR )m_pRain;
			evt.lParam = ( ( DWORD_PTR )0 << 16 | ( DWORD_PTR )true );

			CEventMgr::GetInst()->AddEvent( evt );
		}
	}

	else
	{
		m_fRainTime -= DT * DAYCYCLE;

		if ( m_fRainTime <= 0 )
		{
			m_bRain = false;
			m_fRainTime = 3600.f;

			tEvent evt = {};

			evt.eType = EVENT_TYPE::TRANSFER_LAYER;
			evt.wParam = ( DWORD_PTR )m_pRain;
			evt.lParam = ( ( DWORD_PTR )29 << 16 | ( DWORD_PTR )true );

			CEventMgr::GetInst()->AddEvent( evt );
		}
	}

	m_pRain->SetActive( m_bRain );
	*/


	CLight3D* pLight = Light3D();
	Vec3 vDir = Vec3(1.f, -1.f, 1.f);
	Vec3 vPos = Vec3(0.f, 0.f, 0.f);

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

		// 5½Ã ~ 20½Ã
		if (m_iHour <= 20 && m_iHour >= 5)
		{
			if(m_iHour <= 12)
				fLight = (float)(fTime - 5.f) / 7.f;
			else
				fLight = (float)(fTime - 20.f) / -7.f;
			fLight += 0.3f;
			if (m_pSkybox)
			{
				m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
			}
			if (m_pSea)
			{
				m_pSea->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_3, &fLight);
			}
		}
		else 
		{
			fLight = 0.05f;
			if (m_pSkybox)
			{
				m_pSkybox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fLight);
			}
			if (m_pSea)
			{
				m_pSea->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_3, &fLight);
			}
		}
		Light3D()->SetDiffuseColor(Vec3(fLight, fLight, fLight));
	}

	Vec3 vPlayerPos = m_pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerRot = m_pPlayer->Transform()->GetLocalRot();
	Vec3 vPlayerDir = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;

	Vec3 vShadowPos = vPlayerPos + (vPlayerDir * 500.f);
	vShadowPos.y = CNaviMgr::GetInst()->GetY(vShadowPos);
	vShadowPos += vDir * -3000.f;

	Light3D()->SetLightPos(vShadowPos);
	Light3D()->SetLightDir(vDir);

	// Rain
	if ( !m_pRain )
		return;
	CGameObject* pMainCam = CSceneMgr::GetInst()->GetCurScene()->GetLayer(0)->GetMainCamera();
	Vec3 vMainCam = pMainCam->Transform()->GetLocalPos();
	Vec3 vRain = m_pRain->Transform()->GetLocalPos();
	vRain.x = vMainCam.x;
	vRain.z = vMainCam.z;
	m_pRain->Transform()->SetLocalPos(vRain);
}

void CSunshineScript::Init()
{
	tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

	m_pClock = new CGameObject;
	m_pClock->AddComponent(new CTransform);
	m_pClock->AddComponent(new CFont);
	m_pClock->Transform()->SetLocalPos(Vec3(vResolution.fWidth / 2.f - 100.f, vResolution.fHeight / 2.f - 25.f, 500.f));
	m_pClock->Transform()->SetLocalScale(Vec3(200.f, 50.f, 1.f));

	string strTime = std::to_string(m_iHour) + " : " + std::to_string(m_iMinute);
	m_pClock->Font()->SetString(strTime);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pClock);

	m_pDay = new CGameObject;
	m_pDay->AddComponent(new CTransform);
	m_pDay->AddComponent(new CFont);
	m_pDay->Transform()->SetLocalPos(Vec3(vResolution.fWidth / 2.f - 270.f, vResolution.fHeight / 2.f - 25.f, 500.f));
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

void CSunshineScript::SetTime(int iHour, int iMin)
{
	m_iHour = iHour;
	m_iMinute = iMin;
	DAYCYCLE = 60.f;
}

void CSunshineScript::SetDayCycle(float fCycle)
{
	DAYCYCLE = fCycle;
}

void CSunshineScript::SetSkybox(CGameObject* pObject)
{
	m_pSkybox = pObject;
}

void CSunshineScript::SetPlayer(CGameObject * pObject)
{
	m_pPlayer = pObject;
}

void CSunshineScript::SetSea(CGameObject* pObject)
{
	m_pSea = pObject;
}

void CSunshineScript::SetRain( CGameObject * pObject )
{
	m_pRain = pObject;
}

void CSunshineScript::SetRainDrop( bool bRain )
{
	if ( !bRain )
	{
		tEvent evt = {};

		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = ( DWORD_PTR )m_pRain;
		evt.lParam = ( ( DWORD_PTR )0 << 16 | ( DWORD_PTR )true );

		CEventMgr::GetInst()->AddEvent( evt );
	}

	else
	{
		tEvent evt = {};

		evt.eType = EVENT_TYPE::TRANSFER_LAYER;
		evt.wParam = ( DWORD_PTR )m_pRain;
		evt.lParam = ( ( DWORD_PTR )29 << 16 | ( DWORD_PTR )true );

		CEventMgr::GetInst()->AddEvent( evt );
	}

	m_pRain->SetActive( bRain );
}
