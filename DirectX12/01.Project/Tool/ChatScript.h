#pragma once

#include <Engine/Script.h>

// 사용예 : 로그인창, 채팅 입력칸 등등

class CChatScript : 
	public CScript
{
public:
	CChatScript();
	virtual ~CChatScript();

	CLONE( CChatScript );

private:
	bool			m_bActive;
	wstring			m_strChat;
	
public:
	virtual void OnCollisionEnter( CCollider2D* _pOther );
	virtual void OnCollision( CCollider2D* _pOther );
	virtual void OnCollisionExit( CCollider2D* _pOther );

	virtual void Update();

public:
	void SetChatting( const wstring& strChat );
};

