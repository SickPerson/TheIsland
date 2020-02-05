#pragma once
#include "Engine.h"

class CResource;
class CTexture;
class CSound;
class CMesh;

class CResourceManager
{
private:
	map<wstring, CResource*>	m_mapResource[( UINT )RES_TYPE::END];
	vector<CResource*>			m_vecCloneResource[( UINT )RES_TYPE::END];

private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void InitSound();

public:
	void Init();

	template<typename T>
	void AddResource( const wstring& strKey, Ptr<T> pRes );

	template<typename T>
	void AddCloneResource( Ptr<T> pCloneRes );

	const map<wstring, CResource*>& GetResource( RES_TYPE eType );

	template<typename T>
	Ptr<T> FindRes( const wstring& strKey );

	template<typename T>
	Ptr<T> Load( const wstring& strKey, const wstring& strPath );

	DECLARE_SINGLE( CResourceManager )
};

