#pragma once

// CMyForm 폼 뷰입니다.
#include <Engine/Ptr.h>
#include <Engine/MeshData.h>
#include <Engine/GameObject.h>

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

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
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	CComboBox m_ComboObjectKind;
	afx_msg void OnCbnSelchangeComboObjectkind();
	CComboBox m_ComboObjects;
	afx_msg void OnCbnSelchangeComboObjects();
	// Object에 추가할 FBX를 선택하기 위한 목록
	CComboBox m_ComboFBX;
	CString m_strFBXName;  
	afx_msg void OnCbnSelchangeComboFbx();


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
	void LoadFBX();
	CButton m_btnFBXLoad;
	afx_msg void OnBnClickedButtonFbxload();
};


