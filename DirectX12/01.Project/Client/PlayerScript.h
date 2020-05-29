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

	CCamera*	m_pMainCamera;

	vector<CGameObject*> m_vCollisionObj;

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
	bool CollisionSphere(CCollider2D* _pOther, Vec3 vOffsetScale, float fOffset = 1.f);
	bool CollisionRay(Vec3 vPosRay, Vec3 vDirRay, CCollider2D* _pOther);

	void SetChatObject(CGameObject* pObj);
	void SetInventoryObject(CGameObject* pObj);
	void SetStatusObject(CGameObject* pObj);

	void SetMainCamera(CCamera* pCamera);

	bool GetEnable();

	void Damage(float fDamage);
};

