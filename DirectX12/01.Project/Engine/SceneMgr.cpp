#include "stdafx.h"
#include "SceneMgr.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "ResMgr.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "Collider2D.h"
#include "Light2D.h"
#include "Light3D.h"
#include "LandScape.h"
#include "Mouse.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Camera.h"

#include "CollisionMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "Device.h"
#include "Core.h"

#include "MonsterScript.h"
#include "ToolCamScript.h"
#include "GridScript.h"

CScene* CSceneMgr::GetCurScene()
{
	return m_pCurScene;
}

void CSceneMgr::ChangeScene(CScene * _pNextScene)
{
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pNextScene;
}

CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)	
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::Init()
{
	m_pCurScene = CreateScene( L"Test Scene" );

	m_pCurScene->GetLayer( 0 )->SetName( L"Default" );
	m_pCurScene->GetLayer( 1 )->SetName( L"Player" );
	m_pCurScene->GetLayer( 2 )->SetName( L"Animal" );
	m_pCurScene->GetLayer( 3 )->SetName( L"Environment" );
	m_pCurScene->GetLayer( 4 )->SetName( L"Human" );
	m_pCurScene->GetLayer( 5 )->SetName( L"Build" );
	m_pCurScene->GetLayer( 6 )->SetName( L"House" );
	m_pCurScene->GetLayer( 7 )->SetName( L"Arrow" );

	m_pCurScene->GetLayer( 29 )->SetName( L"Invisible" );
	m_pCurScene->GetLayer( 30 )->SetName( L"UI" );
	m_pCurScene->GetLayer( 31 )->SetName( L"Tool" );

	m_pMouse = new CGameObject;
	m_pMouse->AddComponent( new CTransform );
	m_pMouse->AddComponent( new CMouse );

	m_pCurScene->GetLayer( 0 )->AddGameObject( m_pMouse );

}

void CSceneMgr::Update()
{	

	m_pCurScene->Update();
	m_pCurScene->LateUpdate();

	// Rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();

	m_pCurScene->FinalUpdate(); 
#ifdef CHECK_COLLISION
	// 충돌 처리
	CCollisionMgr::GetInst()->Update();
#else
#endif // CHECK_COLLISION
}

void CSceneMgr::Update_tool()
{
	// Rendermgr 카메라 초기화
	CRenderMgr::GetInst()->ClearCamera();
	m_pCurScene->FinalUpdate();
}

void CSceneMgr::FindGameObjectByTag(const wstring& _strTag, vector<CGameObject*>& _vecFindObj)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (_strTag == vecObject[j]->GetName())
			{
				_vecFindObj.push_back(vecObject[j]);
			}
		}
	}	
}

CScene * CSceneMgr::CreateScene( const wstring & _strTag )
{
	CScene* pScene = new CScene;
	pScene->SetName( _strTag );
	pScene->Awake();
	pScene->Start();

	return pScene;
}

CGameObject * CSceneMgr::Mouse()
{
	return m_pMouse;
}

CGameObject* CSceneMgr::CreateMRTUI()
{
	Vec3 vScale( 150.f, 150.f, 1.f );

	Ptr<CTexture> arrTex[5] = { CResMgr::GetInst()->FindRes<CTexture>( L"DiffuseTargetTex" )
		, CResMgr::GetInst()->FindRes<CTexture>( L"NormalTargetTex" )
		, CResMgr::GetInst()->FindRes<CTexture>( L"PositionTargetTex" )
		, CResMgr::GetInst()->FindRes<CTexture>( L"DiffuseLightTargetTex" )
		, CResMgr::GetInst()->FindRes<CTexture>( L"SpecularLightTargetTex" ) };

	CGameObject* pMRTUI = new CGameObject;
	pMRTUI->SetName(L"UI Object");
	pMRTUI->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	pMRTUI->AddComponent(new CTransform);

	pMRTUI->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pMRTUI->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	// AddGameObject
	m_pCurScene->FindLayer(L"UI")->AddGameObject(pMRTUI);

	for ( UINT i = 0; i < 5; ++i )
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetName( L"UI Object" );
		pObject->FrustumCheck( false );	// 절두체 컬링 사용하지 않음
		pObject->AddComponent( new CTransform );
		pObject->AddComponent( new CMeshRender );

		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();

		pObject->Transform()->SetLocalPos( Vec3( -( res.fWidth / 2.f ) + ( vScale.x / 2.f ) + ( i * vScale.x )
			, ( res.fHeight / 2.f ) - ( vScale.y / 2.f )
			, 1.f ) );

		pObject->Transform()->SetLocalScale( vScale );

		// MeshRender 설정
		pObject->MeshRender()->SetMesh( CResMgr::GetInst()->FindRes<CMesh>( L"RectMesh" ) );
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>( L"TexMtrl" );
		pObject->MeshRender()->SetMaterial( pMtrl->Clone() );
		pObject->MeshRender()->GetSharedMaterial()->SetData( SHADER_PARAM::TEX_0, arrTex[i].GetPointer() );
		pMRTUI->AddChild(pObject);
		// AddGameObject
		//m_pCurScene->FindLayer( L"UI" )->AddGameObject( pObject );
	}
	return pMRTUI;
}

bool Compare(CGameObject* _pLeft, CGameObject* _pRight)
{
	return (_pLeft->Transform()->GetWorldPos().z < _pRight->Transform()->GetWorldPos().z);
}

//void CSceneMgr::FindGameObjectByPoint(POINT _point, vector<CGameObject*>& _vecFindObj, CCamera* _pToolCam)
//{
//	CCamera* pCam = _pToolCam;
//	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
//	{
//		pCam = CRenderMgr::GetInst()->GetCamera(0);
//	}
//
//	tResolution tRes = CRenderMgr::GetInst()->GetResolution();
//	Vec3 vPickPos = Vec3((float)_point.x - (tRes.fWidth / 2.f), (tRes.fHeight / 2.f) - (float)_point.y, 0.f);
//	vPickPos *= pCam->GetScale(); 
//	vPickPos += pCam->Transform()->GetWorldPos();
//
//	for (int i = 0; i < MAX_LAYER; ++i)
//	{
//		const vector<CGameObject*>& vecObject = m_pCurScene->GetLayer(i)->GetObjects();
//		for (size_t j = 0; j < vecObject.size(); ++j)
//		{
//			if (vecObject[j]->Transform()->IsCasting(vPickPos))
//			{
//				_vecFindObj.push_back(vecObject[j]);
//			}
//		}
//	}
//
//	sort(_vecFindObj.begin(), _vecFindObj.end(), Compare);
//}

