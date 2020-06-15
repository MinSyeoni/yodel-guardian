#pragma once
#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	ID3D12Device*				Get_GraphicDevice()					{ return m_pGraphicDevice; }
	ID3D12CommandQueue*			Get_CommandQueue()					{ return m_pCommandQueue; }
	ID3D12CommandAllocator*		Get_CommandAllocator()				{ return m_pCommandAllocator; }
	ID3D12GraphicsCommandList*	Get_CommandList()					{ return m_pCommandList; }

	ID3D12GraphicsCommandList*	Get_CommandListThread() { return m_pCommandListThread; }

	const D3D12_VIEWPORT&		Get_Viewport()						{ return m_Viewport; }
	const D3D12_RECT&    Get_ScissorRect() { return m_ScissorRect; };

	const _bool&				Get_MSAA4X_Enable()					{ return m_bIsMSAA4X_Enable; }
	const _uint&				Get_MSAA4X_QualityLevels()			{ return m_uiMSAA4X_QualityLevels; }
	const _uint&				Get_CBV_SRV_UAV_DescriptorSize()	{ return m_uiCBV_SRV_UAV_DescriptorSize; }
public:
	void						Set_PipelineState(ID3D12PipelineState* pPipelineState) { m_pPipelineState = pPipelineState; }
public:
	HRESULT Ready_GraphicDevice(HWND hWnd, HANDLE hHandle, WINMODE eMode, const _uint& iWidth, const _uint& iHeight);
	HRESULT	Render_Begin(const _rgba& vRGBA);
	HRESULT Render_TextBegin();
	HRESULT Render_TextEnd();
	HRESULT	Render_End();

	CD3DX12_CPU_DESCRIPTOR_HANDLE Get_DepthBuffer_handle();
	HRESULT BackBufferSettingBegin();
	HRESULT BackBufferSettingEnd();

private:
	HRESULT	Create_GraphicDevice(const _uint& iWidth, const _uint& iHeight);
	HRESULT	Create_CommandQueueAndList();
	HRESULT	Create_SwapChain(HWND hWnd, WINMODE eMode, const _uint& iWidth, const _uint& iHeight);
	HRESULT Create_RtvAndDsvDescriptorHeaps();

	HRESULT Create_RootSig();
	HRESULT Create_ColorObjectRoot();
	HRESULT Create_TextureRoot();
	HRESULT Create_MeshRoot();
	HRESULT Create_LightRoot();
	HRESULT Create_BlendRoot();
	HRESULT Create_TerrainRoot();
	HRESULT Create_ShadowDepthRoot();
	HRESULT Create_DownSampleRoot();
	HRESULT Create_BlurRoot();
	HRESULT Create_DistortRoot();
	HRESULT Create_SSAORoot();
	HRESULT Create_EffectRoot();

	HRESULT OnResize(const _uint& iWidth, const _uint& iHeight);
public:
	ID3D12RootSignature* GetLootSig(_uint eType) { return m_arrSig[eType]; }

private:
	ID3D12RootSignature*	m_arrSig[(UINT)ROOT_SIG_TYPE::ENDSIG];

public:
	HRESULT	Wait_ForGpuComplete();	// CPU 와 GPU동기화 함수
	HRESULT Begin_ResetCmdList();
	HRESULT End_ResetCmdList();

	HRESULT Begin_ResetCmdListThread();
	HRESULT End_ResetCmdListThread();
private:
	void	Log_Adapters();
	void	Log_AdapterOutputs(IDXGIAdapter* adapter);
	void	Log_OutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

private:
	array<const CD3DX12_STATIC_SAMPLER_DESC, 1> GetStaticSamplers();


