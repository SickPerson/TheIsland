#include "stdafx.h"
#include "BoneSocket.h"

#include "GameObject.h"
#include "Animator3D.h"


CBoneSocket::CBoneSocket() :
	CComponent(COMPONENT_TYPE::BONESOCKET),
	m_pTarget(NULL)
{
}


CBoneSocket::~CBoneSocket()
{
}

void CBoneSocket::SetTarget( CGameObject * pTarget )
{
	m_pTarget = pTarget;
}

void CBoneSocket::SetTarget( CGameObject * pTarget, const wstring & strBoneName )
{
	m_pTarget = pTarget;
	m_strBoneName = strBoneName;
}

void CBoneSocket::Update()
{
	tMTBone tBone = m_pTarget->Animator3D()->FindBone( m_strBoneName );
	m_vOffset = tBone.matOffset.Translation();
}

void CBoneSocket::SaveToScene( FILE * _pFile )
{
}

void CBoneSocket::LoadFromScene( FILE * _pFile )
{
}
