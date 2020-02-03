#include "stdafx.h"
#include "Device.h"
#include "ConstantBuffer.h"
#include "Texture.h"

DEFINITION_SINGLE( CDevice )

CDevice::CDevice() :
	m_pDevice( NULL ),
	m_pFence( NULL ),
	m_pFactory( NULL ),
	m_iCurTargetIdx( 0 ),
	m_hFenceEvent( NULL ),
	m_iFenceValue( 0 ),
	m_iCurDummyIdx( 0 )
{
}

CDevice::~CDevice()
{
	WaitForFenceEvent();
	CloseHandle( m_hFenceEvent );

	for ( size_t i = 0; i < m_vecCB.size(); ++i )
	{
		SAFE_DELETE( m_vecCB[i] );
	}
}

int CDevice::Init( HWND hWnd, const RESOLUTION & res, bool bWindow )
{
	m_hWnd = hWnd;
	m_tResolution = res;
	m_bWindow = bWindow;

	UINT iFlag = 0;

#ifdef _DEBUG
	D3D12GetDebugInterface( IID_PPV_ARGS( &m_pDbgCtrl ) );
	m_pDbgCtrl->EnableDebugLayer();
#endif	

	CreateDXGIFactory( IID_PPV_ARGS( &m_pFactory ) );

	// CreateDevice
	D3D12CreateDevice( nullptr, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS( &m_pDevice ) );

	// CreateFence
	m_pDevice->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &m_pFence ) );
	m_iFenceValue = 1;

	// Create an event handle to use for frame synchronization.
	m_hFenceEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	if ( m_hFenceEvent == nullptr )
	{
		assert( nullptr );
	}

	// Command Queue 	
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	m_pDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS( &m_pCmdQueue ) );

	// Create Command Allocator
	m_pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &m_pCmdAlloc ) );

	// Creat CmdListRes
	m_pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &m_pCmdAllocRes ) );

	// Create the command list.
	m_pDevice->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT
		, m_pCmdAlloc.Get(), nullptr, IID_PPV_ARGS( &m_pCmdListGraphic ) );

	// CmdListGraphic를 사용하기 전에 닫아줘야한다.
	m_pCmdListGraphic->Close();

	m_pDevice->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAllocRes.Get(), NULL, IID_PPV_ARGS( &m_pCmdListRes ) );
	
	// SwapChain 만들기
	CreateSwapChain();

	// View 만들기
	CreateView();

	// ViewPort 만들기
	CreateViewPort();

	// Empty Signature 만들기
	// 루트 서명 
	// 그리기 호출 전에 해당 자원이 파이프라인에 묶일 자료이며,
	// 어느 시점에 묶이는지, 또는 쉐이더 자원으로 사용 시 레지스터 대응정보를 기술한다.
	D3D12_ROOT_SIGNATURE_DESC sigDesc = {};
	sigDesc.NumParameters = 0;
	sigDesc.pParameters = nullptr;
	sigDesc.NumStaticSamplers = 0;
	sigDesc.pStaticSamplers = nullptr; // 사용 될 Sampler 정보
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> pSignature;
	ComPtr<ID3DBlob> pError;
	D3D12SerializeRootSignature( &sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError );
	m_pDevice->CreateRootSignature( 0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS( &m_arrSig[( UINT )ROOT_SIG_TYPE::INPUT_ASSEM] ) );

	return S_OK;
}

void CDevice::Render_Start( float( &arrFloat )[4] )
{
	m_iCurDummyIdx = 0;

	// 그리기 준비
	m_pCmdAlloc->Reset();

	m_pCmdListGraphic->Reset( m_pCmdAlloc.Get(), NULL );

	m_pCmdListGraphic->SetGraphicsRootSignature( m_arrSig[( UINT )ROOT_SIG_TYPE::INPUT_ASSEM].Get() );

	// 필요한 상태 설정
	m_pCmdListGraphic->RSSetViewports( 1, &m_tVP );
	m_pCmdListGraphic->RSSetScissorRects( 1, &m_tScissorRect );

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; ;
	barrier.Transition.pResource = m_RenderTargets[m_iCurTargetIdx].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// 출력에서
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 다시 백버퍼로 지정
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_pCmdListGraphic->ResourceBarrier( 1, &barrier );
	
	// RenderTarget과 DepthStencilView를 연결
	D3D12_CPU_DESCRIPTOR_HANDLE	hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSV->GetCPUDescriptorHandleForHeapStart();

	// 타겟 지정
	hRTVHandle.ptr += m_iCurTargetIdx * m_iRTVHeapSize;
	m_pCmdListGraphic->OMSetRenderTargets( 1, &hRTVHandle, FALSE, nullptr );

	// 타겟 클리어
	m_pCmdListGraphic->ClearRenderTargetView( hRTVHandle, arrFloat, 0, NULL );
	m_pCmdListGraphic->ClearDepthStencilView( hDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, NULL );

	// 첫 번째 더미 Descriptor Heap 초기화
	ClearDummyDescriptorHeap( 0 );
}

