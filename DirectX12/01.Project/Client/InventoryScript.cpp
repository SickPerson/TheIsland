#include "stdafx.h"
#include "InventoryScript.h"

#include "ItemScript.h"
#include "ItemLootScript.h"
#include "RecipeScript.h"

#include "StuffScript.h"
#include "ToolItemScript.h"
#include "UsableScript.h"
#include "HousingScript.h"
#include "ArmorScript.h"

#include <Engine/Font.h>

#include <Engine/RenderMgr.h>
#include <iostream>

CInventoryScript::CInventoryScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bActive(false),
	m_bClick(false),
	m_pClickObj(NULL),
	m_iClickIdx(-1),
	m_pItemLootScript(NULL),
	m_iRecipePage(0),
	m_bAddable(true),
	m_pNextPage(NULL),
	m_pPrevPage(NULL)
{
	m_vecItemSlot.reserve(25);
	m_vecItem.reserve(25);
	m_vecRecipe.reserve(25);

	RecipeInit();
}


CInventoryScript::~CInventoryScript()
{
	m_vecItemSlot.clear();
	m_vecItem.clear();
	m_vecRecipe.clear();
	SAFE_DELETE(m_pNextPage);
	SAFE_DELETE(m_pPrevPage);
	SAFE_DELETE(m_pClickObj);
}