private:
	/*____________________________________________________________________
	[ Factory / SwapChain / Device ]
	IDXGIFactory	: IDXGISwapChain 인터페이스 생성과 디스플레이 어댑터(그래픽 카드) 열거에 쓰임.
	IDXGISwapChain	: 주로 디스플레이를 제어하기 위해 필요.
	ID3D12Device	: 주로 리소스를 생성하기 위해 필요.
	______________________________________________________________________*/
	IDXGIFactory4*		m_pFactory				 = nullptr;
	IDXGISwapChain*		m_pSwapChain			 = nullptr;
	ID3D12Device*		m_pGraphicDevice		 = nullptr;

	/*____________________________________________________________________
	[ CPU/GPU 동기화 ]
	ID3D12Fence				: GPU와 CPU의 동기화를 위한 수단으로 쓰이는 대표 인터페이스.
	UINT64 m_nFenceValue	: Fence의 값. 시간상의 특정 Fence지점을 식별하는 정수.
							후면버퍼 마다 현재의 펜스 값을 관리하기 위해 m_uiSwapChainBuffers개수 만큼 정의.
	______________________________________________________________________*/
	ID3D12Fence*	m_pFence		 = nullptr;
	UINT64			m_uiCurrentFence = 0;

	/*____________________________________________________________________
	[ 명령 대기열과 명령 목록 ]
	ID3D12CommandQueue			: 명령 대기열을 대표하는 인터페이스.
	ID3D12CommandAllocator		: 명령 할당자. 명령 목록에 추가된 명령들은 이 할당자의 메모리에 저장된다.
								  ExecuteCommandLists로 명령 목록을 실행하면, 명령 대기열은 그 할당자에 담긴 명령들을 참조한다.
	ID3D12GraphicsCommandList	: 실제 그래픽 작업을 위한 명령 목록을 대표하는 인터페이스.
	______________________________________________________________________*/
	ID3D12CommandQueue*			m_pCommandQueue		= nullptr;
	ID3D12CommandAllocator*		m_pCommandAllocator	= nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList		= nullptr;


	ID3D12CommandQueue*			m_pCommandQueueThread = nullptr;
	ID3D12CommandAllocator*		m_pCommandAllocatorThread = nullptr;
	ID3D12GraphicsCommandList*	m_pCommandListThread = nullptr;

	/*____________________________________________________________________
	[ 서술자 힙(Descriptor Heap) ]
	- 그리기 명령을 제출하기 전에, 먼저 해당 그리기 호출이 참조할 자원들을 렌더링 파이프라인에 묶어야 한다.
	- 실제로 파이프라인에 묶이는 것은 해당 자원을 참조하는 서술자(Descriptor) 객체이다.
	- 서술자는 자원을 GPU에게 서술해주는 경량의 자료구조라고 할 수 있다.
	- GPU는 자원 서술자를 통해서 자원의 실제 자료에 접근하며, 그 자료를 사용하는 데 필요한 정보 역시 서술자로부터 얻는다.
	- 서술자는 자원 자료를 지정하는 수단일 뿐만 아니라, 자원을 GPU에 서술하는 수단. 
	  (자원을 파이프라인의 어느 단계에 묶어야 하는지를 말해줌.)
	
	1. CBV(상수 버퍼), SRV(셰이더 자원), UAV(순서 없는 접근)을 서술.
	2. 표본 추출기 서술자는 텍스처 적용에 쓰이는 표본 추출기(smapler) 자원을 서술.
	3. RTV 서술자는 렌더 대상(Render Target)자원을 서술.
	4. DSV 서술자는 깊이, 스텐실(Depth/Stencil)자원을 서술.

	- 서술자들은 응용 프로그램의 초기화 시점에서 생성해야 한다.
	______________________________________________________________________*/
	static const _int		m_iSwapChainBufferCount = 2;
	_int					m_iCurrBackBuffer		= 0;



	ID3D12Resource*			m_ppSwapChainBuffer[m_iSwapChainBufferCount];
	ID3D12Resource*			m_pDepthStencilBuffer = nullptr;

	ID3D12DescriptorHeap*	m_pRTV_Heap = nullptr;
	ID3D12DescriptorHeap*	m_pDSV_Heap = nullptr;

	_uint					m_uiRTV_DescriptorSize			= 0;
	_uint					m_uiDSV_DescriptorSize			= 0;
	_uint					m_uiCBV_SRV_UAV_DescriptorSize	= 0;

	ID3D12PipelineState*	m_pPipelineState				= nullptr;		// 그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터.

	/*____________________________________________________________________
	[ 자원 상태 전이 ]
	ResourceBarrier는 드라이버에서 리소스가 저장된 메모리에 대한 다중 액세스를 
	동기화해야 하는 상황을 그래픽 드라이버에 알린다.
	______________________________________________________________________*/
	D3D12_RESOURCE_BARRIER	m_ResourceBarrier;
	DXGI_PRESENT_PARAMETERS	m_PresentParameters;

	/*____________________________________________________________________
	[ 뷰포트와 씨저 사각형 ]
	- 가위 사각형은 특정 픽셀들을 컬링하는 용도로 쓰인다.
	- 후면 버퍼를 기준으로 가위 직사각형을 정의, 설정하면, 
	  렌더링 시 가위 직사각형의 바깥에 있는 픽셀들은 후면 버퍼에 래스터와 되지 않는다.
	______________________________________________________________________*/
	D3D12_VIEWPORT	m_Viewport;		// 뷰포트.
	D3D12_RECT		m_ScissorRect;	// 씨저 사각형.


private:

	/*____________________________________________________________________
	4X MSAA 품질 수준 지원 점검.
	______________________________________________________________________*/
	_bool m_bIsMSAA4X_Enable		= false;
	_uint m_uiMSAA4X_QualityLevels	= 0;

	public:
		_matrix GetViewMatrix() { return m_matView; };
		_matrix GetProjMatrix() { return m_matProj; };

		void SetViewMatrix(_matrix matView) { m_matView = matView; }
		void SetProjMatrix(_matrix matProj) { m_matProj = matProj; }

		/*__________________________________________________________________________________________________________
			[ DirectX 11 GraphicDevice ]
			- D2D / TextFont Render.
			____________________________________________________________________________________________________________*/
private:

	HRESULT	Create_11On12GraphicDevice();

public:
    /*__________________________________________________________________________________________________________
    [ DirectX 11 ]
    ____________________________________________________________________________________________________________*/
    ID2D1DeviceContext2*		Get_D2DContext() { return m_pD2D_Context; }
    IDWriteFactory2*			Get_DWriteFactory() { return m_pDWriteFactory; }
private: 
	ID3D11Device*			m_p11Device = nullptr;
	ID3D11DeviceContext*	m_p11Context = nullptr;
	ID3D11On12Device*		m_p11On12Device = nullptr;

	ID2D1Factory3*			m_pD2D_Factory = nullptr;
	ID2D1Device2*			m_pD2D_Device = nullptr;
	ID2D1DeviceContext2*	m_pD2D_Context = nullptr;
	IDWriteFactory2*		m_pDWriteFactory = nullptr;

	ID3D11Resource*			m_pWrappedBackBuffers[m_iSwapChainBufferCount];
	ID2D1Bitmap1*			m_pD2DRenderTargets[m_iSwapChainBufferCount];



private:
	_matrix m_matView = INIT_MATRIX;
	_matrix m_matProj = INIT_MATRIX;

private:
	virtual void Free();
};

END