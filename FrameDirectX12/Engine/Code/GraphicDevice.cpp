#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)


CGraphicDevice::CGraphicDevice()
{
	for (int i = 0; i < m_iSwapChainBufferCount; i++)
	{
		m_ppSwapChainBuffer[i] = nullptr;
	}

	ZeroMemory(&m_Viewport, sizeof(D3D12_VIEWPORT));
	ZeroMemory(&m_ScissorRect, sizeof(D3D12_RECT));

	ZeroMemory(&m_ResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	m_PresentParameters.DirtyRectsCount = 0;
	m_PresentParameters.pDirtyRects		= nullptr;
	m_PresentParameters.pScrollRect		= nullptr;
	m_PresentParameters.pScrollOffset	= nullptr;
}


CGraphicDevice::~CGraphicDevice()
{
}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND hWnd,
											HANDLE hHandle,
											WINMODE eMode, 
											const _uint & iWidth, 
											const _uint & iHeight)
{
	FAILED_CHECK_RETURN(Create_GraphicDevice(iWidth, iHeight), E_FAIL);
	FAILED_CHECK_RETURN(Create_CommandQueueAndList(), E_FAIL);
	FAILED_CHECK_RETURN(Create_SwapChain(hWnd, eMode, iWidth, iHeight), E_FAIL);
	FAILED_CHECK_RETURN(Create_RtvAndDsvDescriptorHeaps(), E_FAIL);
	FAILED_CHECK_RETURN(OnResize(iWidth, iHeight), E_FAIL);
	FAILED_CHECK_RETURN(Create_RootSig(),E_FAIL);
	return S_OK;
}

HRESULT CGraphicDevice::Render_Begin(const _rgba& vRGBA)
{
	/*____________________________________________________________________
	- 명령 할당자와 명령 리스트 재설정.
	- 재설정은 GPU가 관련 명령 목록들을 모두 처리한 후에 일어난다.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pCommandAllocator->Reset(), E_FAIL);
	FAILED_CHECK_RETURN(m_pCommandList->Reset(m_pCommandAllocator, m_pPipelineState), E_FAIL);

	/*____________________________________________________________________
	- 뷰포트와 씨저 사각형을 설정한다. 
	- 명령 목록(CommandList)을 재설정할 때마다 이들도 재설정해야 한다.
	______________________________________________________________________*/
	m_pCommandList->RSSetViewports(1, &m_Viewport);
	m_pCommandList->RSSetScissorRects(1, &m_ScissorRect);


	/*____________________________________________________________________
	Indicate a state transition on the resource usage.
	- 자원 용도에 관련된 상태전이를 Direct3D에 통지한다.
	- 현재 렌더 타겟에 대한 프리젠트가 끝나기를 기다린다. 
	- 프리젠트가 끝나면 렌더 타겟 버퍼의 상태는 
	  프리젠트 상태(D3D12_RESOURCE_STATE_PRESENT)에서 렌더 타겟 상태(D3D12_RESOURCE_STATE_RENDER_TARGET)로 바뀔 것이다.
	______________________________________________________________________*/
	m_pCommandList->ResourceBarrier(1,
									&CD3DX12_RESOURCE_BARRIER::Transition(m_ppSwapChainBuffer[m_iCurrBackBuffer],
									D3D12_RESOURCE_STATE_PRESENT, 
									D3D12_RESOURCE_STATE_RENDER_TARGET));

	/*____________________________________________________________________
	 Clear the back buffer and depth buffer.
	- 현재의 렌더 타겟에 해당하는 서술자의 CPU 주소(핸들)를 계산.
	- 원하는 색상으로 렌더 타겟(뷰)을 지운다.
	______________________________________________________________________*/
	_float arrClearColor[4] = { vRGBA.x, vRGBA.y, vRGBA.z, vRGBA.w };
	m_pCommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(),
																		m_iCurrBackBuffer,
																		m_uiRTV_DescriptorSize),
										  arrClearColor,
										  0,
										  nullptr);

	/*____________________________________________________________________
	- 깊이-스텐실 서술자의 CPU 주소를 계산.
	- 원하는 값으로 깊이-스텐실(뷰)을 지운다.
	______________________________________________________________________*/
	m_pCommandList->ClearDepthStencilView(m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart(),
										  D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
										  1.0f,
										  0,
										  0, 
										  nullptr);

	/*____________________________________________________________________
	Specify the buffers we are going to render to.
	- 렌더링 결과가 기록될 렌더 타겟 버퍼들을 지정한다.
	- RTV(서술자)와 DSV(서술자)를 출력-병합 단계(OM)에 연결한다.
	- OMSetRenderTargets : 렌더링에 사용할 렌더 타겟과 깊이,스텐실 버퍼를 파이프라인에 묶는다.
	______________________________________________________________________*/
	m_pCommandList->OMSetRenderTargets(1, 
									   &CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(),
																	  m_iCurrBackBuffer,
																	  m_uiRTV_DescriptorSize), 
									   true,
									   &m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart());


	/* ---------------- 렌더링 코드는 여기에 추가될 것이다. ---------------- */


	return S_OK;
}

