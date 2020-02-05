#pragma once
#include "Component.h"

enum class SCRIPT_TYPE
{
	BULLETSCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
	TESTSCRIPT,
	END
};

class CGameObject;
class CScript :
	public CComponent
{
private:
	friend class CCollider2D;
	friend class CCollider3D;

public:
	CScript(UINT iScriptType);
	virtual ~CScript();

private:
	UINT		m_iScriptType;

public:
	virtual void Update() = 0;

public:
	static void CreateObject( CGameObject* pNewObject, int iLayerIdx );
	static void CreateObject( CGameObject* pNewObject, const wstring& strLayerName );

	static void DeleteObject( CGameObject* pDeleteObject );

	void AddChild( CGameObject* pChildObject );
	static void AddChild( CGameObject* pParent, CGameObject* pChild );

	void ClearParent();
	static void ClearParent( CGameObject* pTarget );

	void TransferLayer( const wstring& strLayerName, bool bMoveAll );
	void TransferLayer( int iLayerIdx, bool bMoveAll );
	static void TransferLayer( CGameObject* pTarget, const wstring& strLayerName, bool bMoveAll );
	static void TransferLayer( CGameObject* pTarget, int iLayerIdx, bool bMoveAll );

	static void EableObject( CGameObject* pTarget );
	static void Disable( CGameObject* pTarget );

protected:
	virtual void OnCollisionEnter( CCollider2D* pOther );
	virtual void OnCollision( CCollider2D* pOther );
	virtual void OnCollisionExit( CCollider2D* pOther );

public:
	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );
};

