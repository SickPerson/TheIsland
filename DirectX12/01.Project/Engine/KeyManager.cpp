#include "stdafx.h"
#include "KeyManager.h"
#include "RenderManager.h"

DEFINITION_SINGLE( CKeyManager )

// KEY_TYPE 에 대응하는 가상키
int arrVK[( UINT )KEY_TYPE::KEY_END] =
{
	VK_UP,		// KEY_UP,
	VK_DOWN,	// KEY_DOWN,
	VK_LEFT,	// KEY_LEFT,
	VK_RIGHT,	// KEY_RIGHT,

	VK_SPACE,   // KEY_SPACE,
	VK_RETURN,  // KEY_ENTER,
	VK_LSHIFT,  // KEY_LSHIFT,
	VK_LCONTROL, // KEY_LCTRL,
	VK_MENU,	// KEY_LALT,
	VK_ESCAPE,	// KEY_ESC,

	'1',//KEY_1,
	'2',//KEY_2,
	'3',//KEY_3,
	'4',//KEY_4,
	'5',//KEY_5,
	'6',//KEY_6,
	'7',//KEY_7,
	'8',//KEY_8,
	'9',//KEY_9,
	'0',//KEY_0,

	'Q',		// KEY_Q,
	'W',
	'S',
	'A',
	'D',
	'E',
	'Z',
	'Y',

	VK_LBUTTON,
	VK_RBUTTON,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,
};


CKeyManager::CKeyManager()
{
}


CKeyManager::~CKeyManager()
{
}

void CKeyManager::Init()
{
	for ( UINT i = 0; i < ( UINT )KEY_TYPE::KEY_END; ++i )
	{
		m_vecKey.push_back( tKeyState{ KEY_STATE::STATE_NONE, false } );
	}
}

void CKeyManager::Update()
{
	// 현재 윈도우가 Active(활성화, 포커싱) 상태가 아니라면, 
	// 모든 키 상태를 초기상태로 변경한다.
	HWND hActive = GetActiveWindow();

	if ( nullptr == hActive )
	{
		for ( UINT i = 0; i < ( UINT )KEY_TYPE::KEY_END; ++i )
		{
			m_vecKey[i].bPrevPress = false;
			m_vecKey[i].eState = KEY_STATE::STATE_NONE;
		}
		return;
	}

	// 윈도우가 활성상태라면 키를 정상적으로 update 한다.
	for ( UINT i = 0; i < ( UINT )KEY_TYPE::KEY_END; ++i )
	{
		// 현재 키가 눌려있다.
		if ( GetAsyncKeyState( arrVK[i] ) & 0x8000 )
		{
			if ( m_vecKey[i].bPrevPress ) // 이전 프레임에 눌려있었다.
			{
				m_vecKey[i].eState = KEY_STATE::STATE_HOLD;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_TAB;
			}
			m_vecKey[i].bPrevPress = true;
		}
		else // 키가 눌리지 않은 상태
		{
			if ( m_vecKey[i].bPrevPress ) // 이전 프레임에 눌려있었다.
			{
				m_vecKey[i].eState = KEY_STATE::STATE_AWAY;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_NONE;
			}
			m_vecKey[i].bPrevPress = false;
		}
	}

	// 마우스 좌표
	m_ptOldMouse = m_ptMouse;
	GetCursorPos( &m_ptMouse );
	ScreenToClient( GET_SINGLE( CRenderManager )->GetHwnd(), &m_ptMouse );

	m_vDragDir = Vec2( ( float )( m_ptMouse.x - m_ptOldMouse.x ), ( float )( m_ptOldMouse.y - m_ptMouse.y ) );
}

KEY_STATE CKeyManager::GetKeyState( KEY_TYPE _eKey )
{
	return m_vecKey[( UINT )_eKey].eState;
}

const POINT & CKeyManager::GetMousePos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_ptMouse;
}

const POINT & CKeyManager::GetPrevMousePos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_ptOldMouse;
}

Vec2 CKeyManager::GetDragDir()
{
	return m_vDragDir;
}
