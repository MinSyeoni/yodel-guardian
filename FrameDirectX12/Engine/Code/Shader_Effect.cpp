#include "Shader_Effect.h"
#include "GraphicDevice.h"

USING(Engine)

CShader_Effect::CShader_Effect(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CShader(pGraphicDevice, pCommandList)
{
}

CShader_Effect::CShader_Effect(const CShader_Effect& rhs)
	: CShader(rhs)
{
	/*____________________________________________________________________
	[ 주의 ]
	- ID3D12DescriptorHeap 컴객체와 CUploadBuffer<T>는 복사 생성으로 생성 X.
	- 같은 셰이더 컴포넌트를 쓰는 객체들 렌더링 시, 마지막에 호출된 객체만 렌더링된다.
	- 복사 생성시, 새롭게 만들어줘야 객체들이 같이 렌더링된다.
	______________________________________________________________________*/


}

CShader_Effect::~CShader_Effect()
{
}

HRESULT CShader_Effect::Ready_Shader(TYPE eType)
{
	if (eType == ALPHABLENDTEX || eType ==FADEOUT)
		m_bIsAlpha = true;
	m_eType = eType;
	FAILED_CHECK_RETURN(Create_PipelineState(), E_FAIL);

	return S_OK;
}

void CShader_Effect::Begin_Shader()
{
	ID3D12DescriptorHeap* pDescriptorHeaps[] = { m_pCBV_DescriptorHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(pDescriptorHeaps), pDescriptorHeaps);
	m_pCommandList->SetPipelineState(m_pPipelineState);
	m_pCommandList->SetGraphicsRootSignature(CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_EFFECT));

}

void CShader_Effect::End_Shader(_uint Texnum, _uint uiOffset)
{

	UINT objCBByteSize = (sizeof(CB_MATRIX_INFO) + 255) & ~255;

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_pCB_MatrixInfo->Resource()->GetGPUVirtualAddress() + uiOffset * objCBByteSize;
	D3D12_GPU_VIRTUAL_ADDRESS objEffectAddress = m_pCB_EffectInfo->Resource()->GetGPUVirtualAddress() + uiOffset * m_pCB_EffectInfo->Get_ElementSize();


	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(Texnum, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
	m_pCommandList->SetGraphicsRootDescriptorTable(0, tex);
	m_pCommandList->SetGraphicsRootConstantBufferView(1, objCBAddress);
	m_pCommandList->SetGraphicsRootConstantBufferView(2, objEffectAddress);

}

void CShader_Effect::Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, _uint offset)
{
	CGraphicDevice::Get_Instance()->Begin_ResetCmdList();
	int iTexSize = (int)pVecTexture.size();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = iTexSize;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_pCBV_DescriptorHeap));

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCBV_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < iTexSize; i++)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = pVecTexture[i]->GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = pVecTexture[i]->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		DEVICE->CreateShaderResourceView(pVecTexture[i].Get(), &srvDesc, hDescriptor);
		hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
	}

	m_pCB_MatrixInfo = new CUploadBuffer<CB_MATRIX_INFO>(DEVICE, offset, true);
	m_pCB_EffectInfo = new CUploadBuffer<CB_EFFECT_INFO>(DEVICE, offset, true);


	CGraphicDevice::Get_Instance()->End_ResetCmdList();
}

HRESULT CShader_Effect::Create_DescriptorHeaps()
{
	//D3D12_DESCRIPTOR_HEAP_DESC CBV_HeapDesc;
	//CBV_HeapDesc.NumDescriptors = 1;
	//CBV_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//CBV_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//CBV_HeapDesc.NodeMask = 0;

	//ThrowIfFailed(m_pGraphicDevice->CreateDescriptorHeap(&CBV_HeapDesc,
	//	IID_PPV_ARGS(&m_pCBV_DescriptorHeap)));

	return S_OK;
}

