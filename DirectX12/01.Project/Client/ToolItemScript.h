#pragma once
#include "ItemScript.h"

#define TOOL_MAX_COUNT 1

class CToolItemScript :
	public CItemScript
{
	float m_fDamage;
	CGameObject* m_pObj;
	CGameObject* m_pSubObj;
	bool m_bCharge;
	CGameObject* m_pHost;
public:
	CToolItemScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CToolItemScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CToolItemScript);

	virtual void Update() override;
	virtual UINT Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT EnableItem(CGameObject* pHost, int num) override;
	virtual void DisableItem(CGameObject* pHost, int num) override;
};

