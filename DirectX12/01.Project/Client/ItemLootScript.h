#pragma once
#include <Engine/Script.h>

#define MAX_ACTIVE_TIME 3.f

class CItemLootScript :
	public CScript
{
	bool				m_bEnable;
	float				m_fActiveTime;

	CFont*				m_pFont;
	Ptr<CMaterial>		m_pIcon;

	wstring				m_strPrevItemName;
	int					m_iPrevCount;

	CGameObject*		pBackground;
public:
	CItemLootScript();
	virtual ~CItemLootScript();

	CLONE(CItemLootScript);

	virtual void Update() override;

public:
	void SetItemLootFont(CFont* pFont);
	void SetItemLootIcon(Ptr<CMaterial> pMaterial);

	void GetItemNotify(Ptr<CTexture> pTex, wstring strItemName, int iCount);
	void SetBackgroundObject(CGameObject* pObject);
};

