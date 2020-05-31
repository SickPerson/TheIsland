#pragma once
#include <Engine/Script.h>

class CFPSCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;

	CGameObject* m_pPlayer;
public:
	virtual void Update();

public:
	CLONE(CFPSCamScript);

public:
	CFPSCamScript();
	virtual ~CFPSCamScript();

	void SetPlayer(CGameObject* pPlayer);
};
