#pragma once
#include "Entity.h"

class CGameObject;

class CLayer :
	public CEntity
{
private:	
	vector<CGameObject*>		m_vecParentObj; // 최상위 부모 오브젝트만
	vector<CGameObject*>		m_vecLayerObj;  // 해당 레이어에 속한 모든 오브젝트

	int							m_iLayerIdx;

public:
	void AddGameObject(CGameObject* _pObject, bool _bMoveAll = false);
	int GetLayerIdx() { return m_iLayerIdx; }
	const vector<CGameObject*>& GetParentObj() const { return m_vecParentObj; }
	const vector<CGameObject*>& GetObjects() const { return m_vecLayerObj; }
	void RegisterObj(CGameObject* _pObj) { m_vecLayerObj.push_back(_pObj); }
	void CheckParentObj();
	void RemoveParentObj(CGameObject* _pObject);
	void RemoveAll();

	CGameObject* GetMainCamera();

	CGameObject* FindObject( const wstring& strKey );

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();




private:
	void SetLayerIdx(int _iIdx) { m_iLayerIdx = _iIdx; }


public:
	CLayer();
	virtual ~CLayer();

	friend class CScene;
};

