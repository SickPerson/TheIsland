#include "stdafx.h"
#include "Resource.h"


CResource::CResource( RES_TYPE  eType ) : 
	m_eType(eType)
{
}


CResource::~CResource()
{
}

RES_TYPE CResource::GetType()
{
	return m_eType;
}
