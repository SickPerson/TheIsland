#pragma once

#include "Resource.h"

#include <DirectTex/DirectXTex.h>
#include <DirectTex/DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectTex\\DirectXTex.lib")
#endif

class CTexture :
	public CResource
{
public:
	CTexture();
	virtual ~CTexture();

private:
	ScratchImage					m_Image;
	ComPtr<ID3D12Resource>			m_pTex2D;
	ComPtr<ID3D12DescriptorHeap>	m_pSRV;

public:
	virtual void Load( const wchar_t* pFileName );

	ComPtr<ID3D12DescriptorHeap> GetSRV();

	float Width();
	float Height();
};

