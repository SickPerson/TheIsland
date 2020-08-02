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
	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * _pFile ) override;
	virtual void LoadFromScene( FILE * _pFile ) override;
};

