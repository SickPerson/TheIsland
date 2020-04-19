#pragma once
#include "header.h"
#include <Engine/SceneScript.h>

class CGameObject;

class CIngameScene :
	public CSceneScript
{
public:
	static concurrency::concurrent_unordered_set<unsigned short> m_cusLoginList; // 로그인한 플레이어들 
	static concurrency::concurrent_unordered_map<unsigned short, CGameObject*> m_cumPlayer; // 로그인한 플레이어들과 각 상태들
public:
	CIngameScene();
	virtual ~CIngameScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;


	void CreateQuickSlotUI(CGameObject* _pInventory);
	void CreatePlayerStatusUI();
	void CreateInventoryUI();
};

