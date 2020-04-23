#pragma once
#include "Entity.h"

// 버퍼의 데이터를 원하는 형태로 설계가능한 버퍼
// SRV와 UAV를 동시에 관리가능

class CStructuredBuffer :
	public CEntity
{
public:
	CStructuredBuffer();
	virtual	~CStructuredBuffer();

private:
	ComPtr<ID3D12Resource>			m_pBuffer;
	ComPtr<ID3D12DescriptorHeap>	m_pSRV;
	ComPtr<ID3D12DescriptorHeap>	m_pUAV;

	UINT							m_iElementSize;
	UINT							m_iElementCount;

	D3D12_RESOURCE_STATES			m_eResState;

public:
	void Create( UINT iElementSize, UINT iElementCount, void* pSysMem );

	void UpdateData( TEXTURE_REGISTER iRegister );		// rendering 단계
	void UpdateData_CS( TEXTURE_REGISTER iRegister );	// computeshader 단계
	void UpdateRWData( UAV_REGISTER iRegister );		// uav는 computershader 단계에서 밖에

	ComPtr<ID3D12DescriptorHeap> GetSRV();
	ComPtr<ID3D12DescriptorHeap> GetUAV();

	void SetResState( D3D12_RESOURCE_STATES eState );
	D3D12_RESOURCE_STATES GetResState();
	ComPtr<ID3D12Resource> GetBuffer();

	UINT GetElementSize();
	UINT GetElementCount();
	UINT GetBufferSize();
};

