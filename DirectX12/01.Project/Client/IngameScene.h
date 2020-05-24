#pragma once
#include <Engine/SceneScript.h>

class CGameObject;

class CIngameScene :
	public CSceneScript
{
	CGameObject* m_pChat;
	CGameObject* m_pPlayer;
	CGameObject* m_pInventory;
	CGameObject* m_pQuickSlot;

public:
	CIngameScene();
	virtual ~CIngameScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;


	void CreateQuickSlotUI(CGameObject* _pInventory);
	void CreatePlayerStatusUI();
	void CreateInventoryUI();
	void CreateChatUI();
};

