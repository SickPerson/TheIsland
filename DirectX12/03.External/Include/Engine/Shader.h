#pragma once
#include "Resource.h"

struct tShaderParam
{
	wstring			strParamName;
	SHADER_PARAM	eType;
};

class CShader :
	public CResource
{
public:
	CShader();
	virtual ~CShader();

private:
	ComPtr<ID3DBlob>					m_pVSBlob;
	ComPtr<ID3DBlob>					m_pHSBlob;
	ComPtr<ID3DBlob>					m_pDSBlob;
	ComPtr<ID3DBlob>					m_pGSBlob;
	ComPtr<ID3DBlob>					m_pPSBlob;
	ComPtr<ID3DBlob>					m_pErrBlob;
	ComPtr<ID3DBlob>					m_pCSBlob;

	ComPtr<ID3D12PipelineState>			m_pPilelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  m_tPipeline;

	D3D_PRIMITIVE_TOPOLOGY				m_eTopology;
	vector<tShaderParam>				m_vecShaderPamam;

	RS_TYPE								m_eRSType;
	BLEND_TYPE							m_eBlendType;
	DEPTH_STENCIL_TYPE					m_eDSType;

public:
	void Create( D3D_PRIMITIVE_TOPOLOGY eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	void CreateVertexShader( const wstring& strFilePath, const string& strFuncName, const string& strhlslVersion = "vs_5_0" );
	void CreatePixelShader( const wstring& strFilePath, const string& strFuncName, const string& strhlslVersion = "ps_5_0" );

public:
	void SetBlendState( BLEND_TYPE eType );
	void SetDepthStencilState( DEPTH_STENCIL_TYPE eType );
	void SetRasterizerType( RS_TYPE eType );

public:
	void UpdateData();
	void AddShaderParam( const tShaderParam& param );
};

