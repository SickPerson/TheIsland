#pragma once
#include "Entity.h"

class CLayer;
class CGameObject;
class CCamera;

class CScene :
	public CEntity
{
public:
	CScene();
	virtual ~CScene();

private:
	CLayer*			m_arrLayer[MAX_LAYER];

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	void SetLayerName( int iIdx, const wstring& strName );
	void AddGameObject( const wstring& strLayerName, CGameObject* pObject, bool bMoveAll );
	void AddGameObject( int iLayerIdx, CGameObject* pObject, bool bMoveAll );
	CLayer* FindLayer( const wstring& strLayerName );
	CLayer* GetLayer( int iIdx );

private:
	void AddLayer( const wstring& strLayerName, int iIdx = -1 );
};

