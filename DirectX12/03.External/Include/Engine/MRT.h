#pragma once
#include "Entity.h"

#include "Ptr.h"
#include "Texture.h"

struct tRT
{
	Ptr<CTexture>	pTarget;
	Vec4			vClearColor;
};

class CMRT :
	public CEntity
{
public:
	CMRT();
	virtual ~CMRT();

private:
	tRT								m_arrRT[8];
	UINT							m_iRTCount;
	Ptr<CTexture>					m_pDSTex;
	ComPtr<ID3D12DescriptorHeap>	m_pRTV;

public:
	void Create( UINT iCount, tRT* arrRT, Ptr<CTexture> pDSTex);
	void OMSet( UINT iCount, UINT iOffset = 0 );
	void OMSet();
	void Clear();
	void Clear( UINT iRTidx );

	Ptr<CTexture>	GetRTTex( UINT iIdx );
	Ptr<CTexture>	GetDSTex();
};

