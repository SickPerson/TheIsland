#pragma once
#include "Component.h"

class CLight3D :
	public CComponent
{
public:
	CLight3D();
	virtual ~CLight3D();

private:
	tLight3D		m_tLightInfo;

public:
	virtual void FinalUpdate();

public:
	void SetLightRange( float fRange );
	void SetLightType( LIGHT_TYPE eType );
	void SetLightPos( const Vec3& vPos );
	void SetLightDir( const Vec3& vDir );
	void SetDiffuseColor( const Vec3& vColor );
	void SetAmbient( const Vec3& vAmb );
	void SetSpecular( const Vec3& vSpec );

public:
	const tLight3D& GetLight3DInfo();

public:
	CLONE( CLight3D );

	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );
};

