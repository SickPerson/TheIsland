#include "stdafx.h"
#include "Texture.h"
#include "PathManager.h"
#include "Device.h"

CTexture::CTexture() :
	CResource(RES_TYPE::TEXTURE),
	m_pTex2D(NULL),
	m_pSRV(NULL)
{
}

CTexture::~CTexture()
{
}

void CTexture::Load( const wchar_t * pFileName )
{
	const wchar_t* pPath = GET_SINGLE( CPathManager )->FindPath( TEXTURE_PATH );
	wstring strFullPath = pPath;
	strFullPath += pFileName;

	wchar_t szExt[50] = L"";
	_wsplitpath_s( strFullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50 );

	if ( lstrcmp( szExt, L".DDS" ) == 0 || lstrcmp( szExt, L".dds" ) == 0 )
	{
		if ( FAILED( LoadFromDDSFile( strFullPath.c_str(), DDS_FLAGS_NONE, NULL, m_Image ) ) )
		{
			assert( nullptr );
		}
	}

	else if ( lstrcmp( szExt, L".TGA" ) == 0 || lstrcmp( szExt, L".tga" ) == 0 )
	{
		if ( FAILED( LoadFromTGAFile( strFullPath.c_str(), NULL, m_Image ) ) )
		{
			assert( nullptr );
		}
	}

	else // png, jpg, jpeg, bmp (WIC)
	{
		if ( FAILED( LoadFromWICFile( strFullPath.c_str(), WIC_FLAGS_NONE, NULL, m_Image ) ) )
		{
			assert( nullptr );
		}
	}

	HRESULT hr = CreateTexture( DEVICE.Get(), m_Image.GetMetadata(), &m_pTex2D );

	if ( FAILED( hr ) )
		assert( nullptr );

	vector<D3D12_SUBRESOURCE_DATA> vecSubresources;

	hr = PrepareUpload( DEVICE.Get(), m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), vecSubresources );

	if ( FAILED( hr ) )
		assert( nullptr );

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize( m_pTex2D.Get(), 0, static_cast< UINT >( vecSubresources.size() ) );

	ComPtr<ID3D12Resource> textureUploadHeap;

	hr = DEVICE->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
		D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer( uploadBufferSize ),
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS( textureUploadHeap.GetAddressOf() ) );

	if ( FAILED( hr ) )
		assert( NULL );

	UpdateSubresources( CMDLIST_RES.Get(),
		m_pTex2D.Get(), textureUploadHeap.Get(), 0, 0,
		static_cast< UINT >( vecSubresources.size() ), vecSubresources.data() );

	GET_SINGLE( CDevice )->ExcuteResourceLoad();

	// SRV¸¸µé±â
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap( &srvHeapDesc, IID_PPV_ARGS( &m_pSRV ) );

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pSRV->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_Image.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView( m_pTex2D.Get(), &srvDesc, m_pSRV->GetCPUDescriptorHandleForHeapStart() );
}

ComPtr<ID3D12DescriptorHeap> CTexture::GetSRV()
{
	return m_pSRV;
}

float CTexture::Width()
{
	return ( float )m_Image.GetMetadata().width;
}

float CTexture::Height()
{
	return ( float )m_Image.GetMetadata().height;
}
