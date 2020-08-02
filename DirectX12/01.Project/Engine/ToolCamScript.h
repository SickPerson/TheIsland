#pragma once
#include "Script.h"

class CToolCamScript :
	public CScript
{
private:
	float		m_fSpeed;
	float		m_fScaleSpeed;

	bool		m_bTool;
	float		m_fMaxY;

public:
	virtual void Update();

public:
	CLONE( CToolCamScript );

public:
	CToolCamScript();
	virtual ~CToolCamScript();

public:
	void SetTool( bool bTool );
	void SetMaxY( float fMaxY );
};

