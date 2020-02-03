#include "stdafx.h"
#include "MeshRender.h"


CMeshRender::CMeshRender()
{
}


CMeshRender::~CMeshRender()
{
}

Ptr<CMesh> CMeshRender::GetMesh()
{
	return Ptr<CMesh>();
}

void CMeshRender::SetMesh( Ptr<CMesh> pMesh )
{
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial()
{
	return Ptr<CMaterial>();
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial()
{
	return Ptr<CMaterial>();
}

void CMeshRender::SetMaterial( Ptr<CMaterial> pMtrl )
{
}

void CMeshRender::Render()
{
}

void CMeshRender::SaveToScene( FILE * pFile )
{
}

void CMeshRender::LoadFromScene( FILE * pFile )
{
}
