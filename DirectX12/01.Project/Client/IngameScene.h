#pragma once
#include <Engine/SceneScript.h>

#include <concurrent_unordered_map.h>
using namespace concurrency;

class CGameObject;

class CIngameScene :
	public CSceneScript
{
	CGameObject* m_pChat;
	CGameObject* m_pPlayer;
	CGameObject* m_pInventory;
	CGameObject* m_pQuickSlot;

	int m_iSelect = -1;
	bool m_bShowCursor = false;
	bool m_bShowMRT = false;

	CGameObject* m_pMRT;

	map<UINT, CGameObject*> m_mapAnimals;
public:
	concurrent_unordered_map<unsigned int, CGameObject*> m_cumPlayer;
	concurrent_unordered_map<unsigned int, CGameObject*> m_cumAnimal;

public:
	CIngameScene();
	virtual ~CIngameScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;

	void GiveStartItem();

	void CreateQuickSlotUI(CGameObject* _pInventory);
	void CreatePlayerStatusUI();
	void CreateInventoryUI();
	void CreateItemUI();
	void CreateChatUI();

	void CreateNatural();
	void CreateAnimalSpawner();

	void AnimalUpdate(UINT uiId, Vec3 vPos, Vec3 vRot, UINT uiType);
	void AnimalDestory (UINT uiId);
};

