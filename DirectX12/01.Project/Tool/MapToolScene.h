#pragma once
#include <Engine/SceneScript.h>

class CMapToolScene :
	public CSceneScript
{
public:
	CMapToolScene();
	virtual ~CMapToolScene();

public:
	virtual void Init() override;
};

