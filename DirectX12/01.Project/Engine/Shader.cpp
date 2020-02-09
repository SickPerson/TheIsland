#include "stdafx.h"
#include "Shader.h"
#include "Device.h"
#include "PathManager.h"

CShader::CShader()
	: CResource( RES_TYPE::SHADER )
	, m_pVSBlob( nullptr )
	, m_pHSBlob( nullptr )
	, m_pDSBlob( nullptr )
	, m_pGSBlob( nullptr )
	, m_pPSBlob( nullptr )
	, m_pErrBlob( nullptr )
	, m_pCSBlob( nullptr )
	, m_pPilelineState( nullptr )
	, m_tPipeline{}
{
}


CShader::~CShader()
{
}

void CShader::Create( D3D_PRIMITIVE_TOPOLOGY eTopology )
{
	m_eTopology = eTopology;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};


	m_tPipeline.InputLayout = { inputElementDescs, _countof( inputElementDescs ) };
	m_tPipeline.pRootSignature = ROOTSIGNATURE( ROOT_SIG_TYPE::INPUT_ASSEM ).Get();

	m_tPipeline.RasterizerState = g_arrRSDesc[( UINT )m_eRSType];
	m_tPipeline.BlendState = g_arrBlendDesc[( UINT )m_eBlendType];
	m_tPipeline.DepthStencilState = g_arrDepthStencilDesc[( UINT )m_eDSType];
	m_tPipeline.SampleMask = UINT_MAX;
	m_tPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_tPipeline.NumRenderTargets = 1;
	m_tPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_tPipeline.SampleDesc.Count = 1;

	HRESULT hr = DEVICE->CreateGraphicsPipelineState( &m_tPipeline, IID_PPV_ARGS( &m_pPilelineState ) );
	if ( FAILED( hr ) )
		assert( nullptr );
}

void CShader::CreateVertexShader( const wstring & strFilePath, const string & strFuncName, const string & strhlslVersion )
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	wstring strPath = GET_SINGLE(CPathManager)->GetResPath();
	strPath += strFilePath;

	char* pErr = nullptr;

	if ( FAILED( D3DCompileFromFile( strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, strFuncName.c_str(), strhlslVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob ) ) )
	{
		pErr = ( char* )m_pErrBlob->GetBufferPointer();
		MessageBoxA( nullptr, pErr, "Shader Create Failed !!!", MB_OK );
	}

	m_tPipeline.VS = { m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize() };
}

void CShader::CreatePixelShader( const wstring & strFilePath, const string & strFuncName, const string & strhlslVersion )
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = GET_SINGLE( CPathManager )->GetResPath();
	strPath += strFilePath;

	char* pErr = nullptr;

	if ( FAILED( D3DCompileFromFile( strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, strFuncName.c_str(), strhlslVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob ) ) )
	{
		pErr = ( char* )m_pErrBlob->GetBufferPointer();
		MessageBoxA( nullptr, pErr, "Shader Create Failed !!!", MB_OK );
	}

	m_tPipeline.PS = { m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize() };
}

void CShader::SetBlendState( BLEND_TYPE eType )
{
	m_eBlendType = eType;
}

void CShader::SetDepthStencilState( DEPTH_STENCIL_TYPE eType )
{
	m_eDSType = eType;
}

void CShader::SetRasterizerType( RS_TYPE eType )
{
	m_eRSType = eType;
}

void CShader::UpdateData()
{
	CMDLIST->SetPipelineState( m_pPilelineState.Get() );
	CMDLIST->IASetPrimitiveTopology( m_eTopology );
}

void CShader::AddShaderParam( const tShaderParam & param )
{
	m_vecShaderPamam.push_back( param );
}
