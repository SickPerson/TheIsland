#pragma once

#include "Entity.h"

class CResource : 
	public CEntity
{
private:
	template<typename T>
	friend class Ptr;

public:
	CResource( RES_TYPE  eType );
	CResource( const CResource& res );
	virtual ~CResource();

private:
	wstring		m_strPath;	// 상대경로
	RES_TYPE	m_eType;

	int			m_iRefCount;
	UINT		m_iCloneCount;		// 리소스 복사 횟수

public:
	RES_TYPE GetType();
	UINT	GetCloneCount();

	void SetPath( const wstring& strPath );
	const wstring& GetPath();

public:
	void AddRef();
	void SubRef();

public:
	virtual void Load( const wstring& strFullPath );
	virtual void Save( const wstring& strFullPath );
	void SaveToScene( FILE* pFile );

public:
	virtual CResource* Clone();

	template<typename T>
	bool LoadFromScene( FILE* pFile );
};

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
