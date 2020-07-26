// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/SceneScript.h>

#include <Engine/GameObject.h>
#include <Engine/Transform.h>

#include <Engine/Ptr.h>
#include <Engine/MeshData.h>
#include <Engine/ResMgr.h>

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
	, m_strObjectName( _T( "" ) )
	, m_pGameObject(NULL)
{
	m_vScale = Vec3::One;
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_BUTTON_MESHLOAD, m_btnMeshLoad );
	DDX_Text( pDX, IDC_EDIT_OBJECTNAME, m_strObjectName );
	DDX_Text( pDX, IDC_EDIT_POSX, m_vPos.x );
	DDX_Text( pDX, IDC_EDIT_POSY, m_vPos.y );
	DDX_Text( pDX, IDC_EDIT_POSZ, m_vPos.z );
	DDX_Text( pDX, IDC_EDIT_SCALEX, m_vScale.x );
	DDX_Text( pDX, IDC_EDIT_SCALEY, m_vScale.y );
	DDX_Text( pDX, IDC_EDIT_SCALEZ, m_vScale.z );
	DDX_Text( pDX, IDC_EDIT_ROTX, m_vRot.x );
	DDX_Text( pDX, IDC_EDIT_ROTY, m_vRot.y );
	DDX_Text( pDX, IDC_EDIT_ROTZ, m_vRot.z );
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	
	ON_BN_CLICKED( IDC_BUTTON_MESHLOAD, &CMyForm::OnBnClickedButtonMeshload )
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

void CMyForm::OnBnClickedButtonMeshload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	   
	wchar_t	strFilter[] = L"모든파일(*.*)|*.*|||";
	CFileDialog	dlg( TRUE, NULL, L"Mesh",	OFN_HIDEREADONLY, strFilter );

	if ( IDOK == dlg.DoModal() )
	{
		CString strName = dlg.GetFileTitle();				// 파일 이름 읽기
		CString strExt = dlg.GetFileExt();
		wstring strMeshPath = L"MeshData\\" + strName + "." + strExt;
		wstring strFbxPath = L"FBX\\" + strName + "." + strExt;

		strExt.MakeUpper();		

		CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

		if ( lstrcmp( strExt, L"FBX" ) == 0 )
		{
			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX( strFbxPath );
			int  a = 0;
		}

		else if ( lstrcmp( strExt, L"MDAT" ) == 0 )
		{
			Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>( strMeshPath, strMeshPath );

			m_pGameObject = pMeshData->Instantiate();
			m_pGameObject->SetName( m_strObjectName.GetString() );
			m_pGameObject->Transform()->SetLocalPos( Vec3( 0.f, 0.f, 0.f ) );
			m_pGameObject->Transform()->SetLocalRot( Vec3( 0.f, 0.f, 0.f ) );
			pScene->AddGameObject( L"Default", m_pGameObject, false );
		}
	}
}


void CMyForm::OnEnChangeEditObjectname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//UpdateData( TRUE );
}

void CMyForm::OnEnChangeEditPosx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vPos );
}


void CMyForm::OnEnChangeEditPosy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vPos );
}


void CMyForm::OnEnChangeEditPosz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vPos );
}


void CMyForm::OnEnChangeEditScalex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vScale );
}


void CMyForm::OnEnChangeEditScaley()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vScale );
}


void CMyForm::OnEnChangeEditScalez()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vScale );
}


void CMyForm::OnEnChangeEditRotx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vRot );
}


void CMyForm::OnEnChangeEditRoty()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vRot );
}


void CMyForm::OnEnChangeEditRotz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( !m_pGameObject )
		return;

	UpdateData( TRUE );
	m_pGameObject->Transform()->SetLocalPos( m_vRot );
}
