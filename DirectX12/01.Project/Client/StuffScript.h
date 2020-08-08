#pragma once
#include "ItemScript.h"

#define STUFF_MAX_COUNT 100

class CStuffScript :
	public CItemScript
{
public:
	CStuffScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CStuffScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CStuffScript);

	virtual void Update() override;
	virtual UINT Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT EnableItem(CGameObject* pHost, int num) override { return 0; };
	virtual void DisableItem(CGameObject* pHost, int num) override {};
};

