#pragma once
#include "ItemScript.h"

#define HOUSING_MAX_COUNT 1

class CHousingScript :
	public CItemScript
{
public:
	CHousingScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CHousingScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CHousingScript);

	virtual void Update() override;
	virtual void Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
};

