#include "stdafx.h"
#include "InputScript.h"

#include <Engine/KeyMgr.h>
#include <Engine/FontMgr.h>
#include <Engine/Material.h>

#include <iostream>

CInputScript::CInputScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bEnable(true),
	m_iCount(0),
	m_vFontColor(Vec4(1.f, 1.f, 1.f, 1.f)),
	m_vFontBackColor(Vec4(0.f, 0.f, 0.f, 0.f))
{
	m_strInput.reserve(10);
	m_vFont.reserve(10);
}


CInputScript::~CInputScript()
{
}

void CInputScript::Init()
{
}

void CInputScript::Update()
{
	// 활성화될 경우
	if (m_bEnable)
	{
		if (KEY_TAB(KEY_TYPE::KEY_1))
		{
			WriteFont('1');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_2))
		{
			WriteFont('2');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_3))
		{
			WriteFont('3');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_4))
		{
			WriteFont('4');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_5))
		{
			WriteFont('5');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_6))
		{
			WriteFont('6');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_7))
		{
			WriteFont('7');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_8))
		{
			WriteFont('8');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_9))
		{
			WriteFont('9');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_0))
		{
			WriteFont('0');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_DOT))
		{
			WriteFont('.');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_BACKSPACE))
		{
			if (m_iCount > 0)
			{
				m_strInput.erase(m_strInput.length()-1, 1);
				Ptr<CMaterial> pMtrl = m_vFont[--m_iCount]->MeshRender()->GetSharedMaterial();
				float startU = 0.f;
				float startV = 0.f;
				float widthU = 0.f;
				float heightV = 0.f;
				pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
				pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
				pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
				pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
				pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vFontColor);
				pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vFontBackColor);
			}
		}
	}
	else
	{
		// 마우스 클릭을 통해 입력칸 활성화

		/*POINT vPos = CKeyMgr::GetInst()->GetMousePos();
		std::cout << vPos.x << std::endl;
		std::cout << vPos.y << std::endl;
		if (vPos.x >= m_rcBox.x && vPos.x <= m_rcBox.x + m_rcBox.w)
		{
			if (vPos.y >= m_rcBox.y && vPos.x <= m_rcBox.y + m_rcBox.h)
			{
				m_bEnable = true;
			}
		}*/
	}
}

void CInputScript::WriteFont(char font)
{
	if (m_iCount < m_vFont.size())
	{
		m_strInput.push_back(font);
		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[font];
		Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vFontBackColor);
	}
}

void CInputScript::AddInputObject(CGameObject * pObject)
{
	m_vFont.emplace_back(pObject);
}

void CInputScript::SetRect(Rect rc)
{
	m_rcBox = rc;
}

void CInputScript::SetFontColor(Vec4 vColor)
{
	m_vFontColor = vColor;
}

void CInputScript::SetFontBackColor(Vec4 vColor)
{
	m_vFontBackColor = vColor;
}

void CInputScript::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

string CInputScript::GetString()
{
	return m_strInput;
}

bool CInputScript::GetEnable()
{
	return m_bEnable;
}
