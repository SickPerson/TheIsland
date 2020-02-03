#include "stdafx.h"
#include "Resource.h"
#include "PathManager.h"

CResource::CResource( RES_TYPE  eType ) : 
	m_eType(eType),
	m_iRefCount(0),
	m_iCloneCount(0)
{
}

CResource::CResource( const CResource & res ) : 
	m_eType( res.m_eType ),
	m_iRefCount( 0 ),
	m_iCloneCount( 0 )
{
}

CResource::~CResource()
{
}

RES_TYPE CResource::GetType()
{
	return m_eType;
}

UINT CResource::GetCloneCount()
{
	return m_iCloneCount;
}

void CResource::SetPath( const wstring & strPath )
{
	m_strPath = strPath;
}

const wstring & CResource::GetPath()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_strPath;
}

void CResource::AddRef()
{
	m_iRefCount++;
}

void CResource::SubRef()
{
	m_iRefCount--;
}

void CResource::Load( const wstring & strFullPath )
{
}

void CResource::Save( const wstring & strFullPath )
{
}

void CResource::SaveToScene( FILE * pFile )
{
	// Key 저장
	GET_SINGLE( CPathManager )->SaveWString( pFile, GetName() );

	// 경로 저장
	GET_SINGLE( CPathManager )->SaveWString( pFile, m_strPath );
}

CResource * CResource::Clone()
{
	return nullptr;
}