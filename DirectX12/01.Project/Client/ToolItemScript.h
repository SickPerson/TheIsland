#pragma once
#include "ItemScript.h"

#define TOOL_MAX_COUNT 1

class CToolItemScript :
	public CItemScript
{
public:
	CToolItemScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CToolItemScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CToolItemScript);

	virtual void Update() override;
	virtual void Use() override;
};

