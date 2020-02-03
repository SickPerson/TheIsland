#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
{
}


CGameObject::~CGameObject()
{
}

void CGameObject::Awake()
{
}

void CGameObject::Start()
{
}

void CGameObject::Update()
{
}

void CGameObject::LateUpdate()
{
}

void CGameObject::FinalUpdate()
{
}

void CGameObject::Enable()
{
}

void CGameObject::Disable()
{
}

void CGameObject::SetActive( bool bActive )
{
	m_bActive = bActive;
}

bool CGameObject::IsActive() const
{
	return m_bActive;
}

void CGameObject::FrustumCheck( bool bFrustumCheck )
{
	m_bFrustumCheck = bFrustumCheck;
}

bool CGameObject::GetFrustumCheck() const
{
	return m_bFrustumCheck;
}

int CGameObject::GetLayerIdx()
{
	return m_iLayerIdx;
}

void CGameObject::AddComponent( CComponent * pCom )
{
}

CComponent * CGameObject::GetComponent( COMPONENT_TYPE eType )
{
	return nullptr;
}

CTransform * CGameObject::Transform()
{
	return nullptr;
}

CMeshRender * CGameObject::MeshRender()
{
	return nullptr;
}

CCollider2D * CGameObject::Collider2D()
{
	return nullptr;
}

CCollider3D * CGameObject::Collider3D()
{
	return nullptr;
}

CAnimation2D * CGameObject::Animation2D()
{
	return nullptr;
}

CCamera * CGameObject::Camera()
{
	return nullptr;
}

CLight2D * CGameObject::Light2D()
{
	return nullptr;
}

CLight3D * CGameObject::Light3D()
{
	return nullptr;
}

const vector<CScript*>& CGameObject::GetScripts() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}