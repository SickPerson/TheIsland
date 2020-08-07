#pragma once
#include <Engine/Script.h>

#define ARROW_REMAIN_TIME 10.f

class CArrowScript :
	public CScript
{
	Vec3	m_vDir;
	Vec3	m_vRot;

	float	m_fDamage;
	float	m_fSpeed;

	float	m_fRemainTime;

	bool	m_bCollision;
	bool	m_bShoot;

	CGameObject* m_pHost;
	CGameObject* m_pCollisionObj;
	Vec3	m_vDiffPos;
public:
	CArrowScript();
	virtual ~CArrowScript();

	CLONE(CArrowScript);
	virtual void Update() override;

	virtual void OnCollision(CCollider2D* _pOther) override;
	virtual void OnCollisionEnter(CCollider2D* _pOther) override;
	virtual void OnCollisionExit(CCollider2D* _pOther) override;

public:
	bool CollisionHouse(CCollider2D* _pOther, Vec3 vHouseOffsetScale, UINT iType);

	void Init(CGameObject* pHost, float fDamage);
	void Charge();
	void Shoot(Vec3 vDir, Vec3 vRot, Vec3 vPos);

	void Collision(CGameObject* pOther);

	float GetDamage() { return m_fDamage; }
	CGameObject* GetHost() { return m_pHost; }
	bool GetCollision() { return m_bCollision; }
};

