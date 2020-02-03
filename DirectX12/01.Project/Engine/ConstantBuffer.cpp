#include "stdafx.h"
#include "ConstantBuffer.h"
#include "Device.h"

CConstantBuffer::CConstantBuffer() :
	m_pBuffer(NULL),
	m_pCBV(NULL),
	m_iBufferSize(0),
	m_eRegisterNum( CONST_REGISTER::b0 ),
	m_pData(NULL),
	m_tHeapDesc{},
	m_iMaxCount(2),
	m_iCurCount(0)
{
}


CConstantBuffer::~CConstantBuffer()
{
}

void CConstantBuffer::Create( UINT iBufferSize, UINT iMaxCount, CONST_REGISTER eRegister )
{
	m_iBufferSize = ( iBufferSize + 255 ) & ~255;			// 버퍼사이즈는 256바이트 단위
	m_iMaxCount = iMaxCount;
	m_eRegisterNum = eRegister;
	
	// 실제 상수버퍼를 GPU메모리에 할당
	D3D12_HEAP_PROPERTIES properties = {};
	properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	properties.CreationNodeMask = 1;
	properties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resource = {};
	resource.Alignment = 0;
	resource.DepthOrArraySize = 1;
	resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource.Format = DXGI_FORMAT_UNKNOWN;
	resource.Width = m_iBufferSize * m_iMaxCount;
	resource.Height = 1;
	resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource.MipLevels = 1;
	resource.SampleDesc.Count = 1;
	resource.SampleDesc.Quality = 0;

	DEVICE->CreateCommittedResource( &properties, D3D12_HEAP_FLAG_NONE,
		&resource, D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL, IID_PPV_ARGS( &m_pBuffer ) );

	// DescriptorHeap을 생성 (View, 상수버퍼랑 연결 될 전달자)
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
	cbvHeapDesc.NumDescriptors = m_iMaxCount;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap( &cbvHeapDesc, IID_PPV_ARGS( &m_pCBV ) );

	// DescriptorHeap과 Buffer를 연결
	m_hCBV = m_pCBV->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_hCBV;
	UINT iIncreSize = DEVICE->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
	for ( UINT i = 0; i < m_iMaxCount; ++i )
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = m_pBuffer->GetGPUVirtualAddress() + ( m_iBufferSize * i );
		cbvDesc.SizeInBytes = m_iBufferSize;
		DEVICE->CreateConstantBufferView( &cbvDesc, handle );
		handle.ptr += iIncreSize;
	}
	
	// 상수버퍼에 접근하기 위해서 맵핑을 해둔다.
	D3D12_RANGE readRange{ 0, 0 };
	m_pBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &m_pData ) );
	
}

void CConstantBuffer::SetData( void * pSrc )
{
	memcpy( m_pData, pSrc, m_iBufferSize );
}

void CConstantBuffer::SetData( void * pSrc, UINT iSize )
{
	memcpy( m_pData, pSrc, iSize );
}

void CConstantBuffer::SetData( void * pSrc, UINT iSize, UINT iOffserPos )
{
	memcpy( m_pData + ( m_iBufferSize * iOffserPos ), pSrc, iSize );
}

UINT CConstantBuffer::AddData( void * pSrc )
{
	// 지정한 크기의 상수버퍼를 넘어서게 데이터가 들어오는 경우
	assert( !( m_iCurCount >= m_iMaxCount ) );

	UINT iOffsetPos = m_iCurCount++;

	memcpy( m_pData + ( m_iBufferSize * iOffsetPos ), pSrc, m_iBufferSize );

	return iOffsetPos;
}

void CConstantBuffer::Clear()
{
	m_iCurCount = 0;
}

ComPtr<ID3D12DescriptorHeap> CConstantBuffer::GetCBV()
{
	return m_pCBV;
}

D3D12_CPU_DESCRIPTOR_HANDLE CConstantBuffer::GetHandle()
{
	return m_hCBV;
}

CONST_REGISTER CConstantBuffer::GetRegisterNum()
{
	return m_eRegisterNum;
}
