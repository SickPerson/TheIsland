#include "stdafx.h"
#include "SceneManager.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()
{
}


CSceneManager::~CSceneManager()
{
}

void CSceneManager::Init()
{
}

void CSceneManager::Update()
{
}

CScene * CSceneManager::GetCurScene()
{
	return m_pCurScene;
}

void CSceneManager::ChangeScene( CScene * pNextScene )
{
}

void CSceneManager::FindGameObjectByTag( const wstring & strTag, vector<CGameObject*>& vecFindObj )
{
}
