#include "stdafx.h"
#include "ChatScript.h"
#include <Engine/FontMgr.h>
#include <Engine/Font.h>

#include "CheatMgr.h"

#include <iostream>

CChatScript::CChatScript() : 
	CScript((UINT )SCRIPT_TYPE::UISCRIPT)
	, m_vNameColor(Vec4(1.f, 0.f, 0.f, 1.f))
	, m_vChatColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vBackColor(Vec4(0.5f, 0.5f, 0.5f, 0.3f))
	, m_bCheat(true)
{
	for (int i = 0; i < MAX_CHAT_LINE; ++i)
	{
		m_pChatObj[i] = NULL;
	}
}


CChatScript::~CChatScript()
{
}

void CChatScript::Update()
{

}

void CChatScript::AddChatObject(CGameObject * pObject, int line)
{
	m_pChatObj[line] = pObject;
	m_pChatObj[line]->Font()->SetChangeColorIndex(10);
	m_pChatObj[line]->Font()->SetFontColor(m_vNameColor, 0);
	m_pChatObj[line]->Font()->SetFontColor(m_vChatColor, 1);
}

void CChatScript::AddChat(string name, string chat)
{
	if (chat.size() == 0)
		return;

	if (m_bCheat)
	{
		if (CCheatMgr::GetInst()->CheckCheat(chat))
		{
			AddChat("System", "Enable Cheat!");
		}
	}

	for (int i = MAX_CHAT_LINE - 2; i >= 0; --i)
	{
		m_pChatObj[i + 1]->Font()->SetString(m_pChatObj[i]->Font()->GetString());
	}

	string str;
	str.resize(30);
	for (int i = 0; i < name.size(); ++i)
	{
		str[i] = name[i];
	}
	for (int i = 0; i < chat.size(); ++i)
	{
		str[i + 10] = chat[i];
	}
	m_pChatObj[0]->Font()->SetString(str);

}
