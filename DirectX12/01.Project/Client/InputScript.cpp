#include "stdafx.h"
#include "InputScript.h"

#include <Engine/KeyMgr.h>
#include <Engine/FontMgr.h>
#include <Engine/Material.h>
#include <Engine/Font.h>

#include <iostream>

CInputScript::CInputScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bEnable(false),
	m_vFontColor(Vec4(1.f, 1.f, 1.f, 1.f)),
	m_vHighlightColor(Vec4(0.5f, 0.5f, 0.7f, 0.5f)),
	m_vBackColor(Vec4(0.2f, 0.2f, 0.2f, 1.f)),
	m_iCount(0),
	m_iMaxCount(20)
{
	m_strInput.reserve(m_iMaxCount);
}


CInputScript::~CInputScript()
{
}

void CInputScript::Init()
{
}

void CInputScript::Clear()
{
	m_strInput.clear();
	m_strEmpty.clear();
	m_iCount = 0;
	Font()->SetString(" ");
}

void CInputScript::Update()
{
	// 활성화될 경우
	if (m_bEnable)
	{
		// 글자
		if (KEY_TAB(KEY_TYPE::KEY_Q))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('Q');
			else
				WriteFont('q');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_W))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('W');
			else
				WriteFont('w');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_E))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('E');
			else
				WriteFont('e');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_R))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('R');
			else
				WriteFont('r');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_T))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('T');
			else
				WriteFont('t');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_Y))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('Y');
			else
				WriteFont('y');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_U))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('U');
			else
				WriteFont('u');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_I))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('I');
			else
				WriteFont('i');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_O))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('O');
			else
				WriteFont('o');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_P))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('P');
			else
				WriteFont('p');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_A))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('A');
			else
				WriteFont('a');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_S))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('S');
			else
				WriteFont('s');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_D))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('D');
			else
				WriteFont('d');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_F))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('F');
			else
				WriteFont('f');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_G))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('G');
			else
				WriteFont('g');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_H))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('H');
			else
				WriteFont('h');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_J))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('J');
			else
				WriteFont('j');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_K))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('K');
			else
				WriteFont('k');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_L))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('L');
			else
				WriteFont('l');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_Z))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('Z');
			else
				WriteFont('z');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_X))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('X');
			else
				WriteFont('x');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_C))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('C');
			else
				WriteFont('c');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_V))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('V');
			else
				WriteFont('v');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_B))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('B');
			else
				WriteFont('b');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_N))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('N');
			else
				WriteFont('n');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_M))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('M');
			else
				WriteFont('m');
		}

		// 숫자
		else if (KEY_TAB(KEY_TYPE::KEY_1))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('!');
			else
				WriteFont('1');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_2))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('@');
			else
				WriteFont('2');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_3))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('#');
			else
				WriteFont('3');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_4))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('$');
			else
				WriteFont('4');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_5))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('%');
			else
				WriteFont('5');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_6))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('^');
			else
				WriteFont('6');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_7))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('&');
			else
				WriteFont('7');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_8))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('*');
			else
				WriteFont('8');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_9))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont('(');
			else
				WriteFont('9');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_0))
		{
			if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT))
				WriteFont(')');
			else
				WriteFont('0');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_DOT))
		{
			WriteFont('.');
		}
		else if (KEY_TAB(KEY_TYPE::KEY_SPACE))
		{
			WriteFont(' ');
		}

		// 백스페이스
		else if (KEY_TAB(KEY_TYPE::KEY_BACKSPACE))
		{
			if (m_iCount > 0)
			{
				m_strInput.erase(m_strInput.length()-1, 1);
				m_iCount--;

				m_strEmpty.clear();
				m_strEmpty.resize(m_iMaxCount - m_strInput.size());

				Font()->SetString(m_strInput + m_strEmpty);
			}
		}
	}
}

void CInputScript::WriteFont(char font)
{
	if (m_iCount < m_iMaxCount)
	{
		m_strInput.push_back(font);
		m_iCount++;

		m_strEmpty.clear();
		m_strEmpty.resize(m_iMaxCount - m_strInput.size());

		Font()->SetString(m_strInput + m_strEmpty);
	}
}

void CInputScript::SetRect(Vec4 rc)
{
	m_vBox = rc;
}

Vec4 CInputScript::GetRect()
{
	return m_vBox;
}

void CInputScript::SetFontColor(Vec4 vColor)
{
	m_vFontColor = vColor;
}

void CInputScript::SetHighlightColor(Vec4 vColor)
{
	m_vHighlightColor = vColor;
}

void CInputScript::SetBackColor(Vec4 vColor)
{
	m_vBackColor = vColor;
}


void CInputScript::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
	if (m_bEnable)
	{
		Font()->SetBackColor(m_vHighlightColor);
	}
	else
	{
		Font()->SetBackColor(m_vBackColor);
	}
}

void CInputScript::SetMaxCount(int iCount)
{
	m_iMaxCount = iCount;
}

bool CInputScript::GetEnable()
{
	return m_bEnable;
}

string CInputScript::GetString()
{
	return m_strInput;
}
