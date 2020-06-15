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
{
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
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
