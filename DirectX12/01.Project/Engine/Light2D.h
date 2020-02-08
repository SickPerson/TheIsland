#pragma once
#include "Component.h"

class CLight2D :
	public CComponent
{
public:
	CLight2D();
	virtual ~CLight2D();

private:
	tLight2D		m_tLightInfo;

public:
	virtual void FinalUpdate();
	void UpdateData();

public:
	void SetLightColor( const Vec3& vColor );
	void SetLightRange( float fRange );
	void SetLightType( LIGHT_TYPE eType );

public:
	Vec3 GetLightColor();
	float GetLightRange();
	LIGHT_TYPE GetLightType();

public:
	CLONE( CLight2D );

	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );
};

