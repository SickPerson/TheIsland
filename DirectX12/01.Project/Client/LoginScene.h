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

public:
	void CreateLoginWorld();

	void CreateInputID();
	void CreateInputIP();
	void CreateInputPassword();
	void NextScene();

private:
	CGameObject*		m_pID;
	CGameObject*		m_pPassword;
	CGameObject*		m_pIP;

	string				m_strID;
	string				m_strPassword;
	string				m_strIP;

	Vec4				m_vFontColor;
	Vec4				m_vFontBackColor;

	bool				m_bIP_Success = false;
	bool				m_bLoginSuccess = false;

public:
	void SetNextScene(bool bLoginSuccess) { m_bLoginSuccess = bLoginSuccess; };
public:
	CGameObject* GetIDObj();
	CGameObject* GetPasswordObj();
};

