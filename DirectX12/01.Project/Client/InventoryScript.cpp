#include "stdafx.h"
#include "InventoryScript.h"

#include "ItemScript.h"


CInventoryScript::CInventoryScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bActive(false)
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

}

void CInventoryScript::AddSlot(CGameObject * pObject)
{
	m_vItemSlot.emplace_back(pObject);
	m_vItem.push_back(NULL);
}

void CInventoryScript::AddItem(CItemScript * pItem, int iCount)
{
	for (int i = 0; i < m_vItemSlot.size(); ++i)
	{
		if (m_vItem[i] == NULL)
		{
			CGameObject* pObj = new CGameObject;

			pObj->AddComponent(new CTransform);
			pObj->AddComponent(new CMeshRender);
			pObj->AddComponent(pItem);

			pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMtrl")->Clone());

			m_vItem[i] = pItem;
			m_vItem[i]->SetItemCount(iCount);

			Vec3 vPos = m_vItemSlot[i]->Transform()->GetLocalPos();
			Vec3 vScale = m_vItemSlot[i]->Transform()->GetLocalScale();

			vPos = Vec3(0.f, 0.f, 0.f);
			vScale = Vec3(1.f, 1.f, 1.f);

			m_vItem[i]->Transform()->SetLocalPos(Vec3(vPos.x, vPos.y, 1.f));
			m_vItem[i]->Transform()->SetLocalScale(vScale);

			m_vItemSlot[i]->AddChild(pObj);

			if (i < 5)
			{
				CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObj);
			}
			else
			{
				CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObj);
			}
			return;
		}
	}
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
