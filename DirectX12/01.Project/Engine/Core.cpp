#include "stdafx.h"
#include "Core.h"
#include "Device.h"
#include "PathManager.h"
#include "KeyManager.h"
#include "TimeManager.h"

DEFINITION_SINGLE(CCore)

CCore::CCore() :
	m_hMainHwnd(NULL)
{
}


CCore::~CCore()
{
	DESTROY_SINGLE( CPathManager );
	DESTROY_SINGLE( CKeyManager );
	DESTROY_SINGLE( CTimeManager );
	DESTROY_SINGLE( CDevice );
}

int CCore::Init( HWND hWnd, const RESOLUTION & resolution, bool bWindow )
{
	m_hMainHwnd = hWnd;
	ChangeWindowSize( m_hMainHwnd, resolution );
	ShowWindow( hWnd, true );

	if ( FAILED( GET_SINGLE( CDevice )->Init( hWnd, resolution, bWindow ) ) )
		return E_FAIL;

	// 상수버퍼
	GET_SINGLE( CDevice )->CreateConstBuffer( L"GLOBAL_MATRIX", sizeof( Vec4 ), 512, CONST_REGISTER::b0 );
	GET_SINGLE( CDevice )->CreateConstBuffer( L"GLOBAL_MATRIX2", sizeof( Vec4 ), 512, CONST_REGISTER::b1 );

	GET_SINGLE( CPathManager )->Init();
	GET_SINGLE( CKeyManager )->Init();
	GET_SINGLE( CTimeManager )->Init();

	return S_OK;
}

void CCore::Run()
{
	GET_SINGLE( CKeyManager )->Update();
	GET_SINGLE( CTimeManager )->Update();

	Update();

	// LateUpdate, FinalUpdate는 SceneManager에서 해주기 때문에 삭제
	
	Render();
}

void CCore::ChangeWindowSize( HWND hWnd, const RESOLUTION & resolution )
{
	RECT rt = { 0, 0, ( int )resolution.fWidth, ( int )resolution.fHeight };

	AdjustWindowRect( &rt, WS_OVERLAPPEDWINDOW, false );
	SetWindowPos( hWnd, NULL, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0 );
}

void CCore::Update()
{

}

void CCore::Render()
{
}
