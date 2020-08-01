#include "stdafx.h"
#include "ItemLootScript.h"

#include <Engine/Font.h>
#include <Engine/Material.h>

CItemLootScript::CItemLootScript()
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT)
	, m_bEnable(false)
	, m_fActiveTime(MAX_ACTIVE_TIME)
	, m_pFont(NULL)
	, m_pIcon(NULL)
{
}


CItemLootScript::~CItemLootScript()
{
}

void CItemLootScript::Update()
{
	if (m_bEnable)
	{
		m_fActiveTime -= DT;
		if (m_fActiveTime < 0.f)
		{
			TransferLayer(29, true);
			m_bEnable = false;
			m_iPrevCount = 0;
			m_strPrevItemName.clear();

		}
	}
}

void CItemLootScript::SetItemLootFont(CFont * pFont)
{
	m_pFont = pFont;
}

void CItemLootScript::SetItemLootIcon(Ptr<CMaterial> pMaterial)
{
	m_pIcon = pMaterial;
}

void CItemLootScript::GetItemNotify(Ptr<CTexture> pTex, wstring strItemName, int iCount)
{
	if (strItemName == m_strPrevItemName)
	{
		string strName;
		strName.assign(strItemName.begin(), strItemName.end());

		iCount += m_iPrevCount;
		m_iPrevCount = iCount;

		m_pFont->SetString(strName.c_str() + string("  +") + std::to_string(iCount));
		m_pIcon->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());
		m_fActiveTime = MAX_ACTIVE_TIME;
	}
	else
	{
		string strName;
		strName.assign(strItemName.begin(), strItemName.end());

		if (iCount < 0)
		{
			m_pFont->SetString(strName.c_str() + string("  ") + std::to_string(iCount));
			m_pIcon->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

			Vec4 vColor = Vec4(0.8f, 0.4f, 0.4f, 0.8f);
			pBackground->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &vColor);
		}
		else
		{
			m_pFont->SetString(strName.c_str() + string("  +") + std::to_string(iCount));
			m_pIcon->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

			Vec4 vColor = Vec4(0.4f, 0.8f, 0.4f, 0.8f);
			pBackground->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &vColor);
		}

		m_fActiveTime = MAX_ACTIVE_TIME;
		TransferLayer(30, true);
		m_bEnable = true;

		m_strPrevItemName = strItemName;
		m_iPrevCount = iCount;
	}
}

void CItemLootScript::SetBackgroundObject(CGameObject * pObject)
{
	pBackground = pObject;
}