void CInventoryScript::Update()
{
	if (m_bActive)
	{
		if (KEY_TAB(KEY_TYPE::KEY_LBTN))
		{
			POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
			tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

			Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);

			// 아이템 클릭
			for (int i = 0; i < m_vecItemSlot.size(); ++i)
			{
				Vec3 vPos = m_vecItemSlot[i]->Transform()->GetLocalPos();
				Vec3 vScale = m_vecItemSlot[i]->Transform()->GetLocalScale();

				if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
				{
					if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
					{
						if (m_vecItem[i] == NULL)
							break;

						m_bClick = true;
						m_iClickIdx = i;
						m_vecItem[i]->GetObj()->ClearParent(NULL);
						m_pClickObj = m_vecItem[i]->GetObj();
						break;
					}
				}
			}

			if (CheckClickUI(vMousePos, m_pNextPage))
			{
				if (m_iRecipePage < m_vecRecipe.size() / 6)
				{
					m_iRecipePage++;
					ShowRecipe();
				}
			}
			else if(CheckClickUI(vMousePos, m_pPrevPage))
			{
				if (m_iRecipePage > 0)
				{
					m_iRecipePage--;
					ShowRecipe();
				}
			}
			// 조합 클릭
			else if (m_bAddable)
			{
				for (int num = m_iRecipePage * 6; num < m_iRecipePage * 6 + 6; ++num)
				{
					if (m_vecRecipe.size() <= num)
					{
						break;
					}
					Vec3 vPos = m_vecRecipe[num]->GetScript<CRecipeScript>()->GetClickPosition();
					Vec3 vScale = m_vecRecipe[num]->GetScript<CRecipeScript>()->GetClickScale();

					if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
					{
						if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
						{
							vector<tItemRecipe> vecRecipe = m_vecRecipe[num]->GetScript<CRecipeScript>()->GetRecipe();
							vector<bool> vecCheck;
							vector<int> vecIndex;
							vecCheck.resize(vecRecipe.size());
							vecIndex.resize(vecRecipe.size());
							// 조합 조건 체크
							for (int recipe = 0; recipe < vecRecipe.size(); ++recipe)
							{
								vecCheck[recipe] = false;
								for (int item = 0; item < m_vecItemSlot.size(); ++item)
								{
									if (m_vecItem[item] == NULL)
										continue;

									if (m_vecItem[item]->GetItemType() == vecRecipe[recipe].eItem)
									{
										if (m_vecItem[item]->CheckItemCount(vecRecipe[recipe].iCount))
										{
											vecCheck[recipe] = true;
											vecIndex[recipe] = item;
											break;
										}
									}
								}
							}
							bool check = true;
							// 조합 조건 체크2
							for (int i = 0; i < vecCheck.size(); ++i)
							{
								if (vecCheck[i] == false)
								{
									check = false;
									break;
								}
							}
							// 아이템 조합 조건 충족
							if (check == true)
							{
								ITEM_TYPE eType = m_vecRecipe[num]->GetScript<CRecipeScript>()->GetItemType();
								if (eType > ITEM_TOOL && eType < ITEM_TOOL_END)
								{
									CItemScript* pItem = new CToolItemScript(eType);
									AddItem(pItem, 1);
								}
								else if (eType > ITEM_STUFF && eType < ITEM_STUFF_END)
								{
									CItemScript* pItem = new CStuffScript(eType);
									AddItem(pItem, 1);
								}
								else if (eType > ITEM_USABLE && eType < ITEM_USABLE_END)
								{
									CItemScript* pItem = new CUsableScript(eType);
									AddItem(pItem, 1);
								}
								else if (eType > ITEM_HOUSING && eType < ITEM_HOUSING_END)
								{
									CItemScript* pItem = new CHousingScript(eType);
									AddItem(pItem, 1);
								}
								else if (eType > ITEM_ARMOR && eType < ITEM_ARMOR_END)
								{
									CItemScript* pItem = new CArmorScript(eType);
									AddItem(pItem, 1);
								}
								// 재료 아이템 제거
								for (int i = 0; i < vecIndex.size(); ++i)
								{
									if (!m_vecItem[vecIndex[i]]->SetItemIncrease(-vecRecipe[i].iCount))
									{
										m_vecItem[vecIndex[i]] = NULL;
										m_bAddable = true;
									}
								}
							}
							break;
						}
					}
				}

			} // if(m_bAddable)
			
		}

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN) && m_bClick)
		{
			POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
			tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

			Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);

			Vec3 vPos = m_vecItemSlot[m_iClickIdx]->Transform()->GetLocalPos();
			Vec3 vScale = m_vecItemSlot[m_iClickIdx]->Transform()->GetLocalScale();

			//m_pClickObj->Transform()->SetLocalPos(Vec3((vMousePos.x - vPos.x) / vScale.x, (vMousePos.y - vPos.y) / vScale.y, -10.f));
			m_pClickObj->Transform()->SetLocalPos(Vec3(vMousePos.x, vMousePos.y, 10.f));
			m_pClickObj->Transform()->SetLocalScale(vScale);
		}

		if (KEY_AWAY(KEY_TYPE::KEY_LBTN) && m_bClick)
		{
			POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
			tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

			Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);

			// 아이템 이동 종료
			for (int i = 0; i < m_vecItemSlot.size(); ++i)
			{
				Vec3 vPos = m_vecItemSlot[i]->Transform()->GetLocalPos();
				Vec3 vScale = m_vecItemSlot[i]->Transform()->GetLocalScale();

				if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
				{
					if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
					{
						if (m_vecItem[i] == NULL)
						{
							m_vecItem[m_iClickIdx]->GetObj()->ClearParent(m_vecItemSlot[i]);

							m_vecItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vecItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
						}
						else if (m_vecItem[i]->GetItemType() == m_vecItem[m_iClickIdx]->GetItemType())
						{
							if (m_vecItem[i]->Addable())
							{
								int iRemain = m_vecItem[i]->GetRemainCount();
								int iCount = m_vecItem[m_iClickIdx]->GetItemCount();
								if (iCount > iRemain)
								{
									m_vecItem[i]->SetItemIncrease(iRemain);
									m_vecItem[m_iClickIdx]->SetItemCount(iCount - iRemain);

									m_vecItem[m_iClickIdx]->GetObj()->ClearParent(m_vecItemSlot[m_iClickIdx]);

									m_vecItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
									m_vecItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

									m_bClick = false;
									m_iClickIdx = -1;
									m_pClickObj = NULL;

									return;
								}
								else
								{
									m_vecItem[i]->SetItemIncrease(iCount);

									tEvent tEv;
									tEv.eType = EVENT_TYPE::DELETE_OBJECT;
									tEv.wParam = (DWORD_PTR)m_vecItem[m_iClickIdx]->GetObj();
									CEventMgr::GetInst()->AddEvent(tEv);
									m_vecItem[m_iClickIdx] = NULL;

									m_bClick = false;
									m_iClickIdx = -1;
									m_pClickObj = NULL;

									return;
								}
							}
							else
							{
								m_vecItem[i]->GetObj()->ClearParent(m_vecItemSlot[m_iClickIdx]);
								m_vecItem[m_iClickIdx]->GetObj()->ClearParent(m_vecItemSlot[i]);

								m_vecItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
								m_vecItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

								m_vecItem[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
								m_vecItem[i]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

								CItemScript* pTemp = m_vecItem[i];
								m_vecItem[i] = m_vecItem[m_iClickIdx];
								m_vecItem[m_iClickIdx] = pTemp;

								m_bClick = false;
								m_iClickIdx = -1;
								m_pClickObj = NULL;
								return;
							}
						}
						else
						{
							m_vecItem[i]->GetObj()->ClearParent(m_vecItemSlot[m_iClickIdx]);
							m_vecItem[m_iClickIdx]->GetObj()->ClearParent(m_vecItemSlot[i]);

							m_vecItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vecItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

							m_vecItem[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vecItem[i]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
						}

						CItemScript* pTemp = m_vecItem[i];
						m_vecItem[i] = m_vecItem[m_iClickIdx];
						m_vecItem[m_iClickIdx] = pTemp;

						m_bClick = false;
						m_iClickIdx = -1;
						m_pClickObj = NULL;
						return;
					}
				}
			}

			m_vecItemSlot[m_iClickIdx]->AddChild(m_pClickObj);

			m_vecItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
			m_vecItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

			m_bClick = false;
			m_iClickIdx = -1;
			m_pClickObj = NULL;
		}
	}
}

