// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/SceneScript.h>

#include <Engine/GameObject.h>
// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
	, m_strObjectName( _T( "" ) )
	, m_fPosX( 0 )
	, m_fPosY( 0 )
	, m_fPosZ( 0 )
	, m_fScaleX( 0 )
	, m_fScaleY( 0 )
	, m_fRotX( 0 )
	, m_fRotY( 0 )
	, m_fRotZ( 0 )
	, m_fScaleZ( 0 )
{
	
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_BUTTON_ADDOBJECT, m_btnAddObject );
	DDX_Text( pDX, IDC_EDIT_OBJECTNAME, m_strObjectName );
	DDX_Text( pDX, IDC_EDIT_POSX, m_fPosX );
	DDX_Text( pDX, IDC_EDIT_POSY, m_fPosY );
	DDX_Text( pDX, IDC_EDIT_POSZ, m_fPosZ );
	DDX_Text( pDX, IDC_EDIT_SCALEX, m_fScaleX );
	DDX_Text( pDX, IDC_EDIT_SCALEY, m_fScaleY );
	DDX_Text( pDX, IDC_EDIT_SCALEZ, m_fScaleZ );
	DDX_Text( pDX, IDC_EDIT_ROTX, m_fRotX );
	DDX_Text( pDX, IDC_EDIT_ROTY, m_fRotY );
	DDX_Text( pDX, IDC_EDIT_ROTZ, m_fRotZ );
	DDX_Control( pDX, IDC_BUTTON_DELOBJECT, m_btnDelObject );
	DDX_Control( pDX, IDC_COMBO_OBJECTKIND, m_ComboObjectKind );
	DDX_Control( pDX, IDC_COMBO_OBJECTS, m_ComboObjects );
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED( IDC_BUTTON_ADDOBJECT, &CMyForm::OnBnClickedButtonAddobject )
	ON_EN_CHANGE( IDC_EDIT_OBJECTNAME, &CMyForm::OnEnChangeEditObjectname )
	ON_EN_CHANGE( IDC_EDIT_POSX, &CMyForm::OnEnChangeEditPosx )
	ON_EN_CHANGE( IDC_EDIT_POSY, &CMyForm::OnEnChangeEditPosy )
	ON_EN_CHANGE( IDC_EDIT_POSZ, &CMyForm::OnEnChangeEditPosz )
	ON_EN_CHANGE( IDC_EDIT_SCALEX, &CMyForm::OnEnChangeEditScalex )
	ON_EN_CHANGE( IDC_EDIT_SCALEY, &CMyForm::OnEnChangeEditScaley )
	ON_EN_CHANGE( IDC_EDIT_SCALEZ, &CMyForm::OnEnChangeEditScalez )
	ON_EN_CHANGE( IDC_EDIT_ROTX, &CMyForm::OnEnChangeEditRotx )
	ON_EN_CHANGE( IDC_EDIT_ROTY, &CMyForm::OnEnChangeEditRoty )
	ON_EN_CHANGE( IDC_EDIT_ROTZ, &CMyForm::OnEnChangeEditRotz )
	ON_BN_CLICKED( IDC_BUTTON_DELOBJECT, &CMyForm::OnBnClickedButtonDelobject )
	ON_CBN_SELCHANGE( IDC_COMBO_OBJECTKIND, &CMyForm::OnCbnSelchangeComboObjectkind )
	ON_CBN_SELCHANGE( IDC_COMBO_OBJECTS, &CMyForm::OnCbnSelchangeComboObjects )
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CMyForm::OnBnClickedButtonAddobject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	CGameObject* pObject = new CGameObject;
	pScene->AddGameObject( L"Default", pObject, false );

	int a = 0;
}


void CMyForm::OnBnClickedButtonDelobject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditObjectname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditPosx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditPosy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditPosz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditScalex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditScaley()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditScalez()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditRotx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditRoty()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnEnChangeEditRotz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnCbnSelchangeComboObjectkind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int  iSel = m_ComboObjectKind.GetCurSel();

	//CString strScriptName;
	//m_ComboObjectKind.GetLBText( iSel, strScriptName);
}


void CMyForm::OnCbnSelchangeComboObjects()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
