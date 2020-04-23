#pragma once
#include "header.h"
#include <Engine/SceneScript.h>

class CGameObject;

class CIngameScene :
	public CSceneScript
{
public:
	static concurrency::concurrent_unordered_set<unsigned short> m_cusLoginList; // �α����� �÷��̾�� 
	static concurrency::concurrent_unordered_map<unsigned short, CGameObject*> m_cumPlayer; // �α����� �÷��̾��� �� ���µ�
public:
	CIngameScene();
	virtual ~CIngameScene();

	// CSceneScript��(��) ���� ��ӵ�
	virtual void Init() override;


	void CreateQuickSlotUI(CGameObject* _pInventory);
	void CreatePlayerStatusUI();
	void CreateInventoryUI();
};

