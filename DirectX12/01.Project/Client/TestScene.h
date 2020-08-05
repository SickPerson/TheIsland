#pragma once
#include <Engine/SceneScript.h>

class CTestScene :
	public CSceneScript
{
public:
	CTestScene();
	virtual ~CTestScene();

	// CSceneScript을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;

};

