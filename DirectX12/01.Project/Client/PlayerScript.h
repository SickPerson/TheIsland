#pragma once
#include <Engine/Script.h>

#define LEFT_CLICK true
#define RIGHT_CLICK false
#define PLAYER_ATTACK_COOLTIME 0.6f

class CCamera;
class CQuickSlotScript;

class CPlayerScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;
	float		m_fDamage;

	float		m_fAttackCoolTime;

	CGameObject* m_pChat;
	CGameObject* m_pInventory;
	CGameObject* m_pStatus;
	CQuickSlotScript* m_pQuickSlot;

	CCamera*	m_pMainCamera;

	vector<CGameObject*> m_vCollisionObj;

	bool		m_bEnable;

	bool		m_bInvincible;

	Vec3		m_vPrevPos;
	
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
	bool CollisionHouse(CCollider2D* _pOther, Vec3 vOffsetScale, UINT iType);
	bool CollisionHouse_Door(CCollider2D* _pOther, Vec3 vOffsetScale, Vec3 vOffsetPos);

	void SetChatObject(CGameObject* pObj);
	void SetInventoryObject(CGameObject* pObj);
	void SetStatusObject(CGameObject* pObj);
	void SetQuickSlot(CQuickSlotScript* pQuickSlot);
	CGameObject* GetStatusObject();
	CGameObject* GetInventoryObject();
	CGameObject* GetChatObject();

	void EnableItem(int num);
	void DisableItem(int num);

	void SetMainCamera(CCamera* pCamera);

	bool GetEnable();

	void Damage(float fDamage);
	void PlayerPicking(bool bLeft = LEFT_CLICK);
};