HRESULT CGraphicDevice::Render_End()
{
	/*____________________________________________________________________
	 Indicate a state transition on the resource usage.
	- 자원 용도에 관련된 상태 전이를 Direct3D에 통지한다.
	- 현재 렌더 타겟에 대한 렌더링이 끝나기를 기다린다. 
	- GPU가 렌더 타겟(버퍼)을 더 이상 사용하지 않으면 렌더 타겟의 상태는 
	  프리젠트 상태(D3D12_RESOURCE_STATE_PRESENT)로 바뀔 것이다.
	______________________________________________________________________*/
	m_pCommandList->ResourceBarrier(1, 
									&CD3DX12_RESOURCE_BARRIER::Transition(m_ppSwapChainBuffer[m_iCurrBackBuffer],
									D3D12_RESOURCE_STATE_RENDER_TARGET, 
									D3D12_RESOURCE_STATE_PRESENT));




	/*____________________________________________________________________
	- 명령들의 기록을 마친다.
	- ExecuteCommandLists로 명령 목록을 제출하기 전에 
	  반드시 이 메서드를 이용해서 명령 목록을 닫아야 한다.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pCommandList->Close(), E_FAIL);

	/*____________________________________________________________________
	- 명령 리스트를 명령 큐에 추가하여 실행한다.
	______________________________________________________________________*/
	ID3D12CommandList* ppCommandLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	/*____________________________________________________________________
	- 스왑체인을 프리젠트한다. (후면 버퍼와 전면 버퍼를 교환)
	- 프리젠트를 하면 현재 렌더 타겟(후면버퍼)의 내용이 
	  전면버퍼로 옮겨지고, 렌더 타겟 인덱스가 바뀔 것이다.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pSwapChain->Present(0, 0), E_FAIL);

	m_iCurrBackBuffer = (m_iCurrBackBuffer + 1) % m_iSwapChainBufferCount;

	/*____________________________________________________________________
	- GPU가 모든 명령 리스트를 실행할 때 까지 기다린다.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(Wait_ForGpuComplete(), E_FAIL);

	return S_OK;
}

HRESULT CGraphicDevice::Create_GraphicDevice(const _uint & iWidth, const _uint & iHeight)
{
#if defined(_DEBUG)
	ID3D12Debug* pDebugController = nullptr;
	D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&pDebugController);
	pDebugController->EnableDebugLayer();
#endif

	/*____________________________________________________________________
	DXGI 팩토리를 생성한다.
	______________________________________________________________________*/
	// FAILED_CHECK_RETURN(CreateDXGIFactory1(IID_PPV_ARGS(&m_pFactory)), E_FAIL);
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_pFactory)));

	/*____________________________________________________________________
	 Try to create hardware device.
	______________________________________________________________________*/
	HRESULT hResult = D3D12CreateDevice(nullptr,             // default adapter
										D3D_FEATURE_LEVEL_12_0,
										IID_PPV_ARGS(&m_pGraphicDevice));

	/*____________________________________________________________________
	 Fallback to WARP device.
	특성 레벨 12.0을 지원하는 하드웨어 디바이스를 생성할 수 없으면 WARP 디바이스를 생성.
	______________________________________________________________________*/
	if(FAILED(hResult))
	{
		IDXGIAdapter* pWarpAdapter;
		ThrowIfFailed(m_pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(pWarpAdapter,
										D3D_FEATURE_LEVEL_11_0,
										IID_PPV_ARGS(&m_pGraphicDevice)));
	}


	/*____________________________________________________________________
	펜스와 동기화를 위한 이벤트 객체를 생성한다. (이벤트 객체의 초기값을 FALSE이다).
	이벤트가 실행되면 (Signal)이벤트의 값을 자동적으로 FALSE가 되도록 생성한다.
	______________________________________________________________________*/
	ThrowIfFailed(m_pGraphicDevice->CreateFence(0,
												D3D12_FENCE_FLAG_NONE,
												IID_PPV_ARGS(&m_pFence)));

	m_uiRTV_DescriptorSize			= m_pGraphicDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_uiDSV_DescriptorSize			= m_pGraphicDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_uiCBV_SRV_UAV_DescriptorSize	= m_pGraphicDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	/*____________________________________________________________________
	4X MSAA 품질 수준 지원 점검.
	______________________________________________________________________*/
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MSAA_QualityLevels;
	ZeroMemory(&MSAA_QualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));

	MSAA_QualityLevels.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	MSAA_QualityLevels.SampleCount		= 4;	//Msaa4x 다중 샘플링
	MSAA_QualityLevels.Flags			= D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	MSAA_QualityLevels.NumQualityLevels = 0;

	// 디바이스가 지원하는 다중 샘플의 품질 수준을 확인.
	FAILED_CHECK_RETURN(m_pGraphicDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,		// 다중 표본화 기능.
										  &MSAA_QualityLevels,												// 기능 지원 정보가 설정될 구조체 포인터.
										  sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS)), E_FAIL);	// 구조체의 크기.

	m_uiMSAA4X_QualityLevels = MSAA_QualityLevels.NumQualityLevels;

	//다중 샘플의 품질 수준이 1보다 크면 다중 샘플링을 활성화.
	m_bIsMSAA4X_Enable = (m_uiMSAA4X_QualityLevels > 1) ? true : false;


