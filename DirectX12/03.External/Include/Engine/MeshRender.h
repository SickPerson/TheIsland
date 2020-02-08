#pragma once
#include "Component.h"

class CMesh;
class CMaterial;

class CMeshRender : 
	public CComponent
{
public:
	CMeshRender();
	virtual ~CMeshRender();

	CLONE( CMeshRender );

private:
	Ptr<CMesh>			m_pMesh;
	Ptr<CMaterial>		m_pMtrl;

public:
	Ptr<CMesh>		GetMesh();
	void SetMesh( Ptr<CMesh> pMesh );

	Ptr<CMaterial> GetCloneMaterial();
	Ptr<CMaterial> GetSharedMaterial();

	void SetMaterial( Ptr<CMaterial> pMtrl );

public:
	void Render();
	
	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * pFile ) override;
	virtual void LoadFromScene( FILE * pFile ) override;
};

