#pragma once

class CConstantBuffer;
class CTexture;

class CDevice
{
public:
	HWND									m_hWnd;
	RESOLUTION								m_tResolution;
	bool									m_bWindow;

private:
	ComPtr<ID3D12Device>					m_pDevice;		// 메모리 할당, 객체 생성
	ComPtr<ID3D12PipelineState>				m_pPipelineState;
	ComPtr<ID3D12CommandQueue>				m_pCmdQueue;

	// 렌더링 커맨드리스트
	ComPtr<ID3D12CommandAllocator>			m_pCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>		m_pCmdListGraphic;

	// 리소스 로딩 리스트
	ComPtr<ID3D12CommandAllocator>			m_pCmdAllocRes;
	ComPtr<ID3D12GraphicsCommandList>		m_pCmdListRes;

	ComPtr<ID3D12Fence>						m_pFence;
	ComPtr<IDXGIFactory>					m_pFactory;
	ComPtr<ID3D12Debug>						m_pDbgCtrl;		// 디버그 관리	

	// SwapChain
	ComPtr<IDXGISwapChain>					m_pSwapChain;

	// RenderTarget
	ComPtr<ID3D12Resource>					m_RenderTargets[2];
	ComPtr<ID3D12DescriptorHeap>			m_pRTV;
	size_t									m_iRTVHeapSize;

	// DepthStencil
	ComPtr<ID3D12Resource>					m_pDepthStencilTex;
	ComPtr<ID3D12DescriptorHeap>			m_pDSV;

	D3D12_VIEWPORT							m_tVP;
	D3D12_RECT								m_tScissorRect;

	HANDLE									m_hFenceEvent;
	size_t									m_iFenceValue;
	UINT									m_iCurTargetIdx;

	vector<ComPtr<ID3D12DescriptorHeap>>	m_vecDummyDescriptor;
	ComPtr<ID3D12DescriptorHeap>			m_pInitDescriptor;
	UINT									m_iCurDummyIdx;
	UINT									m_iCBVIncreSize;
	
	vector<CConstantBuffer*>				m_vecCB;

	vector<D3D12_STATIC_SAMPLER_DESC>		m_vecSamplerDesc;

private:
	ComPtr<ID3D12RootSignature>		m_arrSig[( UINT )ROOT_SIG_TYPE::END];

public:
	int Init( HWND hWnd, const RESOLUTION& res, bool bWindow );
	void Render_Start( float( &arrFloat )[4] );
	void Render_Present();
	void WaitForFenceEvent();

	void CreateConstBuffer( const wstring& strName, size_t iSize, size_t iMaxSize,
		CONST_REGISTER eRegister, bool bGlobal = false );
	void SetConstBufferToRegister( CConstantBuffer* pCB, UINT iOffset );
	void SetGlobalConstBufferToRegister( CConstantBuffer* pCB, UINT iOffset );
	void SetTextrueToRegister( CTexture* pTexture, TEXTURE_REGISTER eRegister );
	void ClearDummyDescriptorHeap( UINT iDummyIndex );

	void UpdateTable();
	void ExcuteResourceLoad();

private:
	void CreateSwapChain();
	void CreateView();
	void CreateViewPort();
	void CreateRootSignature();
	void CreateSamplerDesc();

public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList();
	ComPtr<ID3D12GraphicsCommandList> GetCmdListRes();
	ComPtr<ID3D12Device> GetDevice();
	ComPtr<ID3D12RootSignature> GetRootSignature( ROOT_SIG_TYPE eType );
	CConstantBuffer* GetConstBuffer( CONST_REGISTER eRegister );

	DECLARE_SINGLE(CDevice)
};

