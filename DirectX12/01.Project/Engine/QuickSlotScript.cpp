#include "stdafx.h"
#include "QuickSlotScript.h"


CQuickSlotScript::CQuickSlotScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_iSelect(-1)
{
}


CQuickSlotScript::~CQuickSlotScript()
{
}

void CQuickSlotScript::Update()
{
	const vector<CGameObject *>& vecObj = GetObj()->GetChild();

	if (KEY_TAB(KEY_TYPE::KEY_NUM1))
	{
		int a = 2;
		if (m_iSelect == 0)
		{
			m_iSelect = -1;
			a = 2;
		}
		else
		{
			if (m_iSelect != -1)
				vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
			m_iSelect = 0;
			a = 3;
		}
		vecObj[0]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	}
	else if (KEY_TAB(KEY_TYPE::KEY_NUM2))
	{
		int a = 2;
		if (m_iSelect == 1)
		{
			m_iSelect = -1;
			a = 2;
		}
		else
		{
			if (m_iSelect != -1)
				vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
			m_iSelect = 1;
			a = 3;
		}
		vecObj[1]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	}
	else if (KEY_TAB(KEY_TYPE::KEY_NUM3))
	{
		int a = 2;
		if (m_iSelect == 2)
		{
			m_iSelect = -1;
			a = 2;
		}
		else
		{
			if (m_iSelect != -1)
				vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
			m_iSelect = 2;
			a = 3;
		}
		vecObj[2]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	}
	else if (KEY_TAB(KEY_TYPE::KEY_NUM4))
	{
		int a = 2;
		if (m_iSelect == 3)
		{
			m_iSelect = -1;
			a = 2;
		}
		else
		{
			if (m_iSelect != -1)
				vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
			m_iSelect = 3;
			a = 3;
		}
		vecObj[3]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	}
	else if (KEY_TAB(KEY_TYPE::KEY_NUM5))
	{
		int a = 2;
		if (m_iSelect == 4)
		{
			m_iSelect = -1;
			a = 2;
		}
		else
		{
			if (m_iSelect != -1)
				vecObj[m_iSelect]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
			m_iSelect = 4;
			a = 3;
		}
		vecObj[4]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_0, &a);
	}
}
