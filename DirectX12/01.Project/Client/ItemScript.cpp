#include "stdafx.h"
#include "ItemScript.h"

#include <Engine/Ptr.h>
#include <Engine/Texture.h>
#include <Engine/GameObject.h>
#include <Engine/Font.h>


CItemScript::CItemScript(UINT iItemType)
	: CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT)
	, m_iCount(-1)
	, m_eItemType(ITEM_TYPE(iItemType))
	, m_pCountObj(NULL)
	, m_iMaxCount(1)
	, m_bInit(false)
{
	m_pIconTex = GetItemIconByType((ITEM_TYPE)iItemType);
	string strName = GetItemNameByType((ITEM_TYPE)iItemType);
	wstring wstrName;
	wstrName.assign(strName.begin(), strName.end());
	SetName(wstrName);
}


CItemScript::~CItemScript()
{
}

void CItemScript::Update()
{
	if (m_bInit == false)
	{
		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pIconTex.GetPointer());
		m_bInit = true;
	}
}

void CItemScript::SetItemCount(int iCount)
{
	if (iCount > m_iMaxCount)
		m_iCount = iCount;
	else
		m_iCount = iCount;

	if (m_pCountObj)
		m_pCountObj->Font()->SetString(std::to_string(m_iCount));
}

bool CItemScript::SetItemIncrease(int iAmount)
{
	if (m_iCount + iAmount > m_iMaxCount)
		m_iCount = m_iMaxCount;
	else
		m_iCount += iAmount;

	if (m_pCountObj)
		m_pCountObj->Font()->SetString(std::to_string(m_iCount));

	if (m_iCount <= 0)
	{
		// ¾ÆÀÌÅÛ ¼Ò¸ê
		GetObj()->ClearParent();
		tEvent tEv;
		tEv.eType = EVENT_TYPE::DELETE_OBJECT;
		tEv.wParam = (DWORD_PTR)GetObj();
		CEventMgr::GetInst()->AddEvent(tEv);
		return false;
	}
	return true;
}

bool CItemScript::CheckItemCount(int iCount)
{
	if (m_iCount >= iCount)
		return true;
	return false;
}

int CItemScript::GetItemCount()
{
	return m_iCount;
}

int CItemScript::GetRemainCount()
{
	return m_iMaxCount - m_iCount;
}

int CItemScript::GetMaxCount()
{
	return m_iMaxCount;
}

void CItemScript::SetItemTex(Ptr<CTexture> pTex)
{
	m_pIconTex = pTex;
}

void CItemScript::SetFontObject(CGameObject* pCountObj)
{
	m_pCountObj = pCountObj;
}

void CItemScript::SetItemType(ITEM_TYPE eType)
{
	m_eItemType = eType;
	SetItemTex(GetItemIconByType(eType));
}

ITEM_TYPE CItemScript::GetItemType()
{
	return m_eItemType;
}

Ptr<CTexture> CItemScript::GetItemIcon()
{
	return m_pIconTex;
}

bool CItemScript::Addable()
{
	if (m_iCount < m_iMaxCount)
		return true;

	return false;
}
