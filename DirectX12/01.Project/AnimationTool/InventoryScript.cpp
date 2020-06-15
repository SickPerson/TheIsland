#include "stdafx.h"
#include "InventoryScript.h"

#include "ItemScript.h"
#include "ItemLootScript.h"

#include <Engine/Font.h>

#include <Engine/RenderMgr.h>


CInventoryScript::CInventoryScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bActive(false),
	m_bClick(false),
	m_pClickObj(NULL),
	m_iClickIdx(-1),
	m_pItemLootScript(NULL)
{
	m_vItemSlot.reserve(25);
	m_vItem.reserve(25);
}


CInventoryScript::~CInventoryScript()
{
	m_vItemSlot.clear();
	m_vItem.clear();
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

			for (int i = 0; i < m_vItemSlot.size(); ++i)
			{
				Vec3 vPos = m_vItemSlot[i]->Transform()->GetLocalPos();
				Vec3 vScale = m_vItemSlot[i]->Transform()->GetLocalScale();

				if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
				{
					if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
					{
						if (m_vItem[i] == NULL)
							break;

						m_bClick = true;
						m_iClickIdx = i;
						m_vItem[i]->GetObj()->ClearParent(NULL);
						m_pClickObj = m_vItem[i]->GetObj();
						break;
					}
				}
			}
		}

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN) && m_bClick)
		{
			POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
			tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

			Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);

			Vec3 vPos = m_vItemSlot[m_iClickIdx]->Transform()->GetLocalPos();
			Vec3 vScale = m_vItemSlot[m_iClickIdx]->Transform()->GetLocalScale();

			//m_pClickObj->Transform()->SetLocalPos(Vec3((vMousePos.x - vPos.x) / vScale.x, (vMousePos.y - vPos.y) / vScale.y, -10.f));
			m_pClickObj->Transform()->SetLocalPos(Vec3(vMousePos.x, vMousePos.y, 10.f));
			m_pClickObj->Transform()->SetLocalScale(vScale);
		}

		if (KEY_AWAY(KEY_TYPE::KEY_LBTN) && m_bClick)
		{
			POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
			tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

			Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);

			for (int i = 0; i < m_vItemSlot.size(); ++i)
			{
				Vec3 vPos = m_vItemSlot[i]->Transform()->GetLocalPos();
				Vec3 vScale = m_vItemSlot[i]->Transform()->GetLocalScale();

				if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
				{
					if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
					{
						if (m_vItem[i] == NULL)
						{
							m_vItem[m_iClickIdx]->GetObj()->ClearParent(m_vItemSlot[i]);

							m_vItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
						}
						else if (m_vItem[i]->GetItemType() == m_vItem[m_iClickIdx]->GetItemType())
						{
							if (m_vItem[i]->Addable())
							{
								int iRemain = m_vItem[i]->GetRemainCount();
								int iCount = m_vItem[m_iClickIdx]->GetItemCount();
								if (iCount > iRemain)
								{
									m_vItem[i]->SetItemIncrease(iRemain);
									m_vItem[m_iClickIdx]->SetItemCount(iCount - iRemain);

									m_vItem[m_iClickIdx]->GetObj()->ClearParent(m_vItemSlot[m_iClickIdx]);

									m_vItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
									m_vItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

									m_bClick = false;
									m_iClickIdx = -1;
									m_pClickObj = NULL;

									return;
								}
								else
								{
									m_vItem[i]->SetItemIncrease(iCount);

									tEvent tEv;
									tEv.eType = EVENT_TYPE::DELETE_OBJECT;
									tEv.wParam = (DWORD_PTR)m_vItem[m_iClickIdx]->GetObj();
									CEventMgr::GetInst()->AddEvent(tEv);
									m_vItem[m_iClickIdx] = NULL;

									m_bClick = false;
									m_iClickIdx = -1;
									m_pClickObj = NULL;

									return;
								}
							}
							else
							{
								m_vItem[i]->GetObj()->ClearParent(m_vItemSlot[m_iClickIdx]);
								m_vItem[m_iClickIdx]->GetObj()->ClearParent(m_vItemSlot[i]);

								m_vItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
								m_vItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

								m_vItem[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
								m_vItem[i]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

								CItemScript* pTemp = m_vItem[i];
								m_vItem[i] = m_vItem[m_iClickIdx];
								m_vItem[m_iClickIdx] = pTemp;

								m_bClick = false;
								m_iClickIdx = -1;
								m_pClickObj = NULL;
								return;
							}
						}
						else
						{
							m_vItem[i]->GetObj()->ClearParent(m_vItemSlot[m_iClickIdx]);
							m_vItem[m_iClickIdx]->GetObj()->ClearParent(m_vItemSlot[i]);

							m_vItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

							m_vItem[i]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
							m_vItem[i]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
						}

						CItemScript* pTemp = m_vItem[i];
						m_vItem[i] = m_vItem[m_iClickIdx];
						m_vItem[m_iClickIdx] = pTemp;

						m_bClick = false;
						m_iClickIdx = -1;
						m_pClickObj = NULL;
						return;
					}
				}
			}

			m_vItemSlot[m_iClickIdx]->AddChild(m_pClickObj);

			m_vItem[m_iClickIdx]->Transform()->SetLocalPos(Vec3(0.f, 0.f, -10.f));
			m_vItem[m_iClickIdx]->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

			m_bClick = false;
			m_iClickIdx = -1;
			m_pClickObj = NULL;
		}
	}
}

