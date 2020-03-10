#include "Shader_ColorBuffer.h"
#include "GraphicDevice.h"

USING(Engine)

CShader_ColorBuffer::CShader_ColorBuffer(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: CShader(pGraphicDevice, pCommandList)
{
}

CShader_ColorBuffer::CShader_ColorBuffer(const CShader_ColorBuffer & rhs)
	: CShader(rhs)
	,m_eType(rhs.m_eType)
	,m_bIsWire(rhs.m_bIsWire)
{
	/*____________________________________________________________________
	[ 주의 ]
	- ID3D12DescriptorHeap 컴객체와 CUploadBuffer<T>는 복사 생성으로 생성 X.
	- 같은 셰이더 컴포넌트를 쓰는 객체들 렌더링 시, 마지막에 호출된 객체만 렌더링된다.
	- 복사 생성시, 새롭게 만들어줘야 객체들이 같이 렌더링된다.
	______________________________________________________________________*/
	Create_DescriptorHeaps();
	Create_ConstantBufferView();

}

CShader_ColorBuffer::~CShader_ColorBuffer()
{
}

HRESULT CShader_ColorBuffer::Ready_Shader(TYPE eType)
{

	m_eType = eType;
	if (m_eType == WIREFRAME)
		m_bIsWire = D3D12_FILL_MODE_WIREFRAME;

	FAILED_CHECK_RETURN(Create_DescriptorHeaps(), E_FAIL);
	FAILED_CHECK_RETURN(Create_ConstantBufferView(), E_FAIL);
	FAILED_CHECK_RETURN(Create_PipelineState(), E_FAIL);

	return S_OK;
}

void CShader_ColorBuffer::Begin_Shader()
{
	ID3D12DescriptorHeap* pDescriptorHeaps[] = { m_pCBV_DescriptorHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(pDescriptorHeaps), pDescriptorHeaps);

	m_pCommandList->SetPipelineState(m_pPipelineState);
	m_pCommandList->SetGraphicsRootSignature(CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_OBJECT));

}

void CShader_ColorBuffer::End_Shader(_uint Texnum )
{
	m_pCommandList->SetGraphicsRootConstantBufferView(0, m_pCB_MatrixInfo->Resource()->GetGPUVirtualAddress());
}

HRESULT CShader_ColorBuffer::Create_DescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC CBV_HeapDesc;
	CBV_HeapDesc.NumDescriptors = 1;
	CBV_HeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBV_HeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBV_HeapDesc.NodeMask		= 0;

	ThrowIfFailed(m_pGraphicDevice->CreateDescriptorHeap(&CBV_HeapDesc,
														 IID_PPV_ARGS(&m_pCBV_DescriptorHeap)));
	return S_OK;
}

HRESULT CShader_ColorBuffer::Create_ConstantBufferView()
{
	// 물체 n개의 상수 자료를 담을 상수 버퍼.
	m_pCB_MatrixInfo = new CUploadBuffer<CB_MATRIX_INFO>(m_pGraphicDevice, 1, true);

	return S_OK;
}


HRESULT CShader_ColorBuffer::Create_PipelineState()
{
	vector<D3D12_INPUT_ELEMENT_DESC> vecInputLayout;

	m_pVS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_ColorBuffer.hlsl", nullptr, "VS_MAIN", "vs_5_1");
	m_pPS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_ColorBuffer.hlsl", nullptr, "PS_MAIN", "ps_5_1");

	vecInputLayout =
	{
		{	
			"POSITION", 								// 시멘틱 이름.
			0, 											// 시멘티 인덱스.
			DXGI_FORMAT_R32G32B32_FLOAT,				// DXGI 포맷.
			0, 											// Input Slot.
			0,											// 정점 구조체의 시작 위치와, 이 정점 성분 시작 위치 사이의 거리.
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	// Input Slot Class.
			0 											// Instance Data Step Rate.
		},

		{ 
			"COLOR",									// 시멘틱 이름.
			0, 											// 시멘티 인덱스.
			DXGI_FORMAT_R32G32B32A32_FLOAT,				// DXGI 포맷.
			0, 											// Input Slot.
			12,											// 정점 구조체의 시작 위치와, 이 정점 성분 시작 위치 사이의 거리.
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // Input Slot Class.
			0 											// Instance Data Step Rate.
		}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
	ZeroMemory(&PipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	PipelineStateDesc.InputLayout			= { vecInputLayout.data(), (_uint)vecInputLayout.size() };
	PipelineStateDesc.pRootSignature		= CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_OBJECT);
	PipelineStateDesc.VS					= { reinterpret_cast<BYTE*>(m_pVS_ByteCode->GetBufferPointer()), m_pVS_ByteCode->GetBufferSize() };
	PipelineStateDesc.PS					= { reinterpret_cast<BYTE*>(m_pPS_ByteCode->GetBufferPointer()), m_pPS_ByteCode->GetBufferSize() };
	PipelineStateDesc.RasterizerState		= CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState			= CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState		= CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask			= UINT_MAX;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	PipelineStateDesc.NumRenderTargets		= 1;
	PipelineStateDesc.RTVFormats[0]			= DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.SampleDesc.Count		= CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? 4 : 1;
	PipelineStateDesc.SampleDesc.Quality	= CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? (CGraphicDevice::Get_Instance()->Get_MSAA4X_QualityLevels() - 1) : 0;
	PipelineStateDesc.DSVFormat				= DXGI_FORMAT_D24_UNORM_S8_UINT;

	ThrowIfFailed(m_pGraphicDevice->CreateGraphicsPipelineState(&PipelineStateDesc,
																IID_PPV_ARGS(&m_pPipelineState)));

	return S_OK;
}


D3D12_RASTERIZER_DESC CShader_ColorBuffer::Create_RasterizerState()
{
	D3D12_RASTERIZER_DESC RasterizerDesc;

	// 레스터라이저 설정.
	ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode					= m_bIsWire;
	RasterizerDesc.CullMode					= D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise	= FALSE;
	RasterizerDesc.DepthBias				= 0;
	RasterizerDesc.DepthBiasClamp			= 0.0f;
	RasterizerDesc.SlopeScaledDepthBias		= 0.0f;
	RasterizerDesc.DepthClipEnable			= TRUE;
	RasterizerDesc.MultisampleEnable		= FALSE;
	RasterizerDesc.AntialiasedLineEnable	= FALSE;
	RasterizerDesc.ForcedSampleCount		= 0;
	RasterizerDesc.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return RasterizerDesc;
}

D3D12_BLEND_DESC CShader_ColorBuffer::Create_BlendState()
{
	D3D12_BLEND_DESC BlendDesc;

	// 블렌드 설정.
	ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable					= FALSE;
	BlendDesc.IndependentBlendEnable				= FALSE;
	BlendDesc.RenderTarget[0].BlendEnable			= FALSE;
	BlendDesc.RenderTarget[0].LogicOpEnable			= FALSE;
	BlendDesc.RenderTarget[0].SrcBlend				= D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend				= D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp				= D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha		= D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha			= D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].LogicOp				= D3D12_LOGIC_OP_NOOP;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;

	return BlendDesc;
}