void CDevice::Render_Present()
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; ;
	barrier.Transition.pResource = m_RenderTargets[m_iCurTargetIdx].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 백버퍼에서
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 다시 출력으로 지정

	m_pCmdListGraphic->ResourceBarrier( 1, &barrier );
	m_pCmdListGraphic->Close();

	// 커맨드 리스트 수행	
	ID3D12CommandList* ppCommandLists[] = { m_pCmdListGraphic.Get() };
	m_pCmdQueue->ExecuteCommandLists( _countof( ppCommandLists ), ppCommandLists );

	// Present the frame.
	m_pSwapChain->Present( 0, 0 );

	WaitForFenceEvent();

	// 백버퍼 타겟 인덱스 변경
	m_iCurTargetIdx == 0 ? m_iCurTargetIdx = 1 : m_iCurTargetIdx = 0;

	// 상수버퍼 오프셋 초기화
	for ( size_t i = 0; i < m_vecCB.size(); ++i )
		m_vecCB[i]->Clear();
}

void CDevice::WaitForFenceEvent()
{
	const UINT64 fence = m_iFenceValue;
	m_pCmdQueue->Signal( m_pFence.Get(), fence );
	m_iFenceValue++;

	int a = m_pFence->GetCompletedValue();

	if ( a < fence )
	{
		m_pFence->SetEventOnCompletion( fence, m_hFenceEvent );
		WaitForSingleObject( m_hFenceEvent, INFINITE );
	}
}

void CDevice::CreateConstBuffer( const wstring& strName, size_t iSize, size_t iMaxSize,	CONST_REGISTER eRegister, bool bGlobal )
{
	CConstantBuffer* pCB = new CConstantBuffer;
	pCB->SetName( strName );
	pCB->Create( (UINT)iSize, (UINT)iMaxSize, eRegister );
	m_vecCB.push_back( pCB );

	if ( bGlobal )
		SetGlobalConstBufferToRegister( pCB, 0 );
}

void CDevice::SetConstBufferToRegister( CConstantBuffer * pCB, UINT iOffset )
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0번 슬롯이 상수버퍼 데이터
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_vecDummyDescriptor[m_iCurDummyIdx]->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCBVIncreSize * ( UINT )pCB->GetRegisterNum();

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += iOffset * m_iCBVIncreSize;

	m_pDevice->CopyDescriptors( 1, &hDescHandle, &iDestRange, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
}

void CDevice::SetGlobalConstBufferToRegister( CConstantBuffer * pCB, UINT iOffset )
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0번 슬롯이 상수 버퍼 데이터
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pInitDescriptor->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCBVIncreSize * ( UINT )pCB->GetRegisterNum();

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += iOffset * m_iCBVIncreSize;

	m_pDevice->CopyDescriptors( 1, &hDescHandle, &iDestRange,
		1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
}

void CDevice::SetTextrueToRegister( CTexture * pTexture, TEXTURE_REGISTER eRegister )
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0번 슬롯이 상수버퍼 데이터
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_vecDummyDescriptor[m_iCurDummyIdx]->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCBVIncreSize * ( UINT )eRegister;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = pTexture->GetSRV()->GetCPUDescriptorHandleForHeapStart();

	m_pDevice->CopyDescriptors( 1, &hDescHandle, &iDestRange, 
		1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
}