#if defined(_DEBUG)
	Log_Adapters();
	Engine::Safe_Release(pDebugController);
#endif

	return S_OK;
}

HRESULT CGraphicDevice::Create_CommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(m_pGraphicDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&m_pCommandQueue)));

	ThrowIfFailed(m_pGraphicDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator)));

	ThrowIfFailed(m_pGraphicDevice->CreateCommandList(0,
													  D3D12_COMMAND_LIST_TYPE_DIRECT,
													  m_pCommandAllocator,	// Associated command allocator
													  nullptr,				// Initial PipelineStateObject
													  IID_PPV_ARGS(&m_pCommandList)));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	m_pCommandList->Close();


	ThrowIfFailed(m_pGraphicDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocatorThread)));

	ThrowIfFailed(m_pGraphicDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocatorThread,	// Associated command allocator
		nullptr,				// Initial PipelineStateObject
		IID_PPV_ARGS(&m_pCommandListThread)));

	m_pCommandListThread->Close();

	return S_OK;
}

HRESULT CGraphicDevice::Create_SwapChain(HWND hWnd, WINMODE eMode, const _uint& iWidth, const _uint& iHeight)
{
	// Release the previous swapchain we will be recreating.
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width						= iWidth;
    SwapChainDesc.BufferDesc.Height						= iHeight;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
    SwapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.SampleDesc.Count						= m_bIsMSAA4X_Enable ? 4 : 1;
    SwapChainDesc.SampleDesc.Quality					= m_bIsMSAA4X_Enable ? (m_uiMSAA4X_QualityLevels - 1) : 0;
    SwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount							= m_iSwapChainBufferCount;
    SwapChainDesc.OutputWindow							= hWnd;
    SwapChainDesc.Windowed								= eMode;
	SwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
    ThrowIfFailed(m_pFactory->CreateSwapChain(m_pCommandQueue,
											  &SwapChainDesc, 
											  &m_pSwapChain));

	return S_OK;
}

HRESULT CGraphicDevice::Create_RtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC RTV_HeapDesc;
	RTV_HeapDesc.NumDescriptors	= m_iSwapChainBufferCount;
	RTV_HeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTV_HeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTV_HeapDesc.NodeMask		= 0;

	ThrowIfFailed(m_pGraphicDevice->CreateDescriptorHeap(&RTV_HeapDesc, IID_PPV_ARGS(&m_pRTV_Heap)));


	D3D12_DESCRIPTOR_HEAP_DESC DSV_HeapDesc;
	DSV_HeapDesc.NumDescriptors	= 1;
	DSV_HeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSV_HeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSV_HeapDesc.NodeMask		= 0;
	ThrowIfFailed(m_pGraphicDevice->CreateDescriptorHeap(&DSV_HeapDesc, IID_PPV_ARGS(&m_pDSV_Heap)));

	return S_OK;
}

