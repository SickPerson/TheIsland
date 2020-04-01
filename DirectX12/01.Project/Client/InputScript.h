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
	CGameObject*			m_pSpace;

	Vec4					m_vBox;
	// {x1, y1, x2, y2}

	Vec4					m_vFontColor;
	Vec4					m_vFontBackColor;

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
	// 폰트를 띄울 글자 한개마다 게임오브젝트에 있는 메쉬 하나씩 사용
	// 최대 15칸의 문자열이라면 15개의 게임오브젝트 연결
	void AddInputObject(CGameObject* pObject);
	// 마우스 충돌할때 사용할 박스 {x1, y1, x2, y2}
	void SetRect(Vec4 rc);
	Vec4 GetRect();
	// 글씨 색상 기본값은 흰색
	void SetFontColor(Vec4 vColor);
	// 글씨 배경 색상 기본값은 투명
	void SetFontBackColor(Vec4 vColor);

	// 글씨 배경 오브젝트
	void SetFontSpace(CGameObject* pObject);

	void SetEnable(bool bEnable);

	// 입력칸에 입력된 문자열 반환
	string GetString();
	bool GetEnable();
};

