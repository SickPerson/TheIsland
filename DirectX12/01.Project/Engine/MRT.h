#pragma once
#include "Entity.h"

#include "Ptr.h"
#include "Texture.h"

struct tRT
{
	Ptr<CTexture>	pTarget;
	float			clearColor[4];
};

class CMRT :
	public CEntity
{
public:
	CMRT();
	virtual ~CMRT();

public:
	virtual void Create( UINT _iCounr, tRT* arrRT );
};

