#pragma once
#include <Engine/Script.h>

enum ITEM_TYPE {
	ITEM_NOTHING = 0,

	ITEM_TOOL,
	ITEM_PICKAXE,
	ITEM_AXE,
	ITEM_HAMMER,
	ITEM_TOOL_END,

	ITEM_STUFF,
	ITEM_WOOD,
	ITEM_WOODSTICK,
	ITEM_STONE,
	ITEM_LEATHER,
	ITEM_BONE,
	ITEM_STUFF_END,

	ITEM_FOOD,
	ITEM_MEAT,
	ITEM_COOKMEAT,
	ITEM_FOOD_END,

	ITEM_USABLE,
	ITEM_USABLE_END,

	ITEM_END
};

class CItemScript :
	public CScript
{
	Ptr<CTexture>		m_pIconTex;
	Ptr<CTexture>		m_pIconTex_B;

	bool				m_bShowBackTex;

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
	void SetShowTexWithBackground(bool bShow);
	void SetItemCount(int iCount);
	void SetItemIncrease(int iAmount);
	int GetItemCount();
	int GetRemainCount();
	int GetMaxCount();

	void SetItemTex(Ptr<CTexture> pTex, Ptr<CTexture> pTexBack);

	void SetFontObject(CGameObject* pCountObj);

	void SetItemType(ITEM_TYPE eType);
	ITEM_TYPE GetItemType();

	bool Addable();
};

