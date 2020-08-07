#pragma once
#include <Engine/Script.h>

class CSunshineScript :
	public CScript
{
	float m_fTime;
	int m_iDay;
	int m_iHour;
	int m_iMinute;
	float m_fDarkness;

	CGameObject* m_pSkybox;
	CGameObject* m_pSea;

	CGameObject* m_pPlayer;
	CGameObject* m_pClock;
	CGameObject* m_pDay;
public:
	CSunshineScript();
	virtual ~CSunshineScript();

	CLONE(CSunshineScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

public:
	void Init();

	float GetTime();
	void SetTime(float fTime);

	void SetSkybox(CGameObject* pObject);
	void SetPlayer(CGameObject* pObject);
	void SetSea(CGameObject* pObject);
};

