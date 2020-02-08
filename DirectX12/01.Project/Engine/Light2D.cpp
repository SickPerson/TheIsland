#include "stdafx.h"
#include "Light2D.h"

#include "RenderManager.h"
#include "Transform.h"

CLight2D::CLight2D() : 
	CComponent(COMPONENT_TYPE::LIGHT2D),
	m_tLightInfo{ }
{
	m_tLightInfo.iLightType = ( UINT )LIGHT_TYPE::END;
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalUpdate()
{
	if ( !IsActive() )
		return;

	m_tLightInfo.vLightPos = Transform()->GetWorldPos();

	// RenderManager¿¡ µî·Ï
	GET_SINGLE( CRenderManager )->RegisterLight2D( m_tLightInfo );
}

void CLight2D::UpdateData()
{
}

void CLight2D::SetLightColor( const Vec3 & vColor )
{
	m_tLightInfo.vLightColor = vColor;

	Changed();
}

void CLight2D::SetLightRange( float fRange )
{
	m_tLightInfo.fRange = fRange;
	Changed();
}

void CLight2D::SetLightType( LIGHT_TYPE eType )
{
	m_tLightInfo.iLightType = ( int )eType;
	Changed();
}

Vec3 CLight2D::GetLightColor()
{
	return m_tLightInfo.vLightColor;
}

float CLight2D::GetLightRange()
{
	return m_tLightInfo.fRange;
}

LIGHT_TYPE CLight2D::GetLightType()
{
	return (LIGHT_TYPE)m_tLightInfo.iLightType;
}

void CLight2D::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	fwrite( &m_tLightInfo, sizeof( tLight2D ), 1, pFile );
}

void CLight2D::LoadFromScene( FILE * pFile )
{
	fread( &m_tLightInfo, sizeof( tLight2D ), 1, pFile );
}
