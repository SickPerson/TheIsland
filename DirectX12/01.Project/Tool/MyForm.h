#pragma once

// CMyForm 폼 뷰입니다.
#include <Engine/Ptr.h>
#include <Engine/MeshData.h>
#include <Engine/GameObject.h>
#include <Engine/KeyMgr.h>

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE( CMyForm )

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump( CDumpContext& dc ) const;
#endif
#endif

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnAddObject;
	afx_msg void OnBnClickedButtonAddobject();
	CButton m_btnDelObject;
	afx_msg void OnBnClickedButtonDelobject();
	CString m_strObjectName;
	CEdit m_EditObjectName;
	afx_msg void OnEnChangeEditObjectname();
	float m_fPosX;
	afx_msg void OnEnChangeEditPosx();
	float m_fPosY;
	afx_msg void OnEnChangeEditPosy();
	float m_fPosZ;
	afx_msg void OnEnChangeEditPosz();
	float m_fScaleX;
	afx_msg void OnEnChangeEditScalex();
	float m_fScaleY;
	afx_msg void OnEnChangeEditScaley();
	float m_fScaleZ;
	afx_msg void OnEnChangeEditScalez();
	float m_fRotX;
	afx_msg void OnEnChangeEditRotx();
	float m_fRotY;
	afx_msg void OnEnChangeEditRoty();
	float m_fRotZ;
	afx_msg void OnEnChangeEditRotz();
	CString m_strFBXName;
	/*afx_msg void OnCbnSelchangeComboFbx();*/


	UINT		m_iSelectObject;


private:
	vector<Ptr<CMeshData>>	m_vecFBX;
	vector<CGameObject*>	m_vecGameObjet;

	bool m_bLoad;

private:
	Ptr<CMeshData> FindMeshData( const wstring& strFBXName );

public:
	CEdit m_EditPosX;
	CEdit m_EditPosY;
	CEdit m_EditPosZ;
	CEdit m_EditScaleX;
	CEdit m_EditScaleY;
	CEdit m_EditScaleZ;
	CEdit m_EditRotX;
	CEdit m_EditRotY;
	CEdit m_EditRotZ;

public:
	CButton m_btnFBXLoad;
	afx_msg void OnBnClickedButtonFbxload();
	CButton m_btnSave;
	afx_msg void OnBnClickedButtonSave();
	CButton m_btnLoad;
	afx_msg void OnBnClickedButtonLoad();

	bool m_bChoiceObject;
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );

	CGameObject* GetSelectObject();

	CEdit m_MousePosX;
	afx_msg void OnEnChangeEditMouseposx();
	CEdit m_MousePosY;
	afx_msg void OnEnChangeEditMouseposy();
	CEdit m_MousePosZ;
	afx_msg void OnEnChangeEditMouseposz();
	CEdit m_CameraPosX;
	afx_msg void OnEnChangeEditCameraposx();
	CEdit m_CameraPosY;
	afx_msg void OnEnChangeEditCameraposy();
	CEdit m_CameraPosZ;
	afx_msg void OnEnChangeEditCameraposz();
	CListBox m_MeshList;
	afx_msg void OnLbnSelchangeListMesh();
	CListBox m_ScriptList;
	afx_msg void OnLbnSelchangeListScript();
	CListBox m_ObjectList;
	afx_msg void OnLbnSelchangeListObject();
};