void CInventoryScript::AddSlot(CGameObject * pObject)
{
	m_vecItemSlot.emplace_back(pObject);
	m_vecItem.push_back(NULL);
}

void CInventoryScript::AddItem(CItemScript * pItem, int iCount)
{
	m_pItemLootScript->GetItemNotify(pItem->GetItemIcon(), pItem->GetName(), iCount);
	AddItemFunc(pItem, iCount);
}

void CInventoryScript::SetItemLootScript(CItemLootScript * pScript)
{
	m_pItemLootScript = pScript;
}

void CInventoryScript::Show()
{
	if (m_bActive)
	{
		TransferLayer(29, true);
		m_bActive = false;
		HideRecipe();
	}
	else
	{
		TransferLayer(30, true);
		m_bActive = true;
		m_iRecipePage = 0;
		ShowRecipe();
	}
}

void CInventoryScript::OnAddable(int index)
{
	m_vecItem[index] = NULL;
	m_bAddable = true;
}

void CInventoryScript::Use_Left(CGameObject * pHost, CGameObject * pObj, int index)
{
	if (index == -1 || m_vecItem[index] == NULL)
		return;
	m_vecItem[index]->Use_Left(pHost, pObj, index);
}

void CInventoryScript::Use_Right(CGameObject * pHost, CGameObject * pObj, int index)
{
	if (index == -1 || m_vecItem[index] == NULL)
		return;
	m_vecItem[index]->Use_Right(pHost, pObj, index);
}

void CInventoryScript::Use_Highlight(CGameObject * pHost, CGameObject * pObj, int index)
{
	if (index == -1 || m_vecItem[index] == NULL)
		return;
	m_vecItem[index]->Use_Highlight(pHost, pObj, index);
}

void CInventoryScript::DisableItem(CGameObject* pHost, int index)
{
	if (index < 0 || m_vecItem[index] == NULL)
		return;
	m_vecItem[index]->DisableItem(pHost, index);
}

void CInventoryScript::EnableItem(CGameObject* pHost, int index)
{
	if (index < 0 || m_vecItem[index] == NULL)
		return;
	m_vecItem[index]->EnableItem(pHost, index);
}

void CInventoryScript::AddItemFunc(CItemScript * pItem, int iCount)
{
	int iIdx = -1;
	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		if (m_vecItem[i] == NULL)
		{
			if (iIdx == -1)
				iIdx = i;
		}
		else
		{
			if (m_vecItem[i]->GetItemType() == pItem->GetItemType())
			{
				if (m_vecItem[i]->Addable())
				{
					int iRemain = m_vecItem[i]->GetRemainCount();
					if (iRemain >= iCount)
					{
						m_vecItem[i]->SetItemIncrease(iCount);
						return;
					}
					else
					{
						m_vecItem[i]->SetItemIncrease(iRemain);
						AddItem(pItem, iCount - iRemain);
						return;
					}
				}
			}
		}
	}
	if (iIdx != -1 && m_bAddable)
	{
		int iMax = pItem->GetMaxCount();
		int iRemain = 0;
		if (iCount > iMax)
		{
			iRemain = iCount - iMax;
			iCount = iMax;
		}

		CGameObject* pObj = new CGameObject;

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(pItem);

		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMtrl")->Clone());

		m_vecItem[iIdx] = pItem;

		Vec3 vPos = m_vecItemSlot[iIdx]->Transform()->GetLocalPos();
		Vec3 vScale = m_vecItemSlot[iIdx]->Transform()->GetLocalScale();

		vPos = Vec3(0.f, 0.f, 0.f);
		vScale = Vec3(1.f, 1.f, 1.f);

		m_vecItem[iIdx]->Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, -10.f));
		m_vecItem[iIdx]->Transform()->SetLocalScale(vScale);

		m_vecItemSlot[iIdx]->AddChild(pObj);

		CGameObject* pCountObj = new CGameObject;
		pCountObj->AddComponent(new CTransform);
		pCountObj->AddComponent(new CFont);

		//pCountObj->Font()->SetString(std::to_string(m_vItem[iIdx]->GetItemCount()));

		pCountObj->Transform()->SetLocalPos(Vec3(-0.31f, 0.35f, -1.f));
		pCountObj->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 1.f));
		pObj->AddChild(pCountObj);
		m_vecItem[iIdx]->SetFontObject(pCountObj);

		m_vecItem[iIdx]->SetItemCount(iCount);

		if (iIdx < 5 || m_bActive)
		{
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObj);
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pCountObj);
		}
		else
		{
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObj);
			CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pCountObj);
		}

		if (iRemain > 0)
		{
			AddItem(pItem->Clone(), iRemain);
		}
		CheckAddable();
		return;
	}
}

