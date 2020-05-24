#pragma once
#include <Engine/Script.h>

// 마우스를 클릭해서 글자를 입력하는 경우에 사용

class CInputScript :
	public CScript
{
	int						m_iCount;
	int						m_iMaxCount;
	bool					m_bEnable;

	string					m_strInput;
	string					m_strEmpty;

	Vec4					m_vBox;
	// {x1, y1, x2, y2}

	Vec4					m_vHighlightColor;
	Vec4					m_vBackColor;
	Vec4					m_vFontColor;
public:
	CInputScript();
	virtual ~CInputScript();

	CLONE(CInputScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

private:
	void WriteFont(char font);

public:
	void Init();
	void Clear();
	// 마우스 충돌할때 사용할 박스 {x1, y1, x2, y2}
	void SetRect(Vec4 rc);
	Vec4 GetRect();
	// 글씨 색상 기본값은 흰색
	void SetFontColor(Vec4 vColor);
	// 글씨 배경 색상 기본값은 투명
	void SetHighlightColor(Vec4 vColor);
	void SetBackColor(Vec4 vColor);

	void SetEnable(bool bEnable);

	void SetMaxCount(int iCount);

	bool GetEnable();
	string GetString();
};

