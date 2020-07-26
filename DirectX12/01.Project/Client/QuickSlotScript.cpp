#include "stdafx.h"
#include "QuickSlotScript.h"
#include "ItemScript.h"

#include <iostream>

CQuickSlotScript::CQuickSlotScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_iSelect(-1),
	m_bEnable(true),
	m_vSlotColor(Vec4(0.7f, 0.7f, 0.7f, 1.f)),
	m_vHighlightSlotColor(Vec4(0.4f, 0.4f, 0.8f, 1.f))
{
	m_vQuickSlot.reserve(5);
}


CQuickSlotScript::~CQuickSlotScript()
{
}

void CQuickSlotScript::Update()
{

}

void CQuickSlotScript::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

bool CQuickSlotScript::GetEnable()
{
	return m_bEnable;
}

void CQuickSlotScript::KeyInput(int key)
{
	if (!m_bEnable)
	{
		return;
	}

	if (m_iSelect == key - 1)
	{
		m_vQuickSlot[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &m_vSlotColor);
		m_iSelect = -1;
		return;
	}
	else
	{
		if (m_iSelect != -1)
			m_vQuickSlot[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &m_vSlotColor);
		m_iSelect = key - 1;
	}
	m_vQuickSlot[key - 1]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &m_vHighlightSlotColor);
}

int CQuickSlotScript::GetSelect()
{
	return m_iSelect;
}

void CQuickSlotScript::AddQuickSlot(CGameObject * pObj)
{
	m_vQuickSlot.emplace_back(pObj);
}

void CQuickSlotScript::SetHighlightSlotColor(Vec4 vColor)
{
	m_vHighlightSlotColor = vColor;
}

void CQuickSlotScript::SetSlotColor(Vec4 vColor)
{
	m_vSlotColor = vColor;
}