void CDevice::ClearDummyDescriptorHeap( UINT iDummyIndex )
{
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_vecDummyDescriptor[iDummyIndex]->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = m_pInitDescriptor->GetCPUDescriptorHandleForHeapStart();

	UINT iDestRange = ( UINT )TEXTURE_REGISTER::END;
	UINT iSrcRange = ( UINT )TEXTURE_REGISTER::END;

	m_pDevice->CopyDescriptors( 1, &hDescHandle, &iDestRange,
		1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
}

void CDevice::UpdateTable()
{
	ID3D12DescriptorHeap* pDescriptor = m_vecDummyDescriptor[m_iCurDummyIdx].Get();
	m_pCmdListGraphic->SetDescriptorHeaps( 1, &pDescriptor );

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = pDescriptor->GetGPUDescriptorHandleForHeapStart();
	m_pCmdListGraphic->SetGraphicsRootDescriptorTable( 0, gpuHandle );

	// 다음 더미 Descriptor Heap을 가리키게 인덱스를 증가시킨다.
	++m_iCurDummyIdx;

	// 다음 더미 Descriptor Heap을 초기화 한다. ( 전역 상수 버퍼는 남는다 )
	ClearDummyDescriptorHeap( m_iCurDummyIdx );
}

void CDevice::ExcuteResourceLoad()
{
	// 리소스 로딩 명령 닫기
	m_pCmdListRes->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* ppCommandLists[] = { m_pCmdListRes.Get() };
	m_pCmdQueue->ExecuteCommandLists( _countof( ppCommandLists ), ppCommandLists );

	WaitForFenceEvent();

	// 다시 활성화
	m_pCmdAllocRes->Reset();
	m_pCmdListRes->Reset( m_pCmdAllocRes.Get(), NULL );
}

void CDevice::CreateSwapChain()
{
	// 출력 윈도우를 지정하여, 화면 갱신역할을 담당
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferCount = 2; // dx12 는 버퍼 카운트는 2를 넣어준다

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 픽셀 포멧(픽셀당 4바이트
	tDesc.BufferDesc.Width = ( UINT )m_tResolution.fWidth;  // 버퍼의 해상도(윈도우 해상도랑 일치시켜놓음)
	tDesc.BufferDesc.Height = ( UINT )m_tResolution.fHeight;// 버퍼의 해상도(윈도우 해상도랑 일치시켜놓음)
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // 윈도우와 해상도 불일치시 화면 스케일링을 할것인지
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 픽셀의 스캔라인 순서
	tDesc.BufferDesc.RefreshRate.Numerator = 100; // 화면 갱신 비율
	tDesc.BufferDesc.RefreshRate.Denominator = 1;    // 화면 갱신 비율 

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 출력 타겟 용도로 버퍼를 만든다.
	tDesc.Flags = 0; // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH

	tDesc.OutputWindow = m_hWnd;	// 출력 윈도우
	tDesc.Windowed = m_bWindow;   // 창 모드 or 전체화면 모드
	tDesc.SampleDesc.Count = 1;		// 멀티 샘플 사용 안함
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림

	HRESULT hr = m_pFactory->CreateSwapChain( m_pCmdQueue.Get(), &tDesc, &m_pSwapChain );
}

void CDevice::CreateView()
{
	// RenderTargetViewHeap 의 메모리 사이즈
	m_iRTVHeapSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );

	// dx12 설명자힙 으로 RenderTargetView 생성
	// Descriptor Heap(설명자 힙) 이 Dx11 의 RTV, DSV, UAV, SRV 를 전부 대체

	// RenderTargetView 만들기	
	D3D12_DESCRIPTOR_HEAP_DESC tDesc = {};
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDesc.NumDescriptors = 2;
	tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDesc.NodeMask = 0;
	m_pDevice->CreateDescriptorHeap( &tDesc, IID_PPV_ARGS( &m_pRTV ) );

	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHeap = m_pRTV->GetCPUDescriptorHandleForHeapStart();

	// Create a RTV for each frame.
	for ( UINT i = 0; i < 2; i++ )
	{
		// 생성된 SwapChain 에서 버퍼를 가져옴
		m_pSwapChain->GetBuffer( i, IID_PPV_ARGS( &m_RenderTargets[i] ) );

		// 해당 버퍼로 RenderTarvetView 생성함
		m_pDevice->CreateRenderTargetView( m_RenderTargets[i].Get(), nullptr, hRTVHeap );
		hRTVHeap.ptr += m_iRTVHeapSize; // Offset 증가
	}

	// DepthStencilView 만들기
	// Create a Texture2D
	D3D12_RESOURCE_DESC textureDesc{};
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.Width = ( UINT )m_tResolution.fWidth;
	textureDesc.Height = ( UINT )m_tResolution.fHeight;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	//D3D12_CLEAR_VLAUE를 더 쉽게 초기화할 수 있는 구조체
	CD3DX12_CLEAR_VALUE depthOptimizedClearValue( DXGI_FORMAT_D32_FLOAT, 1.f, 0 );

	HRESULT hr = m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
		D3D12_HEAP_FLAG_NONE, &textureDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthOptimizedClearValue,
		IID_PPV_ARGS( &m_pDepthStencilTex ) );

	if ( FAILED( hr ) )
		assert( NULL );

	// DepthStencilView를 저장할 DescriptorHeap 생성
	tDesc = {};
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	tDesc.NumDescriptors = 1;
	tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDesc.NodeMask = 0;

	hr = m_pDevice->CreateDescriptorHeap( &tDesc, IID_PPV_ARGS( &m_pDSV ) );

	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSV->GetCPUDescriptorHandleForHeapStart();
	m_pDevice->CreateDepthStencilView( m_pDepthStencilTex.Get(), NULL, hDSVHandle );
}

