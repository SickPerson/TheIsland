#pragma once
#include <Engine/Script.h>
class CQuickSlotScript :
	public CScript
{
	int m_iSelect;

	bool m_bEnable;
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
};

