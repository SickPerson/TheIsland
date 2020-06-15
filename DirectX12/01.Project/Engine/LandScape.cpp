#include "stdafx.h"
#include "LandScape.h"
#include "PathMgr.h"
#include "Mesh.h"
#include "Ptr.h"
#include "ResMgr.h"

#include "Device.h"
#include "ConstantBuffer.h"

CLandScape::CLandScape() : 
	CComponent(COMPONENT_TYPE::LANDSCAPE),
	m_tLandScape{},
	m_iDetailLevel(37),
	m_iSplatCount(0)
{
}


CLandScape::~CLandScape()
{
}

void CLandScape::SaveToScene( FILE * _pFile )
{
}

void CLandScape::LoadFromScene( FILE * _pFile )
{
}


void CLandScape::CreateLandScape( const wstring & strPath, UINT iNumX, UINT iNumZ )
{
	wstring wstrFullPath = CPathMgr::GetResPath();
	wstrFullPath += strPath;

	string strFullPath{ wstrFullPath.begin(), wstrFullPath.end() };

	DWORD* pPixel = NULL;
	int iBitCount = 3;

	FILE* pFile = NULL;	

	fopen_s( &pFile, strFullPath.c_str(), "rb" );

	if ( pFile )
	{
		BITMAPFILEHEADER fh;
		BITMAPINFOHEADER ih;

		fread( &fh, sizeof( fh ), 1, pFile );
		fread( &ih, sizeof( ih ), 1, pFile );

		// 파일 위치 변경, SEEK_SET : 파일이 시작점
		fseek( pFile, fh.bfOffBits, SEEK_SET );

		pPixel = new DWORD[ih.biWidth * ih.biHeight];

		fread( pPixel, sizeof( DWORD ), ih.biWidth * ih.biHeight, pFile );

		// 비트맵 구조상 상하가 뒤집어져 저장되기에 바꾸어 준다.
		DWORD*	pLine = new DWORD[ih.biWidth];

		for ( int i = 0; i < ih.biHeight / 2; ++i )
		{
			memcpy( pLine, &pPixel[i * ih.biWidth], sizeof( DWORD ) * ih.biWidth );

			memcpy( &pPixel[i * ih.biWidth],
				&pPixel[( ih.biWidth - 1 - i ) * ih.biWidth],
				sizeof( DWORD ) * ih.biWidth );

			memcpy( &pPixel[( ih.biWidth - 1 - i ) * ih.biWidth],
				pLine, sizeof( DWORD ) * ih.biWidth );
		}

		delete[] pLine;
		pLine = NULL;

		fclose( pFile );
	}

	m_iNumX = iNumX;
	m_iNumZ = iNumZ;

	vector<VTX> vecVtx;
	vecVtx.resize( m_iNumX * m_iNumZ );

	for ( int i = 0; i < m_iNumZ; ++i )
	{
		for ( int j = 0; j < m_iNumZ; ++j )
		{
			VTX tVtx{ };

			tVtx.vPos = Vec3( ( float )j, ( pPixel[i * m_iNumX + j] & 0x000000ff ) * 0.03f, ( float )( m_iNumZ - 1 ) - i );
			m_vecPos.push_back( tVtx.vPos );

			tVtx.vNormal = Vec3( 0.f, 1.f, 0.f );
			//tVtx.vUV = Vec2( ( float )j / ( float )( m_iNumX - 1 ), ( float )i / ( float )( m_iNumZ - 1 ) );
			tVtx.vUV = Vec2( j, i );

			vecVtx[i * m_iNumX + j] = tVtx;
		}
	}

	delete[] pPixel;
	pPixel = NULL;

	vector<UINT> vecIndex( ( m_iNumX - 1 ) * ( m_iNumZ - 1 ) * 2 * 3 );

	int iCount = 0;

	for ( int i = 0; i < m_iNumZ - 1; ++i )
	{
		for ( int j = 0; j < m_iNumX - 1; ++j )
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = i * m_iNumX + j;
			// 우상단 삼각형 인덱스
			vecIndex[iCount++] = idx;
			vecIndex[iCount++] = idx + 1;
			vecIndex[iCount++] = idx + m_iNumX + 1;

			// 삼각형의 면법선을 구한다.
			Vec3	vEdge1 = m_vecPos[idx + 1] - m_vecPos[idx];
			Vec3	vEdge2 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vec3	vFaceNormal = vEdge1.Cross( vEdge2 );

			m_vecFaceNormal.push_back( vFaceNormal.Normalize() );

			// 좌하단 삼각형 인덱스
			vecIndex[iCount++] = idx;
			vecIndex[iCount++] = idx + m_iNumX + 1;
			vecIndex[iCount++] = idx + m_iNumX;

			// 삼각형의 면법선을 구한다.
			vEdge1 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];
			vEdge2 = m_vecPos[idx + m_iNumX] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross( vEdge2 );

			m_vecFaceNormal.push_back( vFaceNormal.Normalize() );
		}
	}

	ComputeNormal( vecVtx, vecIndex );
	ComputeTangent( vecVtx, vecIndex );

	Ptr<CMesh>	pMesh = new CMesh;
	pMesh->Create( sizeof( VTX ), ( UINT )vecVtx.size(), 
		( BYTE* )vecVtx.data(), DXGI_FORMAT_R32_UINT, 
		( UINT )vecIndex.size(), ( BYTE* )vecIndex.data() );
	
	CResMgr::GetInst()->AddRes( L"LandScapeMesh", pMesh );

	
}

