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
	virtual UINT Use_Right(CGameObject* pHost, CGameObject* pObj, int num) = 0;
	virtual UINT Use_Left(CGameObject* pHost, CGameObject* pObj, int num) = 0;
	virtual void Use_Highlight(CGameObject* pHost, CGameObject* pObj, int num) = 0;
	virtual UINT EnableItem(CGameObject* pHost, int num) = 0;
	virtual void DisableItem(CGameObject* pHost, int num) = 0;
	virtual void Update() override;
	virtual CItemScript* Clone() = 0;

public:
	void SetItemCount(int iCount);
	bool SetItemIncrease(int iAmount);
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

