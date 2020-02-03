#pragma once
#include "Entity.h"

class CGameObject;

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

class CComponent :
	public CEntity
{
private:
	friend class CGameObject;
	friend class CEventManager;
	friend void ComponentSetChange( CComponent*, bool );

protected:
	CComponent(COMPONENT_TYPE eType);
	virtual ~CComponent();

private:
	CGameObject*			m_pObject;
	COMPONENT_TYPE			m_eComType;
	bool					m_bChange;
	bool					m_bActive;

private:
	void SetGameObject( CGameObject* pObject );

public:
	void SetActive( bool bActive );
	bool IsActive();

	bool IsChanged();

protected:
	void Changed();

public:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update(){};
	virtual void LateUpdate() {};
	virtual void FinalUpdate(){};

	virtual void Enable() {};
	virtual void Disable(){};

	virtual void SaveToScene( FILE* pFile ) = 0;
	virtual void LoadFromScene( FILE* pFile ) = 0;

public:
	COMPONENT_TYPE	GetComponentType();
	CGameObject*	GetGameObject();
	CTransform*		Transform();
	CMeshRender*	MeshRender();
	CCollider2D*	Collider2D();
	CCollider3D*	Collider3D();
	CAnimation2D*	Animation2D();
	CCamera*		Camera();
	CLight2D*		Light2D();
	CLight3D*		Light3D();
};

