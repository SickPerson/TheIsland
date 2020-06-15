#pragma once
#include "Component.h"

class CMouse :
	public CComponent
{
public:
	CMouse();
	~CMouse();
	CLONE( CMouse );

public:
	virtual void Update();

public:
	virtual void SaveToScene( FILE* _pFile );
	virtual void LoadFromScene( FILE* _pFile );
};

