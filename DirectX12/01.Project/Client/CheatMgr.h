#pragma once
class CCheatMgr
{
	SINGLE(CCheatMgr);
private:
	CGameObject* m_pClock;
	CGameObject* m_pPlayer;

public:
	void Init();

	void SetPlayer(CGameObject* pObject);
	void SetClock(CGameObject* pObject);

	bool CheckCheat(string strText);
};

