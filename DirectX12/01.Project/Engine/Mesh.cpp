#include "stdafx.h"
#include "Mesh.h"

#include "Device.h"
#include "PathMgr.h"

CMesh::CMesh()
	: CResource(RES_TYPE::MESH)
	, m_pVB(nullptr)
	, m_tVtxView{}	
	, m_iVtxSize(0)
	, m_iVtxCount(0)	
    //, m_eIdxFormat(DXGI_FORMAT::DXGI_FORMAT_R32_UINT)
    //, m_iIdxCount(0)
    //, m_eIdxUsage(D3D11_USAGE::D3D11_USAGE_DEFAULT)
    , m_pVtxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSysMem);

	for ( int i = 0; i < m_vecIdxInfo.size(); ++i )
	{
		SAFE_DELETE( m_vecIdxInfo[i].pIdxSysMem );
	}
}

void CMesh::Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
	, DXGI_FORMAT _eIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem)
{
	m_iVtxSize = _iVtxSize;
	m_iVtxCount = _iVtxCount;
	
	tIndexInfo tInfo = {};
	tInfo.eIdxFormat = _eIdxFormat;
	tInfo.iIdxCount = _iIdxCount;


	D3D12_HEAP_PROPERTIES tHeapProperty = {};
		
	tHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	tHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	tHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	tHeapProperty.CreationNodeMask = 1;
	tHeapProperty.VisibleNodeMask = 1;
	
	D3D12_RESOURCE_DESC tResDesc = {};

	tResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	tResDesc.Alignment = 0;
	tResDesc.Width = m_iVtxSize * m_iVtxCount;
	tResDesc.Height = 1;
	tResDesc.DepthOrArraySize = 1;
	tResDesc.MipLevels = 1;
	tResDesc.Format = DXGI_FORMAT_UNKNOWN;
	tResDesc.SampleDesc.Count = 1;
	tResDesc.SampleDesc.Quality = 0;
	tResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	tResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	DEVICE->CreateCommittedResource(
		&tHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&tResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pVB));

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin = nullptr;
	D3D12_RANGE readRange{ 0, 0 }; // We do not intend to read from this resource on the CPU.	
	m_pVB->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
	memcpy(pVertexDataBegin, _pVtxSysMem, (tResDesc.Width * tResDesc.Height) );
	m_pVB->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_tVtxView.BufferLocation = m_pVB->GetGPUVirtualAddress();
	m_tVtxView.StrideInBytes = sizeof(VTX);
	m_tVtxView.SizeInBytes = (UINT)tResDesc.Width;
	   
	// IdxBuffer
	tHeapProperty = {};
	tHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	tHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	tHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	tHeapProperty.CreationNodeMask = 1;
	tHeapProperty.VisibleNodeMask = 1;

	tResDesc = {};
	tResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	tResDesc.Alignment = 0;
	tResDesc.Width = GetSizeofFormat( tInfo.eIdxFormat ) * tInfo.iIdxCount;
	tResDesc.Height = 1;
	tResDesc.DepthOrArraySize = 1;
	tResDesc.MipLevels = 1;
	tResDesc.Format = DXGI_FORMAT_UNKNOWN;
	tResDesc.SampleDesc.Count = 1;
	tResDesc.SampleDesc.Quality = 0;
	tResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	tResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	DEVICE->CreateCommittedResource(
		&tHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&tResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tInfo.pIB));

	// Copy the Index data to the Index buffer.
	UINT8* pIdxDataBegin = nullptr;
	readRange = D3D12_RANGE{ 0, 0 }; // We do not intend to read from this resource on the CPU.	
	tInfo.pIB->Map(0, &readRange, reinterpret_cast<void**>(&pIdxDataBegin));
	memcpy(pIdxDataBegin, _pIdxSysMem, (tResDesc.Width * tResDesc.Height) );
	tInfo.pIB->Unmap(0, nullptr);

	// Initialize the Index buffer view.
	tInfo.tIdxView.BufferLocation = tInfo.pIB->GetGPUVirtualAddress();
	tInfo.tIdxView.Format = DXGI_FORMAT_R32_UINT;
	tInfo.tIdxView.SizeInBytes = (UINT)(tResDesc.Width * tResDesc.Height);

	// IndexInfo 에 Subset 추가
	m_vecIdxInfo.push_back( tInfo );

	CDevice::GetInst()->WaitForFenceEvent();
}

void CMesh::Render( UINT iSubset )
{
	CDevice::GetInst()->UpdateTable();	
	
	CMDLIST->IASetVertexBuffers( 0, 1, &m_tVtxView );
	CMDLIST->IASetIndexBuffer( &m_vecIdxInfo[iSubset].tIdxView );

	//CMDLIST->DrawInstanced(3, 1, 0, 0);
	CMDLIST->DrawIndexedInstanced( m_vecIdxInfo[iSubset].iIdxCount, 1, 0, 0, 0 );
}

