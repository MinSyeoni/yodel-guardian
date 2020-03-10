#include "LightTarget.h"
#include "GraphicDevice.h"
CLightTarget::CLightTarget(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:m_pGraphicDev(pGraphicDevice), m_pCommandLst(pCommandList)
{
}

HRESULT CLightTarget::Ready_Target()
{

	ComPtr<ID3D12Resource> pTex = nullptr;
	//Shade¿ë
	m_veCLightTargetTexture.push_back(pTex);
	//Specular¿ë
	m_veCLightTargetTexture.push_back(pTex);

	m_ClearColor[0] = 0.f;
	m_ClearColor[1] = 0.f;
	m_ClearColor[2] = 0.f;
	m_ClearColor[3] = 1.f;

	m_SpecColor[0] = 0.f;
	m_SpecColor[1] = 0.f;
	m_SpecColor[2] = 0.f;
	m_SpecColor[3] = 0.f;

	m_uiWidth = WINSIZEX;
	m_uiHeight = WINSIZEY;

	m_Viewport = { 0.0f,0.0f,(float)m_uiWidth,(float)m_uiHeight,0.0f,1.0f };
	m_ScissorRect = { 0,0, (int)m_uiWidth,(int)m_uiHeight };
	m_uiRTV_DescriptorSize = m_pGraphicDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	if (FAILED(BuildDescriptors()))
		return E_FAIL;
	if (FAILED(BuildResource()))
		return E_FAIL;


	m_pBuffer = CRcTex::Create(m_pGraphicDev, m_pCommandLst);

	m_pShader = CShader_DefaultTex::Create(m_pGraphicDev, m_pCommandLst);

	m_pShader->Set_Shader_Texture(m_veCLightTargetTexture,(_uint)m_veCLightTargetTexture.size());

	return S_OK;
}


HRESULT CLightTarget::SetUp_OnGraphicDev()
{
	CGraphicDevice::Get_Instance()->BackBufferSettingBegin();

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_veCLightTargetTexture[0].Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_veCLightTargetTexture[1].Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_RENDER_TARGET));


	m_pCommandLst->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(), 0, m_uiRTV_DescriptorSize), m_ClearColor, 0, nullptr);
	m_pCommandLst->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(), 1, m_uiRTV_DescriptorSize), m_SpecColor, 0, nullptr);

	m_pCommandLst->OMSetRenderTargets(2, &CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart()), true,nullptr);


	return S_OK;
}

HRESULT CLightTarget::Release_OnGraphicDev()
{
	CGraphicDevice::Get_Instance()->BackBufferSettingEnd();


	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_veCLightTargetTexture[0].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	m_pCommandLst->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_veCLightTargetTexture[1].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	return S_OK;
}

HRESULT CLightTarget::BuildResource()
{
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
	texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;


	D3D12_CLEAR_VALUE optClear;
	optClear.Color[0] = m_ClearColor[0];
	optClear.Color[1] = m_ClearColor[1];
	optClear.Color[2] = m_ClearColor[2];
	optClear.Color[3] = m_ClearColor[3];
	optClear.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	for (int i = 0; i < 2; ++i)
	{
		if (i == 1)
		{
			optClear.Color[0] = m_SpecColor[0];
			optClear.Color[1] = m_SpecColor[1];
			optClear.Color[2] = m_SpecColor[2];
			optClear.Color[3] = m_SpecColor[3];
		}


		ThrowIfFailed(m_pGraphicDev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&optClear,
			IID_PPV_ARGS(&m_veCLightTargetTexture[i])));

	}

	D3D12_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Texture2D.MipSlice = 0;
	desc.Texture2D.PlaneSlice = 0;

	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	for (int i = 0; i < 2; ++i)
	{
		m_pGraphicDev->CreateRenderTargetView(m_veCLightTargetTexture[i].Get(), &desc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTV_Heap->GetCPUDescriptorHandleForHeapStart(),i,m_uiRTV_DescriptorSize));
	}


	return S_OK;
}

void CLightTarget::Render_RenderTarget()
{

	for (int i = 0; i < 2; ++i)
	{
		SetUp_ConstateTable(i);

		m_pShader->Begin_Shader();
		m_pBuffer->Begin_Buffer();

		m_pShader->End_Shader(i,i);
		m_pBuffer->End_Buffer();

		m_pBuffer->Render_Buffer();
	}
}

void CLightTarget::SetUp_ConstateTable(const int offset)
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;
	_matrix matWorld = INIT_MATRIX;

	matProj = XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0, 1);

	matWorld._11 = 80.f;
	matWorld._22 = 60.f;
	matWorld._33 = 1.f;
	matWorld._41 = (80+160 * offset) - WINSIZEX * 0.5f;
	matWorld._42 = (-180) + WINSIZEY * 0.5f;



	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));


	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	m_pShader->Get_UploadBuffer_MatrixInfo()->CopyData(offset, tCB_MatrixInfo);


}

HRESULT CLightTarget::BuildDescriptors()
{
	D3D12_DESCRIPTOR_HEAP_DESC RTV_HeapDesc;
	RTV_HeapDesc.NumDescriptors = 3;
	RTV_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTV_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTV_HeapDesc.NodeMask = 0;

	ThrowIfFailed(m_pGraphicDev->CreateDescriptorHeap(&RTV_HeapDesc, IID_PPV_ARGS(&m_pRTV_Heap)));

	return S_OK;
}

void CLightTarget::OnResize(_uint newWidth, _uint newHeight)
{
}

CLightTarget * CLightTarget::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CLightTarget* pInstance = new CLightTarget(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Target()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLightTarget::Free()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pBuffer);
	Safe_Release(m_pRTV_Heap);
}
