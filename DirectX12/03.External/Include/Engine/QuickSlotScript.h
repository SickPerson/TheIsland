#pragma once
#include "Script.h"
class CQuickSlotScript :
	public CScript
{
	int m_iSelect;
public:
	CQuickSlotScript();
	virtual ~CQuickSlotScript();

public:
	CLONE(CQuickSlotScript);

public:
	virtual void Update();
};

