#pragma once

#include <Engine/Script.h>

class CGameObject;

class CStoneScript : 
	public CScript
{
public:
	CStoneScript();
	virtual ~CStoneScript();

	CLONE( CStoneScript );

private:
	CGameObject*	m_pGameObject;
	wstring			m_strFBXName;

public:
	virtual void OnCollisionEnter( CCollider2D* _pOther );
	virtual void OnCollision( CCollider2D* _pOther );
	virtual void OnCollisionExit( CCollider2D* _pOther );
	virtual void Update();

public:
	void SetGameObject( CGameObject* pGameObject );
	void SetFBXName( const wstring& strFBXName );

	void SetTransform( CTransform* pTransform );

	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

