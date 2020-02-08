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
