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
	virtual void Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
};
