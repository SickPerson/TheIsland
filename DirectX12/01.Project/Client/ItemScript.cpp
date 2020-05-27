#include "stdafx.h"
#include "ItemScript.h"

#include <Engine/Ptr.h>
#include <Engine/Texture.h>
#include <Engine/GameObject.h>


CItemScript::CItemScript()
	: CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT)
	, m_pIconTex(NULL)
	, m_pIconTex_B(NULL)
	, m_iCount(-1)
	, m_eItemType(ITEM_NOTHING)
	, m_bShowBackTex(false)
	, m_pCountObj(NULL)
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
	m_iCount = iCount;
}

void CItemScript::SetItemIncrease(int iAmount)
{
	m_iCount += iAmount;
}

int CItemScript::GetItemCount()
{
	return m_iCount;
}

void CItemScript::SetItemTex(Ptr<CTexture> pTex, Ptr<CTexture> pTexBack)
{
	m_pIconTex = pTex;
	m_pIconTex_B = pTexBack;
}

void CItemScript::SetItemType(ITEM_TYPE eType)
{
	m_eItemType = eType;
}
