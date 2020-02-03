#include "stdafx.h"
#include "Entity.h"


CEntity::CEntity()
{
}


CEntity::~CEntity()
{
}

void CEntity::SetName( wstring strName )
{
	m_strName = strName;
}

const wstring & CEntity::GetName()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_strName;
}
