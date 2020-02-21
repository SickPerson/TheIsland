#include "stdafx.h"
#include "ChatScript.h"


CChatScript::CChatScript() : 
	CScript((UINT )SCRIPT_TYPE::UISCRIPT),
	m_bActive(false)
{
}


CChatScript::~CChatScript()
{
}

void CChatScript::OnCollisionEnter( CCollider2D * _pOther )
{
}

void CChatScript::OnCollision( CCollider2D * _pOther )
{

}

void CChatScript::OnCollisionExit( CCollider2D * _pOther )
{
}

void CChatScript::Update()
{
	TextOutW( NULL, Transform()->GetLocalPos().x, Transform()->GetLocalPos().y, m_strChat.c_str(), m_strChat.size() );
}

void CChatScript::SetChatting( const wstring & strChat )
{
	m_strChat = strChat;
}
