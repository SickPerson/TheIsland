#pragma once
#include <Engine/Script.h>

class CBuildScript :
	public CScript
{
	bool m_bBuild;
	bool m_bCollision;
public:
	CBuildScript();
	virtual ~CBuildScript();

public:
	virtual void Update();
	CLONE(CBuildScript);

	virtual void OnCollision(CCollider2D* _pOther) override;
	virtual void OnCollisionEnter(CCollider2D* _pOther) override;
	virtual void OnCollisionExit(CCollider2D* _pOther) override;

public:
	void Init();
	bool Build();
};