void CInventoryScript::AddSlot(CGameObject * pObject)
{
	m_vItemSlot.emplace_back(pObject);
	m_vItem.push_back(NULL);
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
	}
	else
	{
		TransferLayer(30, true);
		m_bActive = true;
	}
}

void CInventoryScript::AddItemFunc(CItemScript * pItem, int iCount)
{
	int iIdx = -1;
	for (int i = 0; i < m_vItemSlot.size(); ++i)
	{
		if (m_vItem[i] == NULL)
		{
			if (iIdx == -1)
				iIdx = i;
		}
		else
		{
			if (m_vItem[i]->GetItemType() == pItem->GetItemType())
			{
				if (m_vItem[i]->Addable())
				{
					int iRemain = m_vItem[i]->GetRemainCount();
					if (iRemain >= iCount)
					{
						m_vItem[i]->SetItemIncrease(iCount);
						return;
					}
					else
					{
						m_vItem[i]->SetItemIncrease(iRemain);
						AddItem(pItem, iCount - iRemain);
						return;
					}
				}
			}
		}
	}
	if (iIdx != -1)
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

		m_vItem[iIdx] = pItem;

		Vec3 vPos = m_vItemSlot[iIdx]->Transform()->GetLocalPos();
		Vec3 vScale = m_vItemSlot[iIdx]->Transform()->GetLocalScale();

		vPos = Vec3(0.f, 0.f, 0.f);
		vScale = Vec3(1.f, 1.f, 1.f);

		m_vItem[iIdx]->Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, -10.f));
		m_vItem[iIdx]->Transform()->SetLocalScale(vScale);

		m_vItemSlot[iIdx]->AddChild(pObj);

		CGameObject* pCountObj = new CGameObject;
		pCountObj->AddComponent(new CTransform);
		pCountObj->AddComponent(new CFont);

		//pCountObj->Font()->SetString(std::to_string(m_vItem[iIdx]->GetItemCount()));

		pCountObj->Transform()->SetLocalPos(Vec3(-0.31f, 0.35f, -1.f));
		pCountObj->Transform()->SetLocalScale(Vec3(0.4f, 0.4f, 1.f));
		pObj->AddChild(pCountObj);
		m_vItem[iIdx]->SetFontObject(pCountObj);

		m_vItem[iIdx]->SetItemCount(iCount);

		if (iIdx < 5)
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
		return;
	}
}