bool CInventoryScript::CheckClickUI(Vec2 vMousePos, CGameObject * pObj)
{
	Vec3 vPos = pObj->Transform()->GetLocalPos();
	Vec3 vScale = pObj->Transform()->GetLocalScale();

	if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
	{
		if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
		{
			return true;
		}
	}

	return false;
}

void CInventoryScript::CheckAddable()
{
	bool bAddable = false;
	for (int i = 0; i < m_vecItemSlot.size(); ++i)
	{
		if (m_vecItem[i] == NULL)
		{
			bAddable = true;
			break;
		}
	}
	m_bAddable = bAddable;
}

void CInventoryScript::ShowRecipe()
{
	HideRecipe();
	for (int i = 0; i < 6; ++i)
	{
		if (m_vecRecipe.size() <= i + (m_iRecipePage * 6))
		{
			break;
		}
		m_vecRecipe[i + (m_iRecipePage * 6)]->GetScript<CRecipeScript>()->Show(true);
		m_vecRecipe[i + (m_iRecipePage * 6)]->GetScript<CRecipeScript>()->SetNum(i);
	}
}

void CInventoryScript::HideRecipe()
{
	for (CGameObject* pRecipe : m_vecRecipe)
	{
		pRecipe->GetScript<CRecipeScript>()->Show(false);
	}
}

void CInventoryScript::Init()
{
	m_pNextPage = new CGameObject;
	m_pNextPage->AddComponent(new CTransform);
	m_pNextPage->AddComponent(new CFont);
	m_pNextPage->Font()->SetString("Next");
	m_pNextPage->Font()->SetBackColor(Vec4(0.5f, 0.5f, 0.8f, 1.f));
	m_pNextPage->SetName(L"Next Page");
	m_pNextPage->Transform()->SetLocalPos(Vec3(560.f, -210.f, 50.f));
	m_pNextPage->Transform()->SetLocalScale(Vec3(60.f, 20.f, 1.f));
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(m_pNextPage);
	GetObj()->AddChild(m_pNextPage);

	m_pPrevPage = new CGameObject;
	m_pPrevPage->AddComponent(new CTransform);
	m_pPrevPage->AddComponent(new CFont);
	m_pPrevPage->Font()->SetString("Prev");
	m_pPrevPage->Font()->SetBackColor(Vec4(0.5f, 0.5f, 0.8f, 1.f));
	m_pPrevPage->SetName(L"Prev Page");
	m_pPrevPage->Transform()->SetLocalPos(Vec3(380.f, -210.f, 50.f));
	m_pPrevPage->Transform()->SetLocalScale(Vec3(60.f, 20.f, 1.f));
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(m_pPrevPage);
	GetObj()->AddChild(m_pPrevPage);
}


void CInventoryScript::RecipeInit()
{
	UINT iNum = 0;

	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_PICKAXE));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_AXE));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_HAMMER));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_MACHETTE));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_BOW));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_ARROW));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_CAMPFIRE));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_MAP));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_BANDAGE));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_TSHIRT));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_SHIRT));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_JACKET));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);

	pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->SetName(L"Item Recipe");
	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	pObject->AddComponent(new CRecipeScript(ITEM_TYPE::ITEM_BLUEPRINT));
	pObject->GetScript<CRecipeScript>()->Init();
	pObject->GetScript<CRecipeScript>()->SetNum(iNum++);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	m_vecRecipe.emplace_back(pObject);
}

