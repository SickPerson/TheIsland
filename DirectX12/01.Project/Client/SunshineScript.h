#pragma once
#include <Engine/Script.h>

class CSunshineScript :
	public CScript
{
	float DAYCYCLE;
	float m_fTime;
	int m_iDay;
	int m_iHour;
	int m_iMinute;
	float m_fDarkness;

	float m_fRainTime;
	bool	m_bRain;

	CGameObject* m_pSkybox;
	CGameObject* m_pSea;

	CGameObject* m_pPlayer;
	CGameObject* m_pClock;
	CGameObject* m_pDay;

	CGameObject* m_pRain;

	// 임시
	float time = 1.f;

public:
	CSunshineScript();
	virtual ~CSunshineScript();

	CLONE(CSunshineScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

public:
	void Init();

	float GetTime();
	void SetTime(int iHour, int iMin);

	void SetDayCycle(float fCycle);

	void SetSkybox(CGameObject* pObject);
	void SetPlayer(CGameObject* pObject);
	void SetSea(CGameObject* pObject);
	void SetRain( CGameObject* pObject );

	void SetRainDrop( bool bRain );
};

