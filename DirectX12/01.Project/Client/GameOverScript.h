#pragma once
#include <Engine/Script.h>

#define GAMEOVER_INTERPOLATION_TIME 5.f

class CGameOverScript :
	public CScript
{
	float	m_fTime;

	CGameObject* m_pBackground;
	CGameObject* m_pTitle;
	CGameObject* m_pMessage;
	CGameObject* m_pButton;
	CGameObject* m_pTitleImage;

public:
	CGameOverScript();
	virtual ~CGameOverScript();

	CLONE(CGameOverScript);

	// CScript을(를) 통해 상속됨
	virtual void Update() override;

public:
	void Init();
};

