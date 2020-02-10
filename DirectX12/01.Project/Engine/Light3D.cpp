#include "stdafx.h"
#include "Light3D.h"
#include "Transform.h"
#include "RenderManager.h"

CLight3D::CLight3D() :
	CComponent( COMPONENT_TYPE::LIGHT3D)
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::FinalUpdate()
{
	m_tLightInfo.vLightPos = Transform()->GetWorldPos();
	GET_SINGLE( CRenderManager )->RegisterLight3D( this );
}

void CLight3D::SetLightRange( float fRange )
{
	m_tLightInfo.fRange = fRange;
}

void CLight3D::SetLightType( LIGHT_TYPE eType )
{
	m_tLightInfo.iLightType = ( int )eType;
}

void CLight3D::SetLightPos( const Vec3 & vPos )
{
	m_tLightInfo.vLightPos = vPos;
	Transform()->SetLocalPos( vPos );
}

void CLight3D::SetLightDir( const Vec3 & vDir )
{
	m_tLightInfo.vLightDir = vDir;
	// ������ ����ȭ ��Ų��.
	m_tLightInfo.vLightDir.Normalize();
}

void CLight3D::SetDiffuseColor( const Vec3 & vColor )
{
	m_tLightInfo.tColor.vDiff = vColor;
}

void CLight3D::SetAmbient( const Vec3 & vAmb )
{
	m_tLightInfo.tColor.vAmb = vAmb;
}

void CLight3D::SetSpecular( const Vec3 & vSpec )
{
	m_tLightInfo.tColor.vSpec = vSpec;
}

const tLight3D & CLight3D::GetLight3DInfo()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_tLightInfo;
}

void CLight3D::SaveToScene( FILE * pFile )
{
}

void CLight3D::LoadFromScene( FILE * pFile )
{
}
