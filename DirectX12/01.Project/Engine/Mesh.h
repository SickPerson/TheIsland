#pragma once
#include "Resource.h"

#include "FBXLoader.h"
#include "Ptr.h"
#include "Texture.h"
#include "StructuredBuffer.h"

struct tIndexInfo
{
	ComPtr<ID3D12Resource>		pIB;
	D3D12_INDEX_BUFFER_VIEW		tIdxView;
	DXGI_FORMAT					eIdxFormat;
	UINT						iIdxCount;
	void*						pIdxSysMem;
};

class CMesh :
	public CResource
{
public:
	CMesh();
	virtual ~CMesh();

private:
	ComPtr<ID3D12Resource>		m_pVB;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView; // 버텍스 버퍼 정보 구조체(stride, total size, 가상 GPU 주소)

	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem;

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D	정보
	vector<tMTAnimClip>			m_vecAnimClip;
	vector<tMTBone>				m_vecBones;
	//Ptr<CTexture>				m_pBoneTex;

	CStructuredBuffer*			m_pBoneFrameData; // 전체 본 프레임 정보
	CStructuredBuffer*			m_pBoneOffset;	   // 각 뼈의 offset 행렬

public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);
	void Render( UINT iSubSet = 0 );
	void Render_Instancing( UINT iInstancCount, UINT iSubset = 0 );
	static CMesh* CreateFromContainer( CFBXLoader & loader );

public:
	UINT GetSubsetCount();
	const vector<tMTBone>* GetBones();
	//void SetBoneTex( Ptr<CTexture> pTex );
	const vector<tMTAnimClip>* GetAnimClip();
	//Ptr<CTexture> GetBoneTex();
	bool IsAnimMesh();
	CStructuredBuffer*	GetBoneFrameDataBuffer();
	CStructuredBuffer*	GetBoneOffsetBuffer();
	UINT GetBoneCount();

public:
	virtual void Load( const wstring& strPath );
	virtual void Save( const wstring& strPath );
};