CMesh * CMesh::CreateFromContainer( CFBXLoader & loader )
{
	const tContainer* container = &loader.GetContainer( 0 );

	ComPtr<ID3D12Resource> pVB = NULL;
	D3D12_VERTEX_BUFFER_VIEW tVtxView{};
	UINT iVtxCount = ( UINT )container->vecPos.size();
	UINT iVtxSize = sizeof( VTX );

	VTX* pSysMem = new VTX[iVtxSize];

	for ( UINT i = 0; i < iVtxCount; ++i )
	{
		pSysMem[i].vPos = container->vecPos[i];
		pSysMem[i].vUV = container->vecUV[i];
		pSysMem[i].vColor = Vec4{ 1.f, 0.f, 1.f, 1.f };
		pSysMem[i].vNormal = container->vecNormal[i];
		pSysMem[i].vTangent = container->vecTangent[i];
		pSysMem[i].vBinormal = container->vecBinormal[i];
		pSysMem[i].vWeights = container->vecWeights[i];
		pSysMem[i].vIndices = container->vecIndices[i];
	}

	D3D12_HEAP_PROPERTIES tHeapProperty{};
	tHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	tHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	tHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	tHeapProperty.CreationNodeMask = 1;
	tHeapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC tResDesc{};
	tResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	tResDesc.Alignment = 0;
	tResDesc.Width = iVtxSize * iVtxCount;
	tResDesc.Height = 1;
	tResDesc.DepthOrArraySize = 1;
	tResDesc.MipLevels = 1;
	tResDesc.Format = DXGI_FORMAT_UNKNOWN;
	tResDesc.SampleDesc.Count = 1;
	tResDesc.SampleDesc.Quality = 0;
	tResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	tResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	DEVICE->CreateCommittedResource(
		&tHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&tResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &pVB ) );

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin = nullptr;
	D3D12_RANGE readRange{ 0, 0 }; // We do not intend to read from this resource on the CPU.	
	pVB->Map( 0, &readRange, reinterpret_cast< void** >( &pVertexDataBegin ) );
	memcpy( pVertexDataBegin, pSysMem, ( tResDesc.Width * tResDesc.Height ) );
	pVB->Unmap( 0, nullptr );

	// Initialize the vertex buffer view.
	tVtxView.BufferLocation = pVB->GetGPUVirtualAddress();
	tVtxView.StrideInBytes = sizeof( VTX );
	tVtxView.SizeInBytes = ( UINT )tResDesc.Width;

	CMesh* pMesh = new CMesh;
	pMesh->m_pVB = pVB;
	pMesh->m_iVtxCount = iVtxCount;
	pMesh->m_iVtxSize = iVtxSize;
	pMesh->m_tVtxView = tVtxView;
	pMesh->m_pVtxSysMem = pSysMem;

	// 인덱스 정보
	UINT iIdxBufferCount = ( UINT )container->vecIdx.size();

	for ( UINT i = 0; i < iIdxBufferCount; ++i )
	{
		tIndexInfo info = {};
		info.iIdxCount = ( UINT )container->vecIdx[i].size();
		info.eIdxFormat = DXGI_FORMAT_R32_UINT;
		info.pIdxSysMem = malloc( GetSizeofFormat( info.eIdxFormat ) * info.iIdxCount );
		memcpy( info.pIdxSysMem, &container->vecIdx[i][0], GetSizeofFormat( info.eIdxFormat ) * info.iIdxCount );


		tHeapProperty = {};
		tHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
		tHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		tHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		tHeapProperty.CreationNodeMask = 1;
		tHeapProperty.VisibleNodeMask = 1;

		tResDesc = {};
		tResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		tResDesc.Alignment = 0;
		tResDesc.Width = GetSizeofFormat( info.eIdxFormat ) * info.iIdxCount;
		tResDesc.Height = 1;
		tResDesc.DepthOrArraySize = 1;
		tResDesc.MipLevels = 1;
		tResDesc.Format = DXGI_FORMAT_UNKNOWN;
		tResDesc.SampleDesc.Count = 1;
		tResDesc.SampleDesc.Quality = 0;
		tResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		tResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		DEVICE->CreateCommittedResource(
			&tHeapProperty,
			D3D12_HEAP_FLAG_NONE,
			&tResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &info.pIB ) );

		// Copy the Index data to the Index buffer.
		UINT8* pIdxDataBegin = nullptr;
		readRange = D3D12_RANGE{ 0, 0 }; // We do not intend to read from this resource on the CPU.	
		info.pIB->Map( 0, &readRange, reinterpret_cast< void** >( &pIdxDataBegin ) );
		memcpy( pIdxDataBegin, info.pIdxSysMem, ( tResDesc.Width * tResDesc.Height ) );
		info.pIB->Unmap( 0, nullptr );

		// Initialize the Index buffer view.
		info.tIdxView.BufferLocation = info.pIB->GetGPUVirtualAddress();
		info.tIdxView.Format = info.eIdxFormat;
		info.tIdxView.SizeInBytes = ( UINT )( tResDesc.Width * tResDesc.Height );

		// IndexInfo 에 Subset 추가
		pMesh->m_vecIdxInfo.push_back( info );
	}

	return pMesh;
}

