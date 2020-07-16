#pragma once
#include <Engine/Script.h>
#include "Item.h"

class CRecipeScript :
	public CScript
{
	ITEM_TYPE	m_eType;
	vector<tItemRecipe> m_vecRecipe;

	UINT		m_iNum;
	bool		m_bActive;

	Vec3		m_vClickPos;
	Vec3		m_vClickScale;

public:
	CRecipeScript(ITEM_TYPE eType);
	virtual ~CRecipeScript();

public:
	virtual void Update() override;
	CLONE(CRecipeScript);

public:
	void Init();
	void SetNum(UINT iNum);

	void Show(bool bActive);
	vector<tItemRecipe> GetRecipe();

	Vec3 GetClickPosition();
	Vec3 GetClickScale();

	string GetRecipeName();
	ITEM_TYPE GetItemType();
};

