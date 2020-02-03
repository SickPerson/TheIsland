#include "stdafx.h"
#include "Component.h"


CComponent::CComponent( COMPONENT_TYPE eType )
{
}

CComponent::~CComponent()
{
}


void ComponentSetChange( CComponent *, bool )
{
}


void CComponent::SetGameObject( CGameObject * pObject )
{
}

void CComponent::SetActive( bool bActive )
{
}

bool CComponent::IsActive()
{
	return false;
}

bool CComponent::IsChanged()
{
	return false;
}

void CComponent::Changed()
{
}

COMPONENT_TYPE CComponent::GetComponentType()
{
	return COMPONENT_TYPE();
}

CGameObject * CComponent::GetGameObject()
{
	return nullptr;
}

CTransform * CComponent::Transform()
{
	return nullptr;
}

CMeshRender * CComponent::MeshRender()
{
	return nullptr;
}

CCollider2D * CComponent::Collider2D()
{
	return nullptr;
}

CCollider3D * CComponent::Collider3D()
{
	return nullptr;
}

CAnimation2D * CComponent::Animation2D()
{
	return nullptr;
}

CCamera * CComponent::Camera()
{
	return nullptr;
}

CLight2D * CComponent::Light2D()
{
	return nullptr;
}

CLight3D * CComponent::Light3D()
{
	return nullptr;
}