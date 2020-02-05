#pragma once
#include "Component.h"

class CMesh;
class CMaterial;

class CCollider2D :
	public CComponent
{
public:
	CCollider2D();
	CCollider2D(const CCollider2D& other);
	virtual ~CCollider2D();

private:
	static	UINT	g_iColID;

private:
	Vec3				m_vOffsetPos;
	Vec3				m_vOffsetScale;
	COLLIDER2D_TYPE		m_eType;
	int					m_iCollisionCount;		// Ãæµ¹ È½¼ö

	Ptr<CMesh>			m_pColMesh;
	Ptr<CMaterial>		m_pColMtrl;

	Matrix				m_matColWorld;

	UINT				m_iColID;

public:
	virtual void Update();
	virtual void FinalUpdate();
	void	Render();
	
	void OnCollisionEnter( CCollider2D* pOther );
	void OnCollision( CCollider2D* pOther );
	void OnCollisionExit( CCollider2D* pOther );

public:
	void SetOffsetPos( const Vec3& vPos );
	void SetOffsetScale( const Vec3& vScale );
	void SetCollider2DType( COLLIDER2D_TYPE eType );

public:
	const Vec3& GetOffsetPos();
	const Vec3& GetOffsetScale();
	COLLIDER2D_TYPE GetColliderType();
	UINT	GetColID();
	const Matrix& GetColliderWorldMat();

public:
	CLONE( CCollider2D );
	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );

public:
	void operator =( const CCollider2D& other );
};


