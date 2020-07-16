#pragma once
#include <Engine/Script.h>

class CItemScript;
class CItemLootScript;

class CInventoryScript :
	public CScript
{
	vector<CGameObject*>	m_vecItemSlot;
	vector<CItemScript*>	m_vecItem;

	vector<CGameObject*>	m_vecRecipe;
	UINT					m_iRecipePage;

	bool					m_bActive;

	bool					m_bClick;
	int						m_iClickIdx;
	CGameObject*			m_pClickObj;

	CItemLootScript*		m_pItemLootScript;

public:
	CInventoryScript();
	virtual ~CInventoryScript();

	CLONE(CInventoryScript);

	virtual void Update() override;

public:
	void AddSlot(CGameObject* pObject);

	void AddItem(CItemScript* pItem, int iCount = 1);

	bool GetInventoryActive() { return m_bActive; }
	
	void SetItemLootScript(CItemLootScript* pScript);
	void Show();

private:
	void AddItemFunc(CItemScript* pItem, int iCount);

	void RecipeInit();
	void ShowRecipe();
};

