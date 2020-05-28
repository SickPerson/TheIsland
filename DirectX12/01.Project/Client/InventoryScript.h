#pragma once
#include <Engine/Script.h>

class CItemScript;

class CInventoryScript :
	public CScript
{
	vector<CGameObject*>	m_vItemSlot;
	vector<CItemScript*>	m_vItem;

	bool					m_bActive;

	bool					m_bClick;
	int						m_iClickIdx;
	CGameObject*			m_pClickObj;

public:
	CInventoryScript();
	virtual ~CInventoryScript();

	CLONE(CInventoryScript);

	virtual void Update() override;

public:
	void AddSlot(CGameObject* pObject);

	void AddItem(CItemScript* pItem, int iCount = 1);

	bool GetInventoryActive() { return m_bActive; }
	
	void Show();
};

