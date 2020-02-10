#pragma once

#include "Engine.h"

class CScene;
class CCamera;
class CGameObject;

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager)

private:
	CScene*			m_pCurScene;

public:
	void Init();
	void Update();

public:
	CScene*	GetCurScene();
	void ChangeScene( CScene* pNextScene );
	void FindGameObjectByTag( const wstring& strTag, vector<CGameObject*>& vecFindObj );
};

