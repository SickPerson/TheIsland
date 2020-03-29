#pragma once
#include <Engine/SceneScript.h>

class CGameObject;
class CChatScript;

class CLoginScene :
	public CSceneScript
{
public:
	CLoginScene();
	virtual ~CLoginScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;

private:
	CGameObject*		m_pID;
	CGameObject*		m_pPassword;
	CGameObject*		m_pIP;

	string				m_strID;
	string				m_strPassword;
	string				m_strIP;

public:
	CGameObject* GetIDObj();
	CGameObject* GetPasswordObj();

	CChatScript* GetID();
	CChatScript* GetPassword();
};