void CDevice::CreateViewPort()
{	
	// DirectX 로 그려질 화면 크기를 설정한다.
	m_tVP = D3D12_VIEWPORT{ 0.f, 0.f, m_tResolution.fWidth, m_tResolution.fHeight, 0.f, 1.f };
	m_tScissorRect = D3D12_RECT{ 0, 0, ( LONG )m_tResolution.fWidth, ( LONG )m_tResolution.fHeight };
}

void CDevice::CreateRootSignature()
{
	m_iCBVIncreSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	// 슬롯 별 Descriptor Table을 작성한다.
	vector<D3D12_DESCRIPTOR_RANGE> vecRange;
	
	D3D12_ROOT_PARAMETER slotParam{};
	vecRange.clear();

	D3D12_DESCRIPTOR_RANGE range{};
	range.BaseShaderRegister = 0;
	range.NumDescriptors = 5;
	range.OffsetInDescriptorsFromTableStart = -1;
	range.RegisterSpace = 0;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	vecRange.push_back( range );

	range = {};
	range.BaseShaderRegister = 0;
	range.BaseShaderRegister = 13;
	range.OffsetInDescriptorsFromTableStart = 5;
	range.RegisterSpace = 0;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	vecRange.push_back( range );

	slotParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	slotParam.DescriptorTable.NumDescriptorRanges = vecRange.size();
	slotParam.DescriptorTable.pDescriptorRanges = &vecRange[0];

	CreateSamplerDesc();

	// 루트 서명
	D3D12_ROOT_SIGNATURE_DESC sigDesc = {};
	sigDesc.NumParameters = 1;
	sigDesc.pParameters = &slotParam;
	sigDesc.NumStaticSamplers = m_vecSamplerDesc.size();
	sigDesc.pStaticSamplers = &m_vecSamplerDesc[0];
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> pSignature;
	ComPtr<ID3DBlob> pError;
	HRESULT hr = D3D12SerializeRootSignature( &sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError );
	m_pDevice->CreateRootSignature( 0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS( &m_arrSig[( UINT )ROOT_SIG_TYPE::INPUT_ASSEM] ) );

	// 더미용 Descriptor Heap만들기
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};

	UINT iDescriptorNum = 0;
	for ( size_t i = 0; i < vecRange.size(); ++i )
	{
		iDescriptorNum += vecRange[i].NumDescriptors;
	}

	cbvHeapDesc.NumDescriptors = iDescriptorNum;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	for ( size_t i = 0; i < 512; ++i )
	{
		ComPtr<ID3D12DescriptorHeap> pDummyDescriptor;
		DEVICE->CreateDescriptorHeap( &cbvHeapDesc, IID_PPV_ARGS( &pDummyDescriptor ) );
		m_vecDummyDescriptor.push_back( pDummyDescriptor );
	}

	// 초기화용 Dummy Descriptor Heap 
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_pDevice->CreateDescriptorHeap( &cbvHeapDesc, IID_PPV_ARGS( &m_pInitDescriptor ) );
}

void CDevice::CreateSamplerDesc()
{
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_ANISOTROPIC;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;		// 바둑판
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	m_vecSamplerDesc.push_back( sampler );

	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;		// [0.0, 1.0] 범위를 벗어난 텍스처 좌표는 각각 0.0 또는 1.0의 텍스처 색상으로 설정됩니다.
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 1;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	m_vecSamplerDesc.push_back( sampler );
}

ComPtr<ID3D12GraphicsCommandList> CDevice::GetCmdList()
{
	return m_pCmdListGraphic;
}

ComPtr<ID3D12GraphicsCommandList> CDevice::GetCmdListRes()
{
	return m_pCmdListRes;
}

ComPtr<ID3D12Device> CDevice::GetDevice()
{
	return m_pDevice;
}

ComPtr<ID3D12RootSignature> CDevice::GetRootSignature( ROOT_SIG_TYPE eType )
{
	return m_arrSig[( UINT )eType];
}

CConstantBuffer * CDevice::GetConstBuffer( CONST_REGISTER eRegister )
{
	return m_vecCB[( UINT )eRegister];
}