void CLandScape::ComputeNormal( vector<VTX>& vecVtx, const vector<UINT>& vecIdx )
{
	for ( size_t i = 0; i < m_vecFaceNormal.size(); ++i )
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		vecVtx[idx0].vNormal += m_vecFaceNormal[i];
		vecVtx[idx1].vNormal += m_vecFaceNormal[i];
		vecVtx[idx2].vNormal += m_vecFaceNormal[i];
	}

	for ( size_t i = 0; i < vecVtx.size(); ++i )
	{
		vecVtx[i].vNormal = vecVtx[i].vNormal.Normalize();
	}
}

void CLandScape::ComputeTangent( vector<VTX>& vecVtx, const vector<UINT>& vecIdx )
{
	// 탄젠트 벡터 구함.
	for ( size_t i = 0; i < m_vecFaceNormal.size(); ++i )
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = vecVtx[idx1].vPos.x - vecVtx[idx0].vPos.x;
		fVtx1[1] = vecVtx[idx1].vPos.y - vecVtx[idx0].vPos.y;
		fVtx1[2] = vecVtx[idx1].vPos.z - vecVtx[idx0].vPos.z;

		fVtx2[0] = vecVtx[idx2].vPos.x - vecVtx[idx0].vPos.x;
		fVtx2[1] = vecVtx[idx2].vPos.y - vecVtx[idx0].vPos.y;
		fVtx2[2] = vecVtx[idx2].vPos.z - vecVtx[idx0].vPos.z;

		float	ftu[2], ftv[2];
		ftu[0] = vecVtx[idx1].vUV.x - vecVtx[idx0].vUV.x;
		ftv[0] = vecVtx[idx1].vUV.y - vecVtx[idx0].vUV.y;

		ftu[1] = vecVtx[idx2].vUV.x - vecVtx[idx0].vUV.x;
		ftv[1] = vecVtx[idx2].vUV.y - vecVtx[idx0].vUV.y;

		float	fDen = 1.f / ( ftu[0] * ftv[1] - ftu[1] * ftv[0] );

		Vec3	vTangent;
		vTangent.x = ( ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0] ) * fDen;
		vTangent.y = ( ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1] ) * fDen;
		vTangent.z = ( ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2] ) * fDen;

		vTangent = vTangent.Normalize();

		vecVtx[idx0].vTangent = vTangent;
		vecVtx[idx1].vTangent = vTangent;
		vecVtx[idx2].vTangent = vTangent;

		vecVtx[idx0].vBinormal = vecVtx[idx0].vNormal.Cross( vTangent ).Normalize();
		vecVtx[idx1].vBinormal = vecVtx[idx1].vNormal.Cross( vTangent ).Normalize();
		vecVtx[idx2].vBinormal = vecVtx[idx2].vNormal.Cross( vTangent ).Normalize();
	}
}

void CLandScape::CreateLandScapeInfo()
{
	vector<Vec3>::iterator iter = m_vecPos.begin();
	vector<Vec3>::iterator iterEnd = m_vecPos.end();

	Vec3 vMin = *iter;
	Vec3 vMax = *iter;

	for ( iter = m_vecPos.begin() + 1; iter != iterEnd; ++iter )
	{
		if ( vMin.x > ( *iter ).x )
			vMin.x = ( *iter ).x;

		if ( vMin.y > ( *iter ).y )
			vMin.y = ( *iter ).y;

		if ( vMin.z > ( *iter ).z )
			vMin.z = ( *iter ).z;

		if ( vMax.x < ( *iter ).x )
			vMax.x = ( *iter ).x;

		if ( vMax.y < ( *iter ).y )
			vMax.y = ( *iter ).y;

		if ( vMax.z < ( *iter ).z )
			vMax.z = ( *iter ).z;
	}
}

float CLandScape::GetY( const Vec3 & vPos )
{

	return 0.0f;
}
