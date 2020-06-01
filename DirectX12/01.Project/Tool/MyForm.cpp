// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/SceneScript.h>

#include <Engine/ResMgr.h>

#include <Engine/Transform.h>

#include <Engine/CollisionMgr.h>
#include <Engine/Collider2D.h>

#include <algorithm>

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
	, m_iSelectObject(0)
	, m_bLoad(false)
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
	DDX_Control( pDX, IDC_EDIT_OBJECTNAME, m_EditObjectName );
	DDX_Control( pDX, IDC_COMBO_FBX, m_ComboFBX );
	DDX_Control( pDX, IDC_EDIT_POSX, m_EditPosX );
	DDX_Control( pDX, IDC_EDIT_POSY, m_EditPosY );
	DDX_Control( pDX, IDC_EDIT_POSZ, m_EditPosZ );
	DDX_Control( pDX, IDC_EDIT_SCALEX, m_EditScaleX );
	DDX_Control( pDX, IDC_EDIT_SCALEY, m_EditScaleY );
	DDX_Control( pDX, IDC_EDIT_SCALEZ, m_EditScaleZ );
	DDX_Control( pDX, IDC_EDIT_ROTX, m_EditRotX );
	DDX_Control( pDX, IDC_EDIT_ROTY, m_EditRotY );
	DDX_Control( pDX, IDC_EDIT_ROTZ, m_EditRotZ );
	DDX_Control( pDX, IDC_BUTTON_FBXLOAD, m_btnFBXLoad );
	DDX_Control( pDX, IDC_BUTTON_SAVE, m_btnSave );
	DDX_Control( pDX, IDC_BUTTON_LOAD, m_btnLoad );
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
	ON_CBN_SELCHANGE( IDC_COMBO_FBX, &CMyForm::OnCbnSelchangeComboFbx )
	ON_CBN_SELCHANGE( IDC_COMBO_FBX, &CMyForm::OnCbnSelchangeComboFbx )
	ON_BN_CLICKED( IDC_BUTTON_FBXLOAD, &CMyForm::OnBnClickedButtonFbxload )
	ON_BN_CLICKED( IDC_BUTTON_SAVE, &CMyForm::OnBnClickedButtonSave )
	ON_BN_CLICKED( IDC_BUTTON_LOAD, &CMyForm::OnBnClickedButtonLoad )
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
	if ( m_strObjectName.IsEmpty() )
		return;

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	wstring strFBXName = L"MeshData\\" + m_strFBXName + L".mdat";	

	Ptr<CMeshData> pMeshData = FindMeshData( strFBXName );

	CGameObject* pObject = pMeshData->Instantiate();

	m_vecGameObjet.push_back( pObject );

	pScene->AddGameObject( L"Default", pObject, false );

	m_ComboObjects.AddString( m_strObjectName );
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
	
	m_EditObjectName.GetWindowTextW( m_strObjectName );
}


void CMyForm::OnEnChangeEditPosx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString posX, posY, posZ;
	m_EditPosX.GetWindowTextW( posX );
	m_EditPosY.GetWindowTextW( posY );
	m_EditPosZ.GetWindowTextW( posZ );

	m_fPosX = _tstof( posX );
	m_fPosY = _tstof( posY );
	m_fPosZ = _tstof( posZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalPos( Vec3( m_fPosX, m_fPosY, m_fPosZ ) );
}


void CMyForm::OnEnChangeEditPosy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString posX, posY, posZ;
	m_EditPosX.GetWindowTextW( posX );
	m_EditPosY.GetWindowTextW( posY );
	m_EditPosZ.GetWindowTextW( posZ );

	m_fPosX = _tstof( posX );
	m_fPosY = _tstof( posY );
	m_fPosZ = _tstof( posZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalPos( Vec3( m_fPosX, m_fPosY, m_fPosZ ) );
}


void CMyForm::OnEnChangeEditPosz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString posX, posY, posZ;
	m_EditPosX.GetWindowTextW( posX );
	m_EditPosY.GetWindowTextW( posY );
	m_EditPosZ.GetWindowTextW( posZ );

	m_fPosX = _tstof( posX );
	m_fPosY = _tstof( posY );
	m_fPosZ = _tstof( posZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalPos( Vec3( m_fPosX, m_fPosY, m_fPosZ ) );
}


void CMyForm::OnEnChangeEditScalex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString ScaleX, ScaleY, ScaleZ;
	m_EditScaleX.GetWindowTextW( ScaleX );
	m_EditScaleY.GetWindowTextW( ScaleY );
	m_EditScaleZ.GetWindowTextW( ScaleZ );

	m_fScaleX = _tstof( ScaleX );
	m_fScaleY = _tstof( ScaleY );
	m_fScaleZ = _tstof( ScaleZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalScale( Vec3( m_fScaleX, m_fScaleY, m_fScaleZ ) );
}


void CMyForm::OnEnChangeEditScaley()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString ScaleX, ScaleY, ScaleZ;
	m_EditScaleX.GetWindowTextW( ScaleX );
	m_EditScaleY.GetWindowTextW( ScaleY );
	m_EditScaleZ.GetWindowTextW( ScaleZ );

	m_fScaleX = _tstof( ScaleX );
	m_fScaleY = _tstof( ScaleY );
	m_fScaleZ = _tstof( ScaleZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalScale( Vec3( m_fScaleX, m_fScaleY, m_fScaleZ ) );
}


void CMyForm::OnEnChangeEditScalez()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString ScaleX, ScaleY, ScaleZ;
	m_EditScaleX.GetWindowTextW( ScaleX );
	m_EditScaleY.GetWindowTextW( ScaleY );
	m_EditScaleZ.GetWindowTextW( ScaleZ );

	m_fScaleX = _tstof( ScaleX );
	m_fScaleY = _tstof( ScaleY );
	m_fScaleZ = _tstof( ScaleZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalScale( Vec3( m_fScaleX, m_fScaleY, m_fScaleZ ) );
}


