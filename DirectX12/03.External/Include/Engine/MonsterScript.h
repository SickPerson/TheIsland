#pragma once
#include "Script.h"

class CMonsterScript :
	public CScript
{
private:
	int		m_iDir;
	bool	m_bUD;

public:
	virtual void Update();

	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();

	void SetUpDown(bool bUD);
};

