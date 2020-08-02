
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"

#include <Engine/Core.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/SceneScript.h>
#include <Engine/SceneMgr.h>
#include <Engine/NaviMgr.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>

#include "MapToolScene.h"
#include "MyForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE( CToolView, CScrollView )

BEGIN_MESSAGE_MAP( CToolView, CScrollView )
	// 표준 인쇄 명령입니다.
	ON_COMMAND( ID_FILE_PRINT, &CScrollView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview )
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸
HWND g_hWnd;

CToolView::CToolView() :
	m_pForm( NULL )
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow( CREATESTRUCT& cs )
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow( cs );
}

// CToolView 그리기

// WM_PAINT 메시지 발생할 때 호출됨.
void CToolView::OnDraw( CDC* pDC )
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if ( !pDoc )
		return;



	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting( CPrintInfo* pInfo )
{
	// 기본적인 준비
	return DoPreparePrinting( pInfo );
}

void CToolView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.

}

void CToolView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump( CDumpContext& dc ) const
{
	CScrollView::Dump( dc );
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CToolDoc ) ) );
	return ( CToolDoc* )m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::SetForm( CMyForm * pForm )
{
	m_pForm = pForm;
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( FAILED( CCore::GetInst()->ToolInit( AfxGetInstanceHandle(), m_hWnd, tResolution{ 0, 0 }, tResolution{ 1280, 1000 }, true ) ) )
	{
		return;
	}


	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	CMapToolScene* pScript = pScene->CreateSceneScript<CMapToolScene>( L"SceneScript" );

	CScrollView::SetScrollSizes( MM_TEXT, CSize( 1280, 720 ) );
}

// WM_LBUTTONDOWN 메시지 발생 시 호출됨.
void CToolView::OnLButtonDown( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnLButtonDown( nFlags, point );

	/*D3DXVECTOR3 vMouse =
	{
		(float)point.x + GetScrollPos(0),
		(float)point.y + GetScrollPos(1),
		0.f
	};*/

	CGameObject* pObject = m_pForm->GetSelectObject();
	if ( !pObject )
		return;

	Vec3 vPos = pObject->Transform()->GetLocalPos();

	Vec3 vDis;
	vDis.x = m_vCamera.x - ( 640.f - m_vMouse.x );
	vDis.z = m_vCamera.z + ( 500.f - m_vMouse.y );

	vPos.x = vDis.x;
	vPos.z = vDis.z + 20.f;

	vPos.y = CNaviMgr::GetInst()->GetY( vPos );

	pObject->Transform()->SetLocalPos( vPos );


	// 화면갱신 함수. WM_PAINT 메시지를 발생시킴.
	Invalidate( FALSE );
}

// WM_MOUSEMOVE 메시지 발생 시 호출됨.
void CToolView::OnMouseMove( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CGameObject* pMouse = CSceneMgr::GetInst()->Mouse();
	m_vMouse = pMouse->Transform()->GetLocalPos();

	CString strPosX, strPosY, strPosZ;
	strPosX.Format( L"%f", m_vMouse.x );
	strPosY.Format( L"%f", m_vMouse.y );
	strPosZ.Format( L"%f", m_vMouse.z );

	m_pForm->m_MousePosX.SetWindowTextW( strPosX );
	m_pForm->m_MousePosY.SetWindowTextW( strPosY );
	m_pForm->m_MousePosZ.SetWindowTextW( strPosZ );

	CGameObject* pCamera = CSceneMgr::GetInst()->GetCurScene()->GetLayer( 0 )->GetMainCamera();
	m_vCamera = pCamera->Transform()->GetLocalPos();

	strPosX.Format( L"%f", m_vCamera.x );
	strPosY.Format( L"%f", m_vCamera.y );
	strPosZ.Format( L"%f", m_vCamera.z );

	m_pForm->m_CameraPosX.SetWindowTextW( strPosX );
	m_pForm->m_CameraPosY.SetWindowTextW( strPosY );
	m_pForm->m_CameraPosZ.SetWindowTextW( strPosZ );

	CScrollView::OnMouseMove( nFlags, point );
}
