#include "stdafx.h"
#include "Resource.h"
#include "PathManager.h"
#include "ResourceManager.h"

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

template<typename T>
inline bool CResource::LoadFromScene( FILE * pFile )
{
	// Key
	wstring strName = GET_SINGLE( CPathManager )->LoadWString( pFile );

	// Path
	m_strPath = GET_SINGLE( CPathManager )->LoadWString( pFile );

	// 리소스가 이미 로딩되어있는지 체크
	Ptr<T> pRes = GET_SINGLE( CResourceManager )->FineRes<T>( strName );
	if ( NULL != pRes )
		return false;

	SetName( strName );

	wstring strPath = GET_SINGLE( CPathManager )->GetResPath();
	strPath += m_strPath;
	Load( strPath );
}