HRESULT CGraphicDevice::Create_RootSig()
{
	/*____________________________________________________________________
		 일반적으로 셰이더 프로그램은 특정 자원들(상수 버퍼, 텍스처, 샘플러) 등이 입력된다고 기대한다.
		루트 서명은 셰이더 프로그램이 기대하는 자원들을 정의한다.
		셰이더 프로그램은 본질적으로 하나의 함수이고, 셰이더에 입력되는 자원들은 함수의 매개변수들에 해당하므로
		루트 서명은 곧 함수 서명을 정의하는 수단이라 할 수 있다.
		______________________________________________________________________*/

		// 루트 매개변수는 테이블이거나, 루트 서술자 또는 루트 상수이다.
	CD3DX12_ROOT_PARAMETER RootParameter[1];

	// CBV 하나를 담는 서술자 테이블을 생성한다.
	CD3DX12_DESCRIPTOR_RANGE CBV_Table;
	CBV_Table.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	RootParameter[0].InitAsDescriptorTable(1, &CBV_Table);

	// 루트 서명은 루트 매개변수들의 배열이다.
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(1,
		RootParameter,
		0,		// Texture가 없으므로 0.
		nullptr,	// Texture가 없으므로 nullptr.
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// 상수 버퍼 하나로 구성된 서술자 구간을 가리키는, 슬롯 하나로 이루어진 루트 서명을 생성한다.
	ID3DBlob* pSignatureBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	FAILED_CHECK_RETURN(D3D12SerializeRootSignature(&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pSignatureBlob,
		&pErrorBlob), E_FAIL);
	if (nullptr != pErrorBlob)
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return E_FAIL;
	}

	ThrowIfFailed(m_pGraphicDevice->CreateRootSignature(0,
		pSignatureBlob->GetBufferPointer(),
		pSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_arrSig[(UINT)ROOT_SIG_TYPE::INPUT_OBJECT])));
	Engine::Safe_Release(pSignatureBlob);
	Engine::Safe_Release(pErrorBlob);

	Create_TextureRoot();

	return S_OK;
}

HRESULT CGraphicDevice::Create_TextureRoot()
{
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		1,  // number of descriptors
		0); // register t0

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[2];

	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);

	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(m_pGraphicDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&m_arrSig[(UINT)ROOT_SIG_TYPE::INPUT_TEXTURE])));

	return S_OK;
}

HRESULT CGraphicDevice::OnResize(const _uint& iWidth, const _uint& iHeight)
{
	assert(m_pGraphicDevice);
	assert(m_pSwapChain);
	assert(m_pCommandAllocator);

	// Flush before changing any resources.
	Wait_ForGpuComplete();

	ThrowIfFailed(m_pCommandList->Reset(m_pCommandAllocator, nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < m_iSwapChainBufferCount; ++i)
		m_ppSwapChainBuffer[i]	= nullptr;
	m_pDepthStencilBuffer		= nullptr;

	// Resize the swap chain.
	ThrowIfFailed(m_pSwapChain->ResizeBuffers(m_iSwapChainBufferCount,
											  iWidth, iHeight,
											  DXGI_FORMAT_R8G8B8A8_UNORM,
											  DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));


	m_iCurrBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE RTV_HeapHandle(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart());
	for (_uint i = 0; i < m_iSwapChainBufferCount; i++)
	{
		ThrowIfFailed(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppSwapChainBuffer[i])));

		m_pGraphicDevice->CreateRenderTargetView(m_ppSwapChainBuffer[i], nullptr, RTV_HeapHandle);

		RTV_HeapHandle.Offset(1, m_uiRTV_DescriptorSize);
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC DepthStencilDesc;
	DepthStencilDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	DepthStencilDesc.Alignment			= 0;
	DepthStencilDesc.Width				= iWidth;
	DepthStencilDesc.Height				= iHeight;
	DepthStencilDesc.DepthOrArraySize	= 1;
	DepthStencilDesc.MipLevels			= 1;
	DepthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.SampleDesc.Count	= m_bIsMSAA4X_Enable ? 4 : 1;
	DepthStencilDesc.SampleDesc.Quality = m_bIsMSAA4X_Enable ? (m_uiMSAA4X_QualityLevels - 1) : 0;
	DepthStencilDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	DepthStencilDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format						= DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth			= 1.0f;
	optClear.DepthStencil.Stencil		= 0;
	ThrowIfFailed(m_pGraphicDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
															D3D12_HEAP_FLAG_NONE,
															&DepthStencilDesc,
															D3D12_RESOURCE_STATE_COMMON,
															&optClear,
															IID_PPV_ARGS(&m_pDepthStencilBuffer)));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	m_pGraphicDevice->CreateDepthStencilView(m_pDepthStencilBuffer,
											 nullptr, 
											 m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart());

	// Transition the resource from its initial state to be used as a depth buffer.
	m_pCommandList->ResourceBarrier(1,
									&CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer,
									D3D12_RESOURCE_STATE_COMMON,
									D3D12_RESOURCE_STATE_DEPTH_WRITE));

	// Execute the resize commands.
	ThrowIfFailed(m_pCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	Wait_ForGpuComplete();

	// Update the viewport transform to cover the client area.
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width	= static_cast<_float>(iWidth);
	m_Viewport.Height	= static_cast<_float>(iHeight);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_ScissorRect		= { 0, 0, (LONG)iWidth, (LONG)iHeight };

	return S_OK;
}