HRESULT CShader_Effect::Create_ConstantBufferView()
{
	// 물체 n개의 상수 자료를 담을 상수 버퍼.
	//m_pCB_MatrixInfo = new CUploadBuffer<CB_MATRIX_INFO>(m_pGraphicDevice, 1, true);

	//_uint uiCB_ByteSize = INIT_CB_256(CB_MATRIX_INFO);

	return S_OK;
}


HRESULT CShader_Effect::Create_PipelineState()
{
	vector<D3D12_INPUT_ELEMENT_DESC> vecInputLayout;
	if (m_eType != FADEOUT)
	{
		m_pVS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Effect.hlsl", nullptr, "VS_MAIN", "vs_5_1");
		m_pPS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Effect.hlsl", nullptr, "PS_MAIN", "ps_5_1");
	}
	else
	{

		m_pVS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Effect.hlsl", nullptr, "VS_MAIN", "vs_5_1");
		m_pPS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Effect.hlsl", nullptr, "PS_FADEOUTMAIN", "ps_5_1");
	}


	vecInputLayout =
	{

	   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
	ZeroMemory(&PipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	PipelineStateDesc.InputLayout = { vecInputLayout.data(), (_uint)vecInputLayout.size() };
	PipelineStateDesc.pRootSignature = CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_EFFECT);
	PipelineStateDesc.VS = { reinterpret_cast<BYTE*>(m_pVS_ByteCode->GetBufferPointer()), m_pVS_ByteCode->GetBufferSize() };
	PipelineStateDesc.PS = { reinterpret_cast<BYTE*>(m_pPS_ByteCode->GetBufferPointer()), m_pPS_ByteCode->GetBufferSize() };
	PipelineStateDesc.RasterizerState = Create_RasterizerState();
	PipelineStateDesc.BlendState = Create_BlendState();
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.SampleDesc.Count = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? 4 : 1;
	PipelineStateDesc.SampleDesc.Quality = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? (CGraphicDevice::Get_Instance()->Get_MSAA4X_QualityLevels() - 1) : 0;
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ThrowIfFailed(m_pGraphicDevice->CreateGraphicsPipelineState(&PipelineStateDesc,
		IID_PPV_ARGS(&m_pPipelineState)));

	return S_OK;
}


D3D12_RASTERIZER_DESC CShader_Effect::Create_RasterizerState()
{
	D3D12_RASTERIZER_DESC RasterizerDesc;

	// 레스터라이저 설정.
	ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = m_bIsWire;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return RasterizerDesc;
}

D3D12_BLEND_DESC CShader_Effect::Create_BlendState()
{
	D3D12_BLEND_DESC BlendDesc;

	// 블렌드 설정.
	ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = m_bIsAlpha;
	BlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;;
	BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return BlendDesc;
}

D3D12_DEPTH_STENCIL_DESC CShader_Effect::Create_DepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;

	// Depth & Stencil 설정.
	ZeroMemory(&DepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	DepthStencilDesc.StencilEnable = FALSE;
	DepthStencilDesc.StencilReadMask = 0x00;
	DepthStencilDesc.StencilWriteMask = 0x00;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	DepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return DepthStencilDesc;
}

D3D12_INPUT_LAYOUT_DESC CShader_Effect::Create_InputLayout()
{
	return D3D12_INPUT_LAYOUT_DESC();
}


CComponent* CShader_Effect::Clone()
{
	return new CShader_Effect(*this);
}

CShader_Effect* CShader_Effect::Create(ID3D12Device* pGraphicDevice,
	ID3D12GraphicsCommandList* pCommandList, TYPE eType)
{
	CShader_Effect* pInstance = new CShader_Effect(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Shader(eType)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShader_Effect::Free()
{
	CShader::Free();

	Engine::Safe_Release(m_pCBV_DescriptorHeap);
	Engine::Safe_Delete(m_pCB_MatrixInfo);
	Engine::Safe_Delete(m_pCB_EffectInfo);
}