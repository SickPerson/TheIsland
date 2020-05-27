#pragma once
#include <Engine/Script.h>

enum ITEM_TYPE {
	ITEM_NOTHING,
	ITEM_TOOL,
	ITEM_STUFF,
	ITEM_FOOD,
	ITEM_USABLE,
	ITEM_END
};

class CItemScript :
	public CScript
{
	Ptr<CTexture>		m_pIconTex;
	Ptr<CTexture>		m_pIconTex_B;

	int					m_iCount;
	ITEM_TYPE			m_eItemType;
	bool				m_bShowBackTex;

	CGameObject*		m_pCountObj;
public:
	CItemScript();
	virtual ~CItemScript();
public:
	virtual void Use() {}
	virtual void Update() override;
	CLONE(CItemScript);

public:
	void SetShowTexWithBackground(bool bShow);
	void SetItemCount(int iCount);
	void SetItemIncrease(int iAmount);
	int GetItemCount();

	void SetItemTex(Ptr<CTexture> pTex, Ptr<CTexture> pTexBack);

	void SetItemType(ITEM_TYPE eType);
};

