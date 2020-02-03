#include "stdafx.h"
#include "MeshRender.h"

#include "Transform.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Mesh.h"
#include "PathManager.h"

CMeshRender::CMeshRender() :
	 CComponent(COMPONENT_TYPE::MESHRENDER)
{
}


CMeshRender::~CMeshRender()
{
}

Ptr<CMesh> CMeshRender::GetMesh()
{
	return m_pMesh;
}

void CMeshRender::SetMesh( Ptr<CMesh> pMesh )
{
	m_pMesh = pMesh;
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial()
{
	if ( NULL == m_pMtrl )
		return NULL;

	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial()
{
	return m_pMtrl;
}

void CMeshRender::SetMaterial( Ptr<CMaterial> pMtrl )
{
	m_pMtrl = pMtrl;
}

void CMeshRender::Render()
{
	if ( !IsActive() || NULL == m_pMesh || NULL == m_pMtrl || NULL == m_pMtrl->GetShader() )
		return;

	Transform()->UpdateDate();

	m_pMtrl->UpdateData();
	m_pMesh->Render();
}

void CMeshRender::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	// 존재여부 저장(nullptr 인지 아닌지)
	fwrite( &m_pMesh, sizeof( void* ), 1, pFile );

	if ( nullptr != m_pMesh )
	{
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_pMesh->GetName() );
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_pMesh->GetPath() );
	}

	// 존재여부 저장(nullptr 인지 아닌지)
	fwrite( &m_pMtrl, sizeof( void* ), 1, pFile );

	if ( nullptr != m_pMtrl )
	{
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_pMtrl->GetName() );
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_pMtrl->GetPath() );
	}
}

void CMeshRender::LoadFromScene( FILE * pFile )
{
	void* pData = nullptr;
	fread( &pData, sizeof( void* ), 1, pFile );

	if ( pData )
	{
		wstring strMesh = GET_SINGLE(CPathManager)->LoadWString( pFile );
		wstring strPath = GET_SINGLE(CPathManager)->LoadWString( pFile );
		m_pMesh = GET_SINGLE(CResourceManager)->FindRes<CMesh>( strMesh );

		if ( nullptr == m_pMesh )
		{
			m_pMesh = GET_SINGLE( CResourceManager )->Load<CMesh>( strMesh, strPath );
		}
	}

	fread( &pData, sizeof( void* ), 1, pFile );

	if ( pData )
	{
		wstring strMtrl = GET_SINGLE(CPathManager)->LoadWString( pFile );
		wstring strPath = GET_SINGLE(CPathManager)->LoadWString( pFile );
		m_pMtrl = GET_SINGLE( CResourceManager )->FindRes<CMaterial>( strMtrl );

		if ( nullptr == m_pMtrl )
		{
			m_pMtrl = GET_SINGLE( CResourceManager )->Load<CMaterial>( strMtrl, strPath );
		}
	}
}