void CMyForm::OnEnChangeEditRotx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString RotX, RotY, RotZ;
	m_EditRotX.GetWindowTextW( RotX );
	m_EditRotY.GetWindowTextW( RotY );
	m_EditRotZ.GetWindowTextW( RotZ );

	m_fRotX = _tstof( RotX );
	m_fRotY = _tstof( RotY );
	m_fRotZ = _tstof( RotZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalRot( Vec3( m_fRotX, m_fRotY, m_fRotZ ) );
}


void CMyForm::OnEnChangeEditRoty()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString RotX, RotY, RotZ;
	m_EditRotX.GetWindowTextW( RotX );
	m_EditRotY.GetWindowTextW( RotY );
	m_EditRotZ.GetWindowTextW( RotZ );

	m_fRotX = _tstof( RotX );
	m_fRotY = _tstof( RotY );
	m_fRotZ = _tstof( RotZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalRot( Vec3( m_fRotX, m_fRotY, m_fRotZ ) );
}


void CMyForm::OnEnChangeEditRotz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iSelectObject = m_ComboObjects.GetCurSel();

	CString RotX, RotY, RotZ;
	m_EditRotX.GetWindowTextW( RotX );
	m_EditRotY.GetWindowTextW( RotY );
	m_EditRotZ.GetWindowTextW( RotZ );

	m_fRotX = _tstof( RotX );
	m_fRotY = _tstof( RotY );
	m_fRotZ = _tstof( RotZ );

	m_vecGameObjet[m_iSelectObject]->Transform()->SetLocalRot( Vec3( m_fRotX, m_fRotY, m_fRotZ ) );
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

	m_iSelectObject = m_ComboObjects.GetCurSel();

	Vec3 vPos = m_vecGameObjet[m_iSelectObject]->Transform()->GetLocalPos();
	Vec3 vScale = m_vecGameObjet[m_iSelectObject]->Transform()->GetLocalScale();
	Vec3 vRot = m_vecGameObjet[m_iSelectObject]->Transform()->GetLocalRot();

	CString PosX, PosY, PosZ;
	CString RotX, RotY, RotZ;
	CString ScaleX, ScaleY, ScaleZ;

	PosX.Format( L"%f", vPos.x );
	PosY.Format( L"%f", vPos.y );
	PosZ.Format( L"%f", vPos.z );
	RotX.Format( L"%f", vRot.x );
	RotY.Format( L"%f", vRot.y );
	RotZ.Format( L"%f", vRot.z );
	ScaleX.Format( L"%f", vScale.x );
	ScaleY.Format( L"%f", vScale.y );
	ScaleZ.Format( L"%f", vScale.z );

	m_EditPosX.SetWindowTextW( PosX );
	m_EditPosY.SetWindowTextW( PosY );
	m_EditPosZ.SetWindowTextW( PosZ );
	m_EditRotX.SetWindowTextW( RotX );
	m_EditRotY.SetWindowTextW( RotY );
	m_EditRotZ.SetWindowTextW( RotZ );
	m_EditScaleX.SetWindowTextW( ScaleX );
	m_EditScaleY.SetWindowTextW( ScaleY );
	m_EditScaleZ.SetWindowTextW( ScaleZ );
}


void CMyForm::OnCbnSelchangeComboFbx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// m_ComboFBX에서 선택된 셀의 값을 가져온다.
	m_ComboFBX.GetLBText( m_ComboFBX.GetCurSel(), m_strFBXName );
}

Ptr<CMeshData> CMyForm::FindMeshData( const wstring & strFBXName )
{
	for ( auto iter = m_vecFBX.begin(); iter != m_vecFBX.end(); ++iter )
	{
		if ( ( *iter )->GetName() == strFBXName )
			return *iter;
	}

	return nullptr;
}

void CMyForm::LoadFBX()
{
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rock01.mdat", L"MeshData\\rock01.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rock01" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rock02.mdat", L"MeshData\\rock02.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rock02" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rock03.mdat", L"MeshData\\rock03.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rock03" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rock04.mdat", L"MeshData\\rock04.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rock04" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rockdetailsa.mdat", L"MeshData\\rockdetailsa.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rockdetailsa" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rockdetailsb.mdat", L"MeshData\\rockdetailsb.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rockdetailsb" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rockmediuma.mdat", L"MeshData\\rockmediuma.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rockmediuma" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rockmediumb.mdat", L"MeshData\\rockmediumb.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rockmediumb" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\rockmediumc.mdat", L"MeshData\\rockmediumc.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"rockmediumc" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\spruce_full_a.mdat", L"MeshData\\spruce_full_a.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"spruce_full_a" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\sprucea.mdat", L"MeshData\\sprucea.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"sprucea" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\spruceb.mdat", L"MeshData\\spruceb.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"spruceb" );

	pMeshData = CResMgr::GetInst()->Load<CMeshData>( L"MeshData\\sprucec.mdat", L"MeshData\\sprucec.mdat" );
	m_vecFBX.push_back( pMeshData );
	m_ComboFBX.AddString( L"sprucec" );
}


void CMyForm::OnBnClickedButtonFbxload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( m_bLoad )
		return;

	m_bLoad = true;
	LoadFBX();

	CCollider2D* pColl1 = new CCollider2D;
	CCollider2D* pColl2 = new CCollider2D;

	CCollisionMgr::GetInst()->CollisionSphereRay( pColl1, pColl2 );
}


void CMyForm::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyForm::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
