#include "stdafx.h"
#include "Camera.h"
#include "Frustum.h"

CCamera::CCamera() : 
	CComponent(COMPONENT_TYPE::CAMERA),
	m_Frustum(this),
	m_fFar(1000.f),
	m_fNear(1.f),
	m_fFOV(XM_PI/ 4.f),
	m_fScale(1.f),
	m_eProType(PROJ_TYPE::ORTHGRAPHIC),
	m_iLayerCheck(0)
{
}


CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
}

void CCamera::Render()
{
}

void CCamera::SetProjType( PROJ_TYPE eType )
{
}

void CCamera::SetFar( float fFar )
{
}

void CCamera::SetScale( float fScale )
{
}

void CCamera::SetFOV( float fFOV )
{
}

void CCamera::SetLayerCheck( int iLayerIdx, bool bAdd )
{
}

void CCamera::SetLayerAllCheck()
{
}

void CCamera::SetLayerAllRelease()
{
}

PROJ_TYPE CCamera::GetProjType()
{
	return PROJ_TYPE();
}

float CCamera::GetFar()
{
	return 0.0f;
}

float CCamera::GetScale()
{
	return 0.0f;
}

float CCamera::GetFOV()
{
	return 0.0f;
}

const Matrix & CCamera::GetViewMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const Matrix & CCamera::GetViewMatInv()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const Matrix & CCamera::GetProjMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const Matrix & CCamera::GetProjMatInv()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}

void CCamera::SaveToScene( FILE * pFile )
{
}

void CCamera::LoadFromScene( FILE * pFile )
{
}
