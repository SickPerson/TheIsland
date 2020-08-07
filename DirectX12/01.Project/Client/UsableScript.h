#pragma once
#include "ItemScript.h"

#define USABLE_MAX_COUNT 10

class CUsableScript :
	public CItemScript
{
	float m_fValue;
public:
	CUsableScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CUsableScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CUsableScript);

	virtual void Update() override;
	virtual UINT Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT EnableItem(CGameObject* pHost, int num) override { return 0; };
	virtual void DisableItem(CGameObject* pHost, int num) override {};
};
