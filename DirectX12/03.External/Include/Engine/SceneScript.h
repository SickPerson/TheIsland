#pragma once
#include "Entity.h"

class CSceneScript :
	public CEntity
{
protected:
	friend class CScene;

public:
	CSceneScript();
	virtual ~CSceneScript();

protected:
	class CScene* m_pScene;

public:
	void SetScene( class CScene* pScene );

public:
	virtual void Init() = 0;
	virtual void Input();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();
};

