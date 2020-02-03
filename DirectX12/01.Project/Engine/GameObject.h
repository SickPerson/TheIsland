#pragma once
#include "Entity.h"

class CComponent;
class CTransform;
class CMeshRender;
class CScript;
class CCamera;
class CCollider2D;
class CCollider3D;
class CAnimation2D;
class CAnimation3D;
class CLight2D;
class CLight3D;

class CGameObject :
	public CEntity
{
private:
	friend class CLayer;
	friend class CEvnetManager;

private:
	CGameObject();
	virtual ~CGameObject();

	CGameObject( const CGameObject& obj );

private:
	CComponent*				m_arrCom[( UINT )COMPONENT_TYPE::END];
	vector<CScript*>		m_vecScript;
	vector<CGameObject*>	m_vecChild;
	CGameObject*			m_pParentObj;
	int						m_iLayerIdx;
	bool					m_bDead;
	bool					m_bActive;
	bool					m_bFrustumCheck;

private:
	void SetLayerIdx( int iLayerIdx );

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Enable();
	void Disable();

public:
	void SetActive( bool bActive );
	bool IsActive() const;

	void FrustumCheck( bool bFrustumCheck );
	bool GetFrustumCheck() const;

	bool IsDead();
	void SetDead();

	int GetLayerIdx();

public:
	void AddComponent( CComponent* pCom );
	CComponent*		GetComponent( COMPONENT_TYPE eType );
	CTransform*		Transform();
	CMeshRender*	MeshRender();
	CCollider2D*	Collider2D();
	CCollider3D*	Collider3D();
	CAnimation2D*	Animation2D();
	CCamera*		Camera();
	CLight2D*		Light2D();
	CLight3D*		Light3D();

	const vector<CScript*>& GetScripts() const;

	template<typename T>
	T* GetScript();

public:
	void AddChild( CGameObject* pChildObj );
	bool IsAncestor( CGameObject* pObj );
	void ClearParent( CGameObject* pNextParent = NULL );
	CGameObject* GetParent();
	
	const vector<CGameObject*>& GetChild();

	void RegisterToLayer();

public:
	CLONE( CGameObject );
};

template<typename T>
inline T * CGameObject::GetScript()
{
	T* pScript = NULL;
	for ( size_t i = 0; i < m_vecScript.size(); ++i )
	{
		pScript = dynamic_cast< T* >( m_vecScript[i] );
		if ( NULL != pScript )
			return pScript;
	}

	return pScript;
}
