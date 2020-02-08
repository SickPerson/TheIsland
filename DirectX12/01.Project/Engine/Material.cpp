#include "stdafx.h"
#include "Material.h"
#include "Device.h"
#include "ConstantBuffer.h"
#include "PathManager.h"
#include "ResourceManager.h"

CMaterial::CMaterial() :
	CResource( RES_TYPE::MATERIAL ),
	m_bFileSave( true )
{
	m_tParam.m_vDiff = Vec4( 0.5f, 0.5f, 0.5f, 0.5f );
	m_tParam.m_vSpec = Vec4::One;
	m_tParam.m_vEmv = Vec4::Zero;
}	


CMaterial::~CMaterial()
{
}

void CMaterial::SetShader( Ptr<CShader> pShader )
{
	m_pShader = pShader;
}

void CMaterial::SetData( SHADER_PARAM eParam, void * pData )
{
	switch ( eParam )
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_tParam.m_arrInt[( UINT )eParam - ( UINT )SHADER_PARAM::INT_0] = *( ( int* )pData );
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_tParam.m_arrFloat[( UINT )eParam - ( UINT )SHADER_PARAM::FLOAT_0] = *( ( float* )pData );
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_tParam.m_arrVec2[( UINT )eParam - ( UINT )SHADER_PARAM::VEC2_0] = *( ( Vec2* )pData );
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_tParam.m_arrVec4[( UINT )eParam - ( UINT )SHADER_PARAM::VEC4_0] = *( ( Vec4* )pData );
		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:
		m_tParam.m_arrMat[( UINT )eParam - ( UINT )SHADER_PARAM::MATRIX_0] = *( ( Matrix* )pData );
		break;

	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
		m_arrTex[( UINT )eParam - ( UINT )SHADER_PARAM::TEX_0] = ( CTexture* )pData;
		break;
	default:
		break;
	}
}

void CMaterial::DisableFileSave()
{
	m_bFileSave = false;
}

void CMaterial::UpdateData()
{
	// Texture Register Update
	UINT iOffsetPos = ( UINT )TEXTURE_REGISTER::t0;

	for ( UINT i = 0; i < ( UINT )SHADER_PARAM::TEX_END - ( UINT )SHADER_PARAM::TEX_0; ++i )
	{
		if ( NULL != m_arrTex[i] )
		{
			GET_SINGLE( CDevice )->SetTextrueToRegister( m_arrTex[i].GetPointer(), TEXTURE_REGISTER( i + iOffsetPos ) );
			m_tParam.m_iArrTex[i] = 1;
		}

		else
		{
			m_tParam.m_iArrTex[i] = 0;
		}
	}

	static CConstantBuffer* pCB = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b1 );
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pCB, pCB->AddData( &m_tParam ) );

	m_pShader->UpdateData();
}

void CMaterial::Load( const wstring & strFullPath )
{
	FILE* pFile = nullptr;
	_wfopen_s( &pFile, strFullPath.c_str(), L"rb" );

	// 쉐이더 키값
	wstring strName;
	strName = GET_SINGLE(CPathManager)->LoadWString( pFile );
	m_pShader = GET_SINGLE( CResourceManager )->FindRes<CShader>( strName );

	// 쉐이더 파라미터
	fread( &m_tParam, sizeof( tMtrlParam ), 1, pFile );

	UINT iMaxCount = ( UINT )SHADER_PARAM::TEX_END - ( UINT )SHADER_PARAM::TEX_0;
	for ( UINT i = 0; i < iMaxCount; ++i )
	{
		UINT iExist = 0;
		fread( &iExist, 4, 1, pFile );

		if ( iExist )
		{
			wstring strKey = GET_SINGLE( CPathManager )->LoadWString( pFile );
			wstring strPath = GET_SINGLE( CPathManager )->LoadWString( pFile );

			m_arrTex[i] = GET_SINGLE( CResourceManager )->FindRes<CTexture>( strKey );
			if ( nullptr == m_arrTex[i] )
			{
				m_arrTex[i] = GET_SINGLE( CResourceManager )->Load<CTexture>( strKey, strPath );
			}
		}
	}


	fclose( pFile );
}

void CMaterial::Save( const wstring & strFullPath )
{
	if ( !m_bFileSave )
		return;

	wstring strFilePath = strFullPath;
	strFilePath += GetName();

	FILE* pFile = nullptr;
	_wfopen_s( &pFile, strFilePath.c_str(), L"wb" );

	// 쉐이더 키값
	GET_SINGLE( CPathManager )->SaveWString( pFile, m_pShader->GetName() );

	// 쉐이더 파라미터
	fwrite( &m_tParam, sizeof( tMtrlParam ), 1, pFile );

	UINT iMaxCount = ( UINT )SHADER_PARAM::TEX_END - ( UINT )SHADER_PARAM::TEX_0;
	for ( UINT i = 0; i < iMaxCount; ++i )
	{
		UINT iExist = 1;
		if ( nullptr == m_arrTex[i] )
		{
			iExist = 0;
			fwrite( &iExist, 4, 1, pFile );
			continue;
		}

		fwrite( &iExist, 4, 1, pFile );
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_arrTex[i]->GetName() );
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_arrTex[i]->GetPath() );
	}

	fclose( pFile );
}

Ptr<CShader> CMaterial::GetShader()
{
	return m_pShader;
}

CMaterial * CMaterial::Clone()
{
	CResource::Clone();

	CMaterial* pClone = new CMaterial( *this );

	wchar_t szAdd[50] = {};
	wsprintf( szAdd, L"_Clone_%d.mtrl", GetCloneCount() );

	// Naming
	wstring strPath = L"Material\\";
	strPath += GET_SINGLE( CPathManager )->GetFileName( GetName().c_str() );
	//strPath += szAdd;

	pClone->SetName( strPath );
	pClone->SetPath( strPath );

	GET_SINGLE( CResourceManager )->AddCloneResource<CMaterial>( pClone );

	//if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod())
	//{	
	//	pClone->m_bFileSave = true;
	//	pClone->Save(CPathMgr::GetResPath());
	//}

	return pClone;
}
