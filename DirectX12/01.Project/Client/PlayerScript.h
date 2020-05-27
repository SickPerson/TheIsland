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
	CGameObject* m_pStatus;

	bool		m_bEnable;

	bool m_bInvincible;
	
public:
	virtual void Awake();	
	virtual void Update();

	virtual void OnCollision(CCollider2D* _pOther) override;
	virtual void OnCollisionEnter(CCollider2D* _pOther) override;
	virtual void OnCollisionExit(CCollider2D* _pOther) override;

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();

public:
	void SetChatObject(CGameObject* pObj);
	void SetInventoryObject(CGameObject* pObj);
	void SetStatusObject(CGameObject* pObj);

	bool GetEnable();

	void Damage(float fDamage);
};

