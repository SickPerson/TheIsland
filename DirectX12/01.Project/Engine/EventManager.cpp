#include "stdafx.h"
#include "EventManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Component.h"

DEFINITION_SINGLE( CEventManager )

CEventManager::CEventManager()
{
}


CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	// 삭제 예정 Objects 메모리 해제
	for ( size_t i = 0; i < m_vecDead.size(); ++i )
	{
		m_vecDead[i]->ClearParent();
		SAFE_DELETE( m_vecDead[i] );
	}

	m_vecDead.clear();

	// Event 처리
	for ( size_t i = 0; i < m_vecEvent.size(); ++i )
	{
		Execute( m_vecEvent[i] );
	}
}

void CEventManager::Clear()
{
	m_vecEvent.clear();
}

void CEventManager::AddEvent( tEvent & e )
{
	m_vecEvent.push_back( e );
}

const vector<tEvent>& CEventManager::GetEventList()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecEvent;
}

void CEventManager::Execute( tEvent & e )
{
	switch ( e.eType )
	{
	case EVENT_TYPE::CREATE_OBJECT:
		GET_SINGLE(CSceneManager)->GetCurScene()->AddGameObject( ( int )e.lParam, ( CGameObject* )e.wParam, true );
		break;

	case EVENT_TYPE::DELETE_OBJECT:
		if ( ( ( CGameObject* )e.wParam )->m_bDead )
			break;

		( ( CGameObject* )e.wParam )->SetDead();
		m_vecDead.push_back( ( CGameObject* )e.wParam );

		break;
	case EVENT_TYPE::ADD_CHILD:
		( ( CGameObject* )e.wParam )->AddChild( ( CGameObject* )e.lParam );
		break;

	case EVENT_TYPE::CLEAR_PARENT:
		( ( CGameObject* )e.wParam )->ClearParent();
		break;

	case EVENT_TYPE::TRANSFER_LAYER:
	{
		bool bMoveAll = LOWORD( e.lParam );
		int iLayerIdx = HIWORD( e.lParam );
		GET_SINGLE( CSceneManager )->GetCurScene()->AddGameObject( iLayerIdx, ( ( CGameObject* )e.wParam ), bMoveAll );
	}
	break;

	case EVENT_TYPE::ACTIVATE_GAMEOBJECT:
		( ( CGameObject* )e.wParam )->Enable();
		( ( CGameObject* )e.wParam )->m_bActive = true;
		break;

	case EVENT_TYPE::DEACTIVATE_GAMEOBJECT:
		if ( ( ( CGameObject* )e.wParam )->IsDead() )
			break;

		( ( CGameObject* )e.wParam )->Disable();
		( ( CGameObject* )e.wParam )->m_bActive = false;
		break;

	case EVENT_TYPE::ACTIVATE_COMPONENT:
		( ( CComponent* )e.wParam )->Enable();
		( ( CComponent* )e.wParam )->m_bActive = true;
		break;

	case EVENT_TYPE::DEACTIVATE_COMPONENT:
		( ( CComponent* )e.wParam )->Disable();
		( ( CComponent* )e.wParam )->m_bActive = false;
		break;

	case EVENT_TYPE::TRANSFER_SCENE:

		break;
	}
}
