#include "stdafx.h"
#include "QuickSlotScript.h"


CQuickSlotScript::CQuickSlotScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_iSelect(-1),
	m_bEnable(true)
{
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

	const vector<CGameObject *>& vecObj = GetObj()->GetChild();
	int a = 2;
	if (m_iSelect == key - 1)
	{
		m_iSelect = -1;
		a = 2;
	}
	else
	{
		if (m_iSelect != -1)
			vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
		m_iSelect = key - 1;
		a = 3;
	}
	vecObj[key - 1]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
}
