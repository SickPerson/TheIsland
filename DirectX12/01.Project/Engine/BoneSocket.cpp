#include "stdafx.h"
#include "BoneSocket.h"


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

void CBoneSocket::Update()
{
}

void CBoneSocket::SaveToScene( FILE * _pFile )
{
}

void CBoneSocket::LoadFromScene( FILE * _pFile )
{
}