D3D12_DEPTH_STENCIL_DESC CShader_ColorBuffer::Create_DepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;

	// Depth & Stencil 설정.
	ZeroMemory(&DepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable					= TRUE;
	DepthStencilDesc.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc						= D3D12_COMPARISON_FUNC_LESS;

	DepthStencilDesc.StencilEnable					= FALSE;
	DepthStencilDesc.StencilReadMask				= 0x00;
	DepthStencilDesc.StencilWriteMask				= 0x00;

	DepthStencilDesc.FrontFace.StencilFailOp		= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilPassOp		= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC_NEVER;

	DepthStencilDesc.BackFace.StencilFailOp			= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilPassOp			= D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC_NEVER;

	return DepthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC CShader_ColorBuffer::Create_InputLayout()
{
	// Input Layout 설정.
	D3D12_INPUT_LAYOUT_DESC	InputLayoutDesc;
	ZeroMemory(&InputLayoutDesc, sizeof(D3D12_INPUT_LAYOUT_DESC));

	_uint						uiByteOffset		= 0;
	_uint						uiInputElementDescs	= 2;
	D3D12_INPUT_ELEMENT_DESC*	pInputElementDescs	= new D3D12_INPUT_ELEMENT_DESC[uiInputElementDescs];

	// 정점은 위치 벡터(POSITION)와 색상(COLOR)을 가진다.
	pInputElementDescs[0] = 
	{ 
		"POSITION",									// 시멘틱 이름.
		0,											// 시멘티 인덱스.
		DXGI_FORMAT_R32G32B32_FLOAT,				// DXGI 포맷.
		0,											// Input Slot.
		uiByteOffset,								// 정점 구조체의 시작 위치와, 이 정점 성분 시작 위치 사이의 거리.
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	// Input Slot Class.
		0											// Instance Data Step Rate.
	};

	uiByteOffset += sizeof(_vec3);					// (POSITION = _vec3 = float x 3).
	pInputElementDescs[1] = 
	{ 												
		"COLOR",									// 시멘틱 이름.
		0, 											// 시멘티 인덱스.
		DXGI_FORMAT_R32G32B32A32_FLOAT, 			// DXGI 포맷.
		0, 											// Input Slot.
		uiByteOffset, 								// 정점 구조체의 시작 위치와, 이 정점 성분 시작 위치 사이의 거리.
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	// Input Slot Class.
		0 											// Instance Data Step Rate.
	};

	InputLayoutDesc.pInputElementDescs	= pInputElementDescs;
	InputLayoutDesc.NumElements			= uiInputElementDescs;

	return InputLayoutDesc;
}

CComponent * CShader_ColorBuffer::Clone()
{
	return new CShader_ColorBuffer(*this);
}

CShader_ColorBuffer * CShader_ColorBuffer::Create(ID3D12Device * pGraphicDevice, 
												  ID3D12GraphicsCommandList * pCommandList,TYPE eType)
{
	CShader_ColorBuffer* pInstance = new CShader_ColorBuffer(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Shader(eType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShader_ColorBuffer::Free()
{
	CShader::Free();

	Engine::Safe_Release(m_pCBV_DescriptorHeap);
	Engine::Safe_Delete(m_pCB_MatrixInfo);
}