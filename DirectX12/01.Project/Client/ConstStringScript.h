#pragma once
#include <Engine/Script.h>

class CConstStringScript :
	public CScript
{
	string m_strValue;
	Vec4 m_vFontColor;
	Vec4 m_vFontBackColor;
public:
	CConstStringScript();
	virtual ~CConstStringScript();

	// CScript을(를) 통해 상속됨
	CLONE(CConstStringScript);

	virtual void Update() override;

public:
	void Init(string str, Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f), Vec4 vFontBackColor = Vec4(0.f, 0.f, 0.f, 0.f));
	string GetString();
	void SetFontColor(Vec4 color);
	void SetBackColor(Vec4 color);
};

