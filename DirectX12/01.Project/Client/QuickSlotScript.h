#pragma once
#include <Engine/Script.h>
class CQuickSlotScript :
	public CScript
{
	int m_iSelect;

	bool m_bEnable;

	vector<CGameObject*> m_vQuickSlot;

	Vec4 m_vSlotColor;
	Vec4 m_vHighlightSlotColor;

public:
	CQuickSlotScript();
	virtual ~CQuickSlotScript();

public:
	CLONE(CQuickSlotScript);

public:
	virtual void Update();

	void SetEnable(bool bEnable);
	bool GetEnable();
	void KeyInput(int key);

	void AddQuickSlot(CGameObject* pObj);

	void SetHighlightSlotColor(Vec4 vColor);
	void SetSlotColor(Vec4 vColor);
};

