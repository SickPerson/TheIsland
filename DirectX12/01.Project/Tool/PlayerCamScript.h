#pragma once
#include <Engine/Script.h>

class CPlayerCamScript :
	public CScript
{
	CGameObject* m_pPlayer;
public:
	CPlayerCamScript();
	virtual ~CPlayerCamScript();

	CLONE(CPlayerCamScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

public:
	void SetPlayer(CGameObject* _pPlayer) {
		m_pPlayer = _pPlayer;
	}
};

