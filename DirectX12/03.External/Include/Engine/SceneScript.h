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
	virtual void Init() = 0;

	void Update();
	void LateUpdate();
	void FinalUpdate();

};

