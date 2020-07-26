#pragma once
#include "ItemScript.h"
#define ARMOR_MAX_COUNT 1

class CArmorScript :
	public CItemScript
{
public:
	CArmorScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CArmorScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CArmorScript);

	virtual void Update() override;
	virtual void Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void EnableItem(CGameObject* pHost, int num) override {};
	virtual void DisableItem(CGameObject* pHost, int num) override {};
};

