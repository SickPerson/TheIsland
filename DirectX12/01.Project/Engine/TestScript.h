#include "Script.h"

class CTestScript :
	public CScript
{
private:
	int		m_iLifeCount;

	CGameObject* m_pTestObject;

public:	
	virtual void Update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
public:
	CLONE(CTestScript);

public:
	CTestScript();
	virtual ~CTestScript();

public:
	void SetTestObject(CGameObject* pObject) { m_pTestObject = pObject; }
};

