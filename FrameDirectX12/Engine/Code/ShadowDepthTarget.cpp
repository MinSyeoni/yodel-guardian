#include "ShadowDepthTarget.h"
#include "GraphicDevice.h"
#include "ComponentMgr.h"
CShadowDepthTarget::CShadowDepthTarget(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:m_pGraphicDev(pGraphicDevice), m_pCommandLst(pCommandList)
{
}

HRESULT CShadowDepthTarget::Ready_Target()
{

	ComPtr<ID3D12Resource> pTex = nullptr;
	m_vecDepthTarget.push_back(pTex);

	m_ClearColor[0] = 0.f;
	m_ClearColor[1] = 0.f;
	m_ClearColor[2] = 0.f;
	m_ClearColor[3] = 1.f;

	m_uiWidth = SHADOWDEPTHSIZEX;
	m_uiHeight = SHADOWDEPTHSIZEY;

	m_Viewport = { 0.0f,0.0f,SHADOWDEPTHSIZEX,SHADOWDEPTHSIZEY,0.0f,1.0f };
	m_ScissorRect = { 0,0, SHADOWDEPTHSIZEX,SHADOWDEPTHSIZEY };
	m_uiRTV_DescriptorSize = m_pGraphicDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	if (FAILED(BuildDescriptors()))
		return E_FAIL;
	if (FAILED(BuildResource()))
		return E_FAIL;


	m_pBuffer = CRcTex::Create(m_pGraphicDev, m_pCommandLst);

	m_pShader = CShader_DefaultTex::Create(m_pGraphicDev, m_pCommandLst);

	m_pShader->Set_Shader_Texture(m_vecDepthTarget,(_uint)m_vecDepthTarget.size());


	return S_OK;
}


HRESULT CShadowDepthTarget::SetUp_OnGraphicDev()
{
	CGraphicDevice::Get_Instance()->BackBufferSettingBegin();

	// Transition the resource from its initial state to be used as a depth buffer.

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_vecDepthTarget[0].Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_RENDER_TARGET));


	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer,
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE));

	m_pCommandLst->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(), 0, m_uiRTV_DescriptorSize), m_ClearColor, 0, nullptr);

	m_pCommandLst->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart()), true, &m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart());
	m_pCommandLst->RSSetViewports(1, &m_Viewport);
	m_pCommandLst->RSSetScissorRects(1, &m_ScissorRect);

	return S_OK;
}

HRESULT CShadowDepthTarget::Release_OnGraphicDev()
{

	m_pCommandLst->ClearDepthStencilView(m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr);

	CGraphicDevice::Get_Instance()->BackBufferSettingEnd();

	m_pCommandLst->RSSetViewports(1, &CGraphicDevice::Get_Instance()->Get_Viewport());
	m_pCommandLst->RSSetScissorRects(1, &CGraphicDevice::Get_Instance()->Get_ScissorRect());

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_vecDepthTarget[0].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			D3D12_RESOURCE_STATE_COMMON));
	return S_OK;
}

HRESULT CShadowDepthTarget::BuildResource()
{

	CGraphicDevice::Get_Instance()->Begin_ResetCmdList();

	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = m_uiWidth;
	texDesc.Height = m_uiHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	D3D12_CLEAR_VALUE optClear;
	optClear.Color[0] = m_ClearColor[0];
	optClear.Color[1] = m_ClearColor[1];
	optClear.Color[2] = m_ClearColor[2];
	optClear.Color[3] = m_ClearColor[3];
	optClear.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	ThrowIfFailed(m_pGraphicDev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&optClear,
		IID_PPV_ARGS(&m_vecDepthTarget[0])));



	D3D12_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Texture2D.MipSlice = 0;
	desc.Texture2D.PlaneSlice = 0;

	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_pGraphicDev->CreateRenderTargetView(m_vecDepthTarget[0].Get(), &desc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart()));

//Depth
	D3D12_RESOURCE_DESC DepthStencilDesc;
	DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	DepthStencilDesc.Alignment = 0;
	DepthStencilDesc.Width = m_uiWidth;
	DepthStencilDesc.Height = m_uiHeight;
	DepthStencilDesc.DepthOrArraySize = 1;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	DepthStencilDesc.SampleDesc.Count = 1;
	DepthStencilDesc.SampleDesc.Quality = 0;

	D3D12_CLEAR_VALUE optClear2;
	optClear2.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear2.DepthStencil.Depth = 1.0f;
	optClear2.DepthStencil.Stencil = 0;
	ThrowIfFailed(m_pGraphicDev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&DepthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear2,
		IID_PPV_ARGS(&m_pDepthStencilBuffer)));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	m_pGraphicDev->CreateDepthStencilView(m_pDepthStencilBuffer,
		nullptr,
		m_pDSV_Heap->GetCPUDescriptorHandleForHeapStart());


	CGraphicDevice::Get_Instance()->End_ResetCmdList();
	return S_OK;
}

void CShadowDepthTarget::Render_RenderTarget()
{

	SetUp_ConstateTable();

	m_pShader->Begin_Shader();
	m_pBuffer->Begin_Buffer();

	m_pShader->End_Shader();
	m_pBuffer->End_Buffer();

	m_pBuffer->Render_Buffer();
}

void CShadowDepthTarget::SetUp_ConstateTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;
	_matrix matWorld = INIT_MATRIX;

	matProj = XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0, 1);

	matWorld._11 = 80.f;
	matWorld._22 = 60.f;
	matWorld._33 = 1.f;
	matWorld._41 = (80) - WINSIZEX * 0.5f;
	matWorld._42 = (-315) + WINSIZEY * 0.5f;



	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));


	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	m_pShader->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);


}

HRESULT CShadowDepthTarget::BuildDescriptors()
{
	D3D12_DESCRIPTOR_HEAP_DESC RTV_HeapDesc;
	RTV_HeapDesc.NumDescriptors = 1;
	RTV_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTV_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTV_HeapDesc.NodeMask = 0;

	ThrowIfFailed(m_pGraphicDev->CreateDescriptorHeap(&RTV_HeapDesc, IID_PPV_ARGS(&m_pRTV_Heap)));


	D3D12_DESCRIPTOR_HEAP_DESC DSV_HeapDesc;
	DSV_HeapDesc.NumDescriptors = 1;
	DSV_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSV_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSV_HeapDesc.NodeMask = 0;
	ThrowIfFailed(m_pGraphicDev->CreateDescriptorHeap(&DSV_HeapDesc, IID_PPV_ARGS(&m_pDSV_Heap)));
	return S_OK;
}

void CShadowDepthTarget::OnResize(_uint newWidth, _uint newHeight)
{
}

CShadowDepthTarget * CShadowDepthTarget::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CShadowDepthTarget* pInstance = new CShadowDepthTarget(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Target()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShadowDepthTarget::Free()
{
	Safe_Release(m_pDepthStencilBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRTV_Heap);
	Safe_Release(m_pDSV_Heap);
}