UINT CMesh::GetSubsetCount()
{
	return (UINT)m_vecIdxInfo.size();
}

const vector<tMTBone>* CMesh::GetBones()
{
	return &m_vecBones;
}

void CMesh::SetBoneTex( Ptr<CTexture> pTex )
{
	m_pBoneTex = pTex;
}

const vector<tMTAnimClip>* CMesh::GetAnimClip()
{
	return &m_vecAnimClip;
}

Ptr<CTexture> CMesh::GetBoneTex()
{
	return m_pBoneTex;
}

bool CMesh::IsAnimMesh()
{
	return !m_vecAnimClip.empty();
}

void CMesh::Load( const wstring & strPath )
{
	FILE* pFile = nullptr;
	_wfopen_s( &pFile, strPath.c_str(), L"rb" );

	// 키값
	SetName( LoadWString( pFile ) );

	// 상대경로
	SetPath( LoadWString( pFile ) );

	// 로드
	// 정점데이터
	fread( &m_iVtxSize, sizeof( int ), 1, pFile );
	fread( &m_iVtxCount, sizeof( int ), 1, pFile );

	UINT iByteSize = 0;
	fread( &iByteSize, sizeof( int ), 1, pFile );

	m_pVtxSysMem = malloc( iByteSize );
	fread( m_pVtxSysMem, 1, iByteSize, pFile );

	fclose( pFile );
}

void CMesh::Save( const wstring & strPath )
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += strPath;
	SetPath( strPath );

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s( &pFile, strFileName.c_str(), L"wb" );

	assert( pFile );

	// 키값
	SaveWString( pFile, GetName().c_str() );

	// 상대 경로
	SaveWString( pFile, GetPath().c_str() );

	// 정점 데이터 저장	
	fwrite( &m_iVtxSize, sizeof( int ), 1, pFile );
	fwrite( &m_iVtxCount, sizeof( int ), 1, pFile );

	int iByteSize = m_iVtxSize * m_iVtxCount;
	fwrite( &iByteSize, sizeof( int ), 1, pFile );
	fwrite( m_pVtxSysMem, iByteSize, 1, pFile );

	// 인덱스 정보
	UINT iMtrlCount = ( UINT )m_vecIdxInfo.size();
	fwrite( &iMtrlCount, sizeof( int ), 1, pFile );

	UINT iIdxBuffSize = 0;
	for ( UINT i = 0; i < iMtrlCount; ++i )
	{
		fwrite( &m_vecIdxInfo[i], sizeof( tIndexInfo ), 1, pFile );
		fwrite( m_vecIdxInfo[i].pIdxSysMem
			, m_vecIdxInfo[i].iIdxCount * GetSizeofFormat( m_vecIdxInfo[i].eIdxFormat )
			, 1, pFile );
	}

	// Animation3D 정보 
	UINT iCount = ( UINT )m_vecAnimClip.size();
	fwrite( &iCount, sizeof( int ), 1, pFile );
	for ( UINT i = 0; i < iCount; ++i )
	{
		SaveWString( pFile, m_vecAnimClip[i].strAnimName.c_str() );
		fwrite( &m_vecAnimClip[i].dStartTime, sizeof( double ), 1, pFile );
		fwrite( &m_vecAnimClip[i].dEndTime, sizeof( double ), 1, pFile );
		fwrite( &m_vecAnimClip[i].dTimeLength, sizeof( double ), 1, pFile );
		fwrite( &m_vecAnimClip[i].eMode, sizeof( int ), 1, pFile );
		fwrite( &m_vecAnimClip[i].fUpdateTime, sizeof( float ), 1, pFile );
		fwrite( &m_vecAnimClip[i].iStartFrame, sizeof( int ), 1, pFile );
		fwrite( &m_vecAnimClip[i].iEndFrame, sizeof( int ), 1, pFile );
		fwrite( &m_vecAnimClip[i].iFrameLength, sizeof( int ), 1, pFile );
	}

	iCount = ( UINT )m_vecBones.size();
	fwrite( &iCount, sizeof( int ), 1, pFile );

	for ( UINT i = 0; i < iCount; ++i )
	{
		SaveWString( pFile, m_vecBones[i].strBoneName.c_str() );
		fwrite( &m_vecBones[i].iDepth, sizeof( int ), 1, pFile );
		fwrite( &m_vecBones[i].iParentIndx, sizeof( int ), 1, pFile );
		fwrite( &m_vecBones[i].matBone, sizeof( Matrix ), 1, pFile );
		fwrite( &m_vecBones[i].matOffset, sizeof( Matrix ), 1, pFile );

		int iFrameCount = ( int )m_vecBones[i].vecKeyFrame.size();
		fwrite( &iFrameCount, sizeof( int ), 1, pFile );

		for ( int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j )
		{
			fwrite( &m_vecBones[i].vecKeyFrame[j], sizeof( tMTKeyFrame ), 1, pFile );
		}
	}

	fclose( pFile );
}
