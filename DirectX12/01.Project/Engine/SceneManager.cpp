#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "ToolCameraScript.h"
#include "Light3D.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() : 
	m_pCurScene(NULL)
{
}


CSceneManager::~CSceneManager()
{
	SAFE_DELETE( m_pCurScene );
}

void CSceneManager::Init()
{
	// ==================
	// 필요한 리소스 로딩
	// ==================
	
	// ==================
	//     Scene 생성
	// ==================
	m_pCurScene = new CScene;
	m_pCurScene->SetName( L"Default Scene" );

	// ==================
	//  Layer 이름 지정
	// ==================	
	m_pCurScene->GetLayer(0)->SetName(L"Default");
	m_pCurScene->GetLayer( 1 )->SetName( L"Player" );
	m_pCurScene->GetLayer( 2 )->SetName( L"Monster" );
	m_pCurScene->GetLayer( 3 )->SetName( L"Bullet" );
		
	CGameObject* pObject = nullptr;

	// ==================
	// Camera Object 생성
	// ==================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CCamera );
	pObject->AddComponent( new CToolCameraScript );

	pObject->Camera()->SetProjType( PROJ_TYPE::PERSPECTIVE );
	pObject->Camera()->SetFar( 100000.f );
	pObject->Camera()->SetLayerAllCheck();

	m_pCurScene->FindLayer( L"Default" )->AddGameObject( pObject );

	// ==================
	// 3DLight Object 추가
	// ==================
	pObject = new CGameObject;
	pObject->AddComponent( new CTransform );
	pObject->AddComponent( new CLight3D );


	pObject->Light3D()->SetLightPos( Vec3( 0.f, 200.f, 1000.f ) );
	pObject->Light3D()->SetLightType( LIGHT_TYPE::POINT );
	pObject->Light3D()->SetDiffuseColor( Vec3( 1.f, 1.f, 1.f ) );
	pObject->Light3D()->SetSpecular( Vec3( 0.3f, 0.3f, 0.3f ) );
	pObject->Light3D()->SetAmbient( Vec3( 0.f, 0.f, 0.f ) );
	pObject->Light3D()->SetLightDir( Vec3( 1.f, -1.f, 1.f ) );
	pObject->Light3D()->SetLightRange( 500.f );

	m_pCurScene->FindLayer( L"Default" )->AddGameObject( pObject );
}

void CSceneManager::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	// RenderManager Camera 초기화
	GET_SINGLE( CRenderManager )->ClearCamera();

	m_pCurScene->FinalUpdate();

	// 충돌처리 
	//GET_SINGLE( CCollisionManager )->Update();
}

CScene * CSceneManager::GetCurScene()
{
	return m_pCurScene;
}

void CSceneManager::ChangeScene( CScene * pNextScene )
{
	SAFE_DELETE( m_pCurScene );
	m_pCurScene = pNextScene;
}

void CSceneManager::FindGameObjectByTag( const wstring & strTag, vector<CGameObject*>& vecFindObj )
{
	for ( int i = 0; i < MAX_LAYER; ++i )
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer( i )->GetObjects();
		for ( size_t j = 0; j < vecObject.size(); ++j )
		{
			if ( strTag == vecObject[j]->GetName() )
			{
				vecFindObj.push_back( vecObject[j] );
			}
		}
	}
}
