#pragma once
#include <Engine/Script.h>
#include "Item.h"

class CItemScript :
	public CScript
{
	Ptr<CTexture>		m_pIconTex;

	bool				m_bInit;

	CGameObject*		m_pCountObj;

protected:
	ITEM_TYPE			m_eItemType;

	int					m_iCount;
	int					m_iMaxCount;


public:
	CItemScript(UINT iItemType);
	virtual ~CItemScript();
public:
	virtual void Use() {}
	virtual void Update() override;
	CLONE(CItemScript);

public:
	void SetItemCount(int iCount);
	void SetItemIncrease(int iAmount);
	bool CheckItemCount(int iCount);
	int GetItemCount();
	int GetRemainCount();
	int GetMaxCount();

	void SetItemTex(Ptr<CTexture> pTex);

	void SetFontObject(CGameObject* pCountObj);

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType();

	Ptr<CTexture> GetItemIcon();

	bool Addable();
};

