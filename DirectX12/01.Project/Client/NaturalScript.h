#pragma once
#include <Engine/Script.h>
#define NATURAL_RESPAWN_TIME 10.f

enum NATURAL_TYPE {
	NATURAL_TREE,
	NATURAL_STONE,
	NATURAL_BUSH,
	NATURAL_END
};

class CNaturalScript :
	public CScript
{
	float			m_fHealth;
	float			m_fTime;

	float			m_fAngle;

	NATURAL_TYPE	m_eType;

	bool			m_bDestory;
	bool			m_bRotate;

	Vec3			m_vOrginRot;

	CGameObject*	m_pParticleObj;
	float			m_fParticleTime;
public:
	CNaturalScript(NATURAL_TYPE eType);
	virtual ~CNaturalScript();

	CLONE(CNaturalScript);
	// CScript을(를) 통해 상속됨
	virtual void Update() override;

	virtual void OnCollision(CCollider2D* _pOther) override;
	virtual void OnCollisionEnter(CCollider2D* _pOther) override;
	virtual void OnCollisionExit(CCollider2D* _pOther) override;
public:
	bool Damage(CGameObject* pObj, float fDamage);
	bool GetDestroy();
	NATURAL_TYPE GetNaturalType();

	void Respawn();
};

