#include "Shader_Shadow.h"
#include "GraphicDevice.h"

USING(Engine)

CShader_Shadow::CShader_Shadow(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: CShader(pGraphicDevice, pCommandList)
{
}

CShader_Shadow::CShader_Shadow(const CShader_Shadow & rhs)
	: CShader(rhs)
{
	/*____________________________________________________________________
	[ 주의 ]
	- ID3D12DescriptorHeap 컴객체와 CUploadBuffer<T>는 복사 생성으로 생성 X.
	- 같은 셰이더 컴포넌트를 쓰는 객체들 렌더링 시, 마지막에 호출된 객체만 렌더링된다.
	- 복사 생성시, 새롭게 만들어줘야 객체들이 같이 렌더링된다.
	______________________________________________________________________*/


}

CShader_Shadow::~CShader_Shadow()
{
}

HRESULT CShader_Shadow::Ready_Shader()
{

	FAILED_CHECK_RETURN(Create_PipelineState(), E_FAIL);
	FAILED_CHECK_RETURN(Create_ConstantBufferView(), E_FAIL);
	return S_OK;
}

void CShader_Shadow::Begin_Shader()
{
	m_iCBMeshCount = 0;
	m_pCommandList->SetPipelineState(m_pPipelineState);
	m_pCommandList->SetGraphicsRootSignature(CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_SHADOWDEPTH));

}

void CShader_Shadow::End_Shader(_uint Texnum)
{

	m_iCBBoneCount = 0;
	m_iCBMeshCount = 0;
	return;

}

void CShader_Shadow::End_ShadowShader(bool Bone)
{

	UINT objCBByteSize = (sizeof(CB_SHADOW_INFO) + 255) & ~255;
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = m_pCB_ShadowInfo->Resource()->GetGPUVirtualAddress() + m_iCBMeshCount * objCBByteSize;
	m_pCommandList->SetGraphicsRootConstantBufferView(0, objCBAddress);

	if (Bone == true)
	{
		UINT objCBByteSize = (sizeof(CB_BONE_INFO) + 255) & ~255;
		D3D12_GPU_VIRTUAL_ADDRESS BoneCBAddress = m_pCB_BoneInfo->Resource()->GetGPUVirtualAddress() + m_iCBBoneCount * objCBByteSize;
		m_pCommandList->SetGraphicsRootConstantBufferView(1, BoneCBAddress);
		m_iCBBoneCount++;

	}

}
CUploadBuffer<CB_BONE_INFO>* CShader_Shadow::Get_UploadBuffer_BoneInfo()
{
	return m_pCB_BoneInfo;
}

CUploadBuffer<CB_SHADOW_INFO>* CShader_Shadow::Get_UploadBuffer_ShadowInfo()
{
	return m_pCB_ShadowInfo;
}


HRESULT CShader_Shadow::Create_DescriptorHeaps()
{
	return S_OK;
}

HRESULT CShader_Shadow::Create_ConstantBufferView()
{
	// 물체 n개의 상수 자료를 담을 상수 버퍼.
	m_pCB_ShadowInfo = new CUploadBuffer<CB_SHADOW_INFO>(m_pGraphicDevice, 100, true);
	m_pCB_BoneInfo = new CUploadBuffer<CB_BONE_INFO>(m_pGraphicDevice, 60, true);

	return S_OK;
}


HRESULT CShader_Shadow::Create_PipelineState()
{
	vector<D3D12_INPUT_ELEMENT_DESC> vecInputLayout;

	m_pVS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_ShadowDepth.hlsl", nullptr, "VS_MAIN", "vs_5_1");
	m_pPS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_ShadowDepth.hlsl", nullptr, "PS_MAIN", "ps_5_1");

	vecInputLayout =
	{

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
	ZeroMemory(&PipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	PipelineStateDesc.InputLayout = { vecInputLayout.data(), (_uint)vecInputLayout.size() };
	PipelineStateDesc.pRootSignature = CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_SHADOWDEPTH);
	PipelineStateDesc.VS = { reinterpret_cast<BYTE*>(m_pVS_ByteCode->GetBufferPointer()), m_pVS_ByteCode->GetBufferSize() };
	PipelineStateDesc.PS = { reinterpret_cast<BYTE*>(m_pPS_ByteCode->GetBufferPointer()), m_pPS_ByteCode->GetBufferSize() };
	PipelineStateDesc.RasterizerState = Create_RasterizerState();
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PipelineStateDesc.SampleDesc.Count = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? 4 : 1;
	PipelineStateDesc.SampleDesc.Quality = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? (CGraphicDevice::Get_Instance()->Get_MSAA4X_QualityLevels() - 1) : 0;
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ThrowIfFailed(m_pGraphicDevice->CreateGraphicsPipelineState(&PipelineStateDesc,
		IID_PPV_ARGS(&m_pPipelineState)));

	return S_OK;
}


D3D12_RASTERIZER_DESC CShader_Shadow::Create_RasterizerState()
{
	D3D12_RASTERIZER_DESC RasterizerDesc;

	// 레스터라이저 설정.
	ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
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

D3D12_BLEND_DESC CShader_Shadow::Create_BlendState()
{
	D3D12_BLEND_DESC BlendDesc;

	// 블렌드 설정.
	ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return BlendDesc;
}

D3D12_DEPTH_STENCIL_DESC CShader_Shadow::Create_DepthStencilState()
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

D3D12_INPUT_LAYOUT_DESC CShader_Shadow::Create_InputLayout()
{
	return D3D12_INPUT_LAYOUT_DESC();
}


CComponent * CShader_Shadow::Clone()
{
	return new CShader_Shadow(*this);
}

CShader_Shadow * CShader_Shadow::Create(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList)
{
	CShader_Shadow* pInstance = new CShader_Shadow(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Shader()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShader_Shadow::Free()
{
	CShader::Free();
	Engine::Safe_Delete(m_pCB_BoneInfo);	
	Engine::Safe_Delete(m_pCB_ShadowInfo);
	Engine::Safe_Delete(m_pCB_MatrixInfo);
	Engine::Safe_Delete(m_pCBV_DescriptorHeap);
}