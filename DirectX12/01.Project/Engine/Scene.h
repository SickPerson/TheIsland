#pragma once
#include "Entity.h"

class CLayer;
class CGameObject;
class CCamera;
class CSceneScript;

class CScene :
	public CEntity
{
public:
	CScene();
	virtual ~CScene();

private:
	CLayer*				m_arrLayer[MAX_LAYER];	
	CSceneScript*		m_pCurSceneScript;

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();	

public:
	void SetLayerName(int _iIdx, const wstring& _strName);
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll);	
	void AddGameObject(int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll);
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iIdx) { return m_arrLayer[_iIdx]; };
	
private:
	void AddLayer(const wstring& _strLayerName, int _iIdx = -1);

public:
	template<typename T>
	T* CreateSceneScript( const wstring& _strTag )
	{
		SAFE_DELETE( m_pCurSceneScript );

		T* pScript = new T;

		pScript->SetName( _strTag );
		pScript->SetScene( this );
		pScript->Init();

		m_pCurSceneScript = pScript;

		return pScript;
	}
};

