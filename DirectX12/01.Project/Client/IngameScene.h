#pragma once
#include <Engine/SceneScript.h>

class CGameObject;

class CIngameScene :
	public CSceneScript
{
public:
	CIngameScene();
	virtual ~CIngameScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;


	void CreateQuickSlotUI(CGameObject* _pInventory);
	void CreatePlayerStatusUI();
	void CreateInventoryUI();
};

