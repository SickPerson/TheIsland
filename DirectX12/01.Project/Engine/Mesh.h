#pragma once
#include "Resource.h"

class CMesh :
	public CResource
{
public:
	CMesh();
	virtual ~CMesh();

private:
	ComPtr<ID3D12Resource>		m_pVB;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem;

	ComPtr<ID3D12Resource>		m_pIB;
	D3D12_INDEX_BUFFER_VIEW		m_tIdxView;
	DXGI_FORMAT					m_eIdxFormat;
	UINT						m_iIdxCount;
	void*						m_pIdxSysMem;

public:
	void Create( UINT iVtxSize, UINT iVtxCount, BYTE* pVtxSysMem,
		DXGI_FORMAT eIdxFormat, UINT iIdxCount, BYTE* pIdxSysMem );

	void Render();

private:
	int GetSizeofFormat( DXGI_FORMAT eFormat );
};

