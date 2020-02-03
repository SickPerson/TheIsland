#pragma once

#include "Entity.h"

class CConstantBuffer : 
	public CEntity
{
private:
	friend class CDevice;

private:
	CConstantBuffer();
	virtual ~CConstantBuffer();

private:
	ComPtr<ID3D12Resource>			m_pBuffer;			// 실제 상수버퍼(리소스)
	ComPtr<ID3D12DescriptorHeap>	m_pCBV;				// 상수버퍼 설명자 힙(View 역할)
	D3D12_CPU_DESCRIPTOR_HANDLE		m_hCBV;				// 설명자 힙 핸들
	UINT							m_iBufferSize;		// 상수버퍼 개당 크기
	UINT							m_iMaxCount;		// 상수버퍼 최대 개수
	UINT							m_iCurCount;		// 상수버퍼에 Add된 현재 데이터 개수
	CONST_REGISTER					m_eRegisterNum;		// 상수버퍼 대용 레지스터 번호
	BYTE*							m_pData;			// 상수버퍼와 맵핑된 주소

	D3D12_DESCRIPTOR_HEAP_DESC		m_tHeapDesc;		// 상수버퍼와 설명자 힙을 연결하는 정보

public:
	void Create( UINT iBufferSize, UINT iMaxCount, CONST_REGISTER eRegister );
	void SetData( void* pSrc );
	void SetData( void* pSrc, UINT iSize );
	void SetData( void* pSrc, UINT iSize, UINT iOffserPos );
	UINT AddData( void* pSrc );
	void Clear();

	ComPtr<ID3D12DescriptorHeap> GetCBV();
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle();
	CONST_REGISTER	GetRegisterNum();
};

