#pragma once
class CCheatMgr
{
	SINGLE(CCheatMgr);
private:
	CGameObject* m_pClock;
	CGameObject* m_pPlayer;

	CGameObject* m_pSpawner[3];

	bool m_bInvincible = false;

public:
	void Init();

	void SetPlayer(CGameObject* pObject);
	void SetClock(CGameObject* pObject);
	void SetSpawner(CGameObject* pObject1, CGameObject* pObject2, CGameObject* pObject3);

	bool CheckCheat(string strText);
};

