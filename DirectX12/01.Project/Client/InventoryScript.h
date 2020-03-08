#pragma once
#include <Engine/Script.h>

class CInventoryScript :
	public CScript
{
	vector<CGameObject*> m_vItemSlot;

	bool m_bActive;

public:
	CInventoryScript();
	virtual ~CInventoryScript();

	CLONE(CInventoryScript);

	virtual void Update() override;

public:
	void AddSlot(CGameObject* pObject);
	bool GetInventoryActive() { return m_bActive; }
};

