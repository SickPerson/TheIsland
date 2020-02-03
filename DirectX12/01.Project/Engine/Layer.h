#pragma once
#include "Entity.h"

class CGameObject;

class CLayer :
	public CEntity
{
private:
	friend class CScene;

private:
	CLayer();
	virtual ~CLayer();

private:
	vector<CGameObject*>			m_vecParentObj;			// 최상위 부모 오브젝트
	vector<CGameObject*>			m_vecLayerObj;

	int								m_iLayerIdx;

public:
	void AddGameObject( CGameObject* pObject, bool bMoveAll = false );
	int GetLayerIdx() const;
	const vector<CGameObject*>& GetParentObj()	const;
	const vector<CGameObject*>& GetObjects()	const;
	void RegisterObject( CGameObject* pObj );
	void CheckParentObject();
	void RemoveParentObject( CGameObject* pObject );

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

private:
	void SetLayerIdx( int iIdx );
};

