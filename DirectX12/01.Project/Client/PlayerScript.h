#pragma once
#include <Engine/Script.h>

class CCamera;

class CPlayerScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;

	CGameObject* m_pChat;
	CGameObject* m_pInventory;

	bool		m_bEnable;
	
public:
	virtual void Awake();	
	virtual void Update();

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();

public:
	void SetChatObject(CGameObject* pObj);
	void SetInventoryObject(CGameObject* pObj);

	bool GetEnable();
};