HRESULT CGraphicDevice::Wait_ForGpuComplete()
{
	/*____________________________________________________________________
	 Advance the fence value to mark commands up to this fence point.
	- Cpu 펜스값 증가.
	- 현재 Fence 지점까지의 명령들을 표시하도록 울타리 값을 전진시킨다.
	______________________________________________________________________*/
	m_uiCurrentFence++;

	/*____________________________________________________________________
	 Add an instruction to the command queue to set a new fence point.  Because we 
	 are on the GPU timeline, the new fence point won't be set until the GPU finishes
	 processing all the commands prior to this Signal().

	- Cpu가 펜스의 값을 설정하는 명령을 명령 큐에 추가.
	- 새 Fence 지점을 설정하는 명령(Signal)을 명령 대기열에 추가한다.
	 지금 우리는 GPU 시간선 상에 있으므로, 새 Fence 지점은 
	 GPU가 Signal()명령까지의 모든 명령을 처리하기 전까지는 설정되지 않는다.
	______________________________________________________________________*/
	ThrowIfFailed(m_pCommandQueue->Signal(m_pFence, m_uiCurrentFence));

	/*____________________________________________________________________
	 Wait until the GPU has completed commands up to this fence point.
	- GPU가 이 Fence 지점까지의 명령들을 완료할 때 까지 기다린다.
	______________________________________________________________________*/
	if (m_pFence->GetCompletedValue() < m_uiCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		// GPU가 현재 Fence 지점에 도달했으면 이벤트를 발동한다.
		ThrowIfFailed(m_pFence->SetEventOnCompletion(m_uiCurrentFence, eventHandle));
		
		// GPU가 현재 울타리 지점에 도달했음을 뜻하는 이벤트를 기다린다.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	return S_OK;
}

HRESULT CGraphicDevice::Begin_ResetCmdList()
{
	FAILED_CHECK_RETURN(m_pCommandList->Reset(m_pCommandAllocator, nullptr), E_FAIL);

	return S_OK;
}

HRESULT CGraphicDevice::End_ResetCmdList()
{
	FAILED_CHECK_RETURN(m_pCommandList->Close(), E_FAIL);

	ID3D12CommandList* CommandLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

	Wait_ForGpuComplete();

	return S_OK;
}

HRESULT CGraphicDevice::Begin_ResetCmdListThread()
{
	FAILED_CHECK_RETURN(m_pCommandListThread->Reset(m_pCommandAllocatorThread, nullptr), E_FAIL);

	return S_OK;
}

HRESULT CGraphicDevice::End_ResetCmdListThread()
{
	FAILED_CHECK_RETURN(m_pCommandListThread->Close(), E_FAIL);

	ID3D12CommandList* CommandLists[] = { m_pCommandListThread };
	m_pCommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

	Wait_ForGpuComplete();

	return S_OK;
}


void CGraphicDevice::Log_Adapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (m_pFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		Log_AdapterOutputs(adapterList[i]);
		Engine::Safe_Release(adapterList[i]);
	}
}

void CGraphicDevice::Log_AdapterOutputs(IDXGIAdapter* pAdapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (pAdapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		Log_OutputDisplayModes(output, DXGI_FORMAT_R8G8B8A8_UNORM);

		Engine::Safe_Release(output);

		++i;
	}
}

void CGraphicDevice::Log_OutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

array<const CD3DX12_STATIC_SAMPLER_DESC, 6> CGraphicDevice::GetStaticSamplers()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}


void CGraphicDevice::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy GrpahicDevice");
#endif

	for (int i = 0; i < m_iSwapChainBufferCount; i++)
		Engine::Safe_Release(m_ppSwapChainBuffer[i]);

	Engine::Safe_Release(m_pDepthStencilBuffer);

	Engine::Safe_Release(m_pRTV_Heap);
	Engine::Safe_Release(m_pDSV_Heap);

	Engine::Safe_Release(m_pCommandAllocator);
	Engine::Safe_Release(m_pCommandQueue);
	Engine::Safe_Release(m_pCommandList);
	Engine::Safe_Release(m_pFence);

	m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	Engine::Safe_Release(m_pSwapChain);

	Engine::Safe_Release(m_pGraphicDevice);
	Engine::Safe_Release(m_pFactory);
}
