#include "stdafx.h"
#include "ChatScript.h"
#include <Engine/FontMgr.h>
#include <iostream>
CChatScript::CChatScript() : 
	CScript((UINT )SCRIPT_TYPE::UISCRIPT)
	, m_vNameColor(Vec4(1.f, 0.f, 0.f, 1.f))
	, m_vChatColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vBackColor(Vec4(0.f, 0.f, 0.f, 0.f))
{
	for (int i = 0; i < MAX_CHAT_LINE; ++i)
	{
		m_vecChatObj[i].reserve(30);
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
	m_vecChatObj[line].emplace_back(pObject);
}

void CChatScript::AddChat(string name, string chat)
{
	/*for (int i = MAX_CHAT_LINE - 2; i >= 0; ++i)
	{
		m_strChat[i + 1] = m_strChat[i];
		m_strChatName[i + 1] = m_strChatName[i];

		for (int word = 0; word < 30; ++word)
		{
			m_vecChatObj[i + 1][word]->MeshRender()->SetMaterial(m_vecChatObj[i][word]->MeshRender()->GetSharedMaterial());
		}
	}*/
	std::cout << name << std::endl;
	for (int i = 0; i < name.size(); ++i)
	{
		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[name[i]];
		Ptr<CMaterial> pMtrl = m_vecChatObj[0][i]->MeshRender()->GetSharedMaterial();
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
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vNameColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vBackColor);
	}

	for (int i = 10; i < chat.size() + 10; ++i)
	{
		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[chat[i - 10]];
		Ptr<CMaterial> pMtrl = m_vecChatObj[0][i]->MeshRender()->GetSharedMaterial();
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
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vChatColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vBackColor);
	}
}
