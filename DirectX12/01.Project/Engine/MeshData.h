#pragma once
#include "Resource.h"

#include "FBXLoader.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CGameObject;

class CMeshData :
	public CResource
{
public:
	CMeshData();
	~CMeshData();

private:
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;

public:
	static CMeshData* LoadFromFBX( const wstring& strPath );

	virtual void Load( const wstring& strPath );
	virtual void Save( const wstring& strPath );

	CGameObject* Instantiate();
};

