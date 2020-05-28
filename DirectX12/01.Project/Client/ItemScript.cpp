#include "stdafx.h"
#include "ItemScript.h"

#include <Engine/Ptr.h>
#include <Engine/Texture.h>
#include <Engine/GameObject.h>
#include <Engine/Font.h>


CItemScript::CItemScript(UINT iItemType)
	: CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT)
	, m_pIconTex(NULL)
	, m_pIconTex_B(NULL)
	, m_iCount(-1)
	, m_eItemType(ITEM_TYPE(iItemType))
	, m_bShowBackTex(false)
	, m_pCountObj(NULL)
	, m_iMaxCount(1)
{
}


CItemScript::~CItemScript()
{
}

void CItemScript::Update()
{
	if(!m_bShowBackTex)
		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pIconTex.GetPointer());
	else
		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pIconTex_B.GetPointer());
}

void CItemScript::SetShowTexWithBackground(bool bShow)
{
	m_bShowBackTex = bShow;
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

void CItemScript::SetItemIncrease(int iAmount)
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
	}
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

void CItemScript::SetItemTex(Ptr<CTexture> pTex, Ptr<CTexture> pTexBack)
{
	m_pIconTex = pTex;
	m_pIconTex_B = pTexBack;
}

void CItemScript::SetFontObject(CGameObject* pCountObj)
{
	m_pCountObj = pCountObj;
}

void CItemScript::SetItemType(ITEM_TYPE eType)
{
	m_eItemType = eType;
}

ITEM_TYPE CItemScript::GetItemType()
{
	return m_eItemType;
}

bool CItemScript::Addable()
{
	if (m_iCount < m_iMaxCount)
		return true;

	return false;
}
