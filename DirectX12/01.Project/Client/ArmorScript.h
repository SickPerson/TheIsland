#pragma once
#include "ItemScript.h"

#define ARMOR_MAX_COUNT 1

class CArmorScript :
	public CItemScript
{
	float	m_fArmor;
	CGameObject* m_pHost;
public:
	CArmorScript(ITEM_TYPE eType, int iCount = 1);
	virtual ~CArmorScript();

public:
	// CItemScript을(를) 통해 상속됨
	CLONE(CArmorScript);

	virtual void Update() override;
	virtual UINT Use_Right(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT Use_Left(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) override;
	virtual UINT EnableItem(CGameObject* pHost, int num) override { return 0; };
	virtual void DisableItem(CGameObject* pHost, int num) override {};

public:
	void EquipArmor();
	void DestroyArmor();
	void SetArmorValue(float fValue);
};

