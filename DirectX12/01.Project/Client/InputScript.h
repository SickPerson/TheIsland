#pragma once
#include <Engine/Script.h>

// 마우스를 클릭해서 글자를 입력하는 경우에 사용

class CInputScript :
	public CScript
{
	bool					m_bEnable;
	int						m_iCount;
	string					m_strInput;
	vector<CGameObject*>	m_vFont;
	Rect					m_rcBox;
public:
	CInputScript();
	virtual ~CInputScript();

	CLONE(CInputScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

public:
	void AddInputObject(CGameObject* pObject);
	void SetRect(Rect rc);
	void Init();
};

