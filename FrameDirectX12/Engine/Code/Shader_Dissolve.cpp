#include "Shader_Dissolve.h"
#include "GraphicDevice.h"

USING(Engine)

CShader_Dissolve::CShader_Dissolve(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
    : CShader(pGraphicDevice, pCommandList)
{
}

CShader_Dissolve::CShader_Dissolve(const CShader_Dissolve& rhs)
    : CShader(rhs)
    , m_eFileMode(rhs.m_eFileMode)
    , m_bIsZwrite(rhs.m_bIsZwrite)
    , m_bIsAlphaBlend(rhs.m_bIsAlphaBlend)
{
    /*____________________________________________________________________
    [ 주의 ]
    - ID3D12DescriptorHeap 컴객체와 CUploadBuffer<T>는 복사 생성으로 생성 X.
    - 같은 셰이더 컴포넌트를 쓰는 객체들 렌더링 시, 마지막에 호출된 객체만 렌더링된다.
    - 복사 생성시, 새롭게 만들어줘야 객체들이 같이 렌더링된다.
    ______________________________________________________________________*/


}

HRESULT CShader_Dissolve::Ready_Shader()
{
    FAILED_CHECK_RETURN(Create_PipelineState(), E_FAIL);

    return S_OK;
}

void CShader_Dissolve::Begin_Shader()
{
    ID3D12DescriptorHeap* pDescriptorHeaps[] = { m_pCBV_DescriptorHeap };
    m_pCommandList->SetDescriptorHeaps(_countof(pDescriptorHeaps), pDescriptorHeaps);
    m_pCommandList->SetPipelineState(m_pPipelineState);
    m_pCommandList->SetGraphicsRootSignature(CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_DISSOLVE));

}

void CShader_Dissolve::End_Shader(_uint Texnum, _int boneIndex)
{
    int TexCount = Texnum;
    if (Texnum > m_vecTextureType.size() - 1)
        Texnum = (_uint)m_vecTextureType.size() - 1;

    CB_TEXTURE_INFO pTexInfo;
    ZeroMemory(&pTexInfo, sizeof(CB_TEXTURE_INFO));
    if (m_vecTextureType[Texnum].m_iSpecular != 999)
        pTexInfo.vecTexInfo.x = 1.f;
    if (m_vecTextureType[Texnum].m_iEmissive != 999)
        pTexInfo.vecTexInfo.y = 1.f;

    m_pCB_TextureInfo->CopyData(Texnum, pTexInfo);


    CD3DX12_GPU_DESCRIPTOR_HANDLE tex(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    tex.Offset(Texnum, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
    m_pCommandList->SetGraphicsRootDescriptorTable(0, tex);

    if (m_vecTextureType[Texnum].m_iNormal != 999)
    {
        CD3DX12_GPU_DESCRIPTOR_HANDLE Normal(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        Normal.Offset(m_vecTextureType[Texnum].m_iNormal, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        m_pCommandList->SetGraphicsRootDescriptorTable(1, Normal);
    }
    if (m_vecTextureType[Texnum].m_iSpecular != 999)
    {
        CD3DX12_GPU_DESCRIPTOR_HANDLE Spec(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        Spec.Offset(m_vecTextureType[Texnum].m_iSpecular, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        m_pCommandList->SetGraphicsRootDescriptorTable(2, Spec);
    }
    if (m_vecTextureType[Texnum].m_iEmissive != 999)
    {
        CD3DX12_GPU_DESCRIPTOR_HANDLE Emis(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        Emis.Offset(m_vecTextureType[Texnum].m_iEmissive, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        m_pCommandList->SetGraphicsRootDescriptorTable(3, Emis);
    }

    CD3DX12_GPU_DESCRIPTOR_HANDLE Diss(m_pCBV_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    Diss.Offset(m_iDissolveOffset, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
    m_pCommandList->SetGraphicsRootDescriptorTable(4, Diss);

    m_pCommandList->SetGraphicsRootConstantBufferView(5, m_pCB_MatrixInfo->Resource()->GetGPUVirtualAddress());
    m_pCommandList->SetGraphicsRootConstantBufferView(6, m_pCB_BoneInfo->Resource()->GetGPUVirtualAddress() + (m_pCB_BoneInfo->Get_ElementSize() * boneIndex));
    m_pCommandList->SetGraphicsRootConstantBufferView(7, m_pCB_TextureInfo->Resource()->GetGPUVirtualAddress() + (m_pCB_TextureInfo->Get_ElementSize() * TexCount));
}

void CShader_Dissolve::Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, vector< ComPtr<ID3D12Resource>> pNormalTexture, vector< ComPtr<ID3D12Resource>> pSpecularTexture, vector< ComPtr<ID3D12Resource>> pEmissiveTexture,
                                          vector< ComPtr<ID3D12Resource>> pVecDissolveTexture)
{

    m_vecTextureType.resize(pVecTexture.size());

    CGraphicDevice::Get_Instance()->Begin_ResetCmdList();;
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = (_uint)(pVecTexture.size() * 4);
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_pCBV_DescriptorHeap));

    CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCBV_DescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    _int OffSet = 0;
    for (int i = 0; i < pVecTexture.size(); i++)
    {
        m_vecTextureType[i].m_iAlbedo = OffSet;

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = pVecTexture[i]->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = pVecTexture[i]->GetDesc().MipLevels;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        DEVICE->CreateShaderResourceView(pVecTexture[i].Get(), &srvDesc, hDescriptor);
        hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        OffSet++;
    }
    for (int i = 0; i < pNormalTexture.size(); ++i)
    {
        if (pNormalTexture[i] == nullptr)
            continue;
        m_vecTextureType[i].m_iNormal = OffSet;
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = pNormalTexture[i]->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = pNormalTexture[i]->GetDesc().MipLevels;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        DEVICE->CreateShaderResourceView(pNormalTexture[i].Get(), &srvDesc, hDescriptor);
        hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        OffSet++;
    }
    for (int i = 0; i < pSpecularTexture.size(); ++i)
    {
        if (pSpecularTexture[i] == nullptr)
            continue;

        m_vecTextureType[i].m_iSpecular = OffSet;
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = pSpecularTexture[i]->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = pSpecularTexture[i]->GetDesc().MipLevels;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        DEVICE->CreateShaderResourceView(pSpecularTexture[i].Get(), &srvDesc, hDescriptor);
        hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        OffSet++;
    }
    for (int i = 0; i < pEmissiveTexture.size(); ++i)
    {
        if (pEmissiveTexture[i] == nullptr)
            continue;

        m_vecTextureType[i].m_iEmissive = OffSet;
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = pEmissiveTexture[i]->GetDesc().Format;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = pEmissiveTexture[i]->GetDesc().MipLevels;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        DEVICE->CreateShaderResourceView(pEmissiveTexture[i].Get(), &srvDesc, hDescriptor);
        hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
        OffSet++;

    } 
    
    // 디졸브
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = pVecDissolveTexture[0]->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = pVecDissolveTexture[0]->GetDesc().MipLevels;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
    DEVICE->CreateShaderResourceView(pVecDissolveTexture[0].Get(), &srvDesc, hDescriptor);
    hDescriptor.Offset(1, CGraphicDevice::Get_Instance()->Get_CBV_SRV_UAV_DescriptorSize());
    m_iDissolveOffset = OffSet;

    m_pCB_MatrixInfo = new CUploadBuffer<CB_MATRIX_INFO>(DEVICE, 2, true);

    m_pCB_BoneInfo = new CUploadBuffer<CB_BONE_INFO>(DEVICE, 10, true);

    m_pCB_TextureInfo = new CUploadBuffer<CB_TEXTURE_INFO>(DEVICE, 15, true);

    CGraphicDevice::Get_Instance()->End_ResetCmdList();
}



HRESULT CShader_Dissolve::Create_DescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC CBV_HeapDesc;
    CBV_HeapDesc.NumDescriptors = 2;
    CBV_HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBV_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBV_HeapDesc.NodeMask = 0;

    ThrowIfFailed(m_pGraphicDevice->CreateDescriptorHeap(&CBV_HeapDesc,
        IID_PPV_ARGS(&m_pCBV_DescriptorHeap)));

    return S_OK;
}

HRESULT CShader_Dissolve::Create_ConstantBufferView()
{



    return S_OK;
}


HRESULT CShader_Dissolve::Create_PipelineState()
{
    vector<D3D12_INPUT_ELEMENT_DESC> vecInputLayout;

    m_pVS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Dissolve.hlsl", nullptr, "VS_MAIN", "vs_5_1");
    m_pPS_ByteCode = Compile_Shader(L"../../Bin/Shader/Shader_Dissolve.hlsl", nullptr, "PS_MAIN", "ps_5_1");

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
    PipelineStateDesc.pRootSignature = CGraphicDevice::Get_Instance()->GetLootSig((_uint)ROOT_SIG_TYPE::INPUT_DISSOLVE);
    PipelineStateDesc.VS = { reinterpret_cast<BYTE*>(m_pVS_ByteCode->GetBufferPointer()), m_pVS_ByteCode->GetBufferSize() };
    PipelineStateDesc.PS = { reinterpret_cast<BYTE*>(m_pPS_ByteCode->GetBufferPointer()), m_pPS_ByteCode->GetBufferSize() };
    PipelineStateDesc.RasterizerState = Create_RasterizerState();
    PipelineStateDesc.BlendState = Create_BlendState();
    PipelineStateDesc.DepthStencilState = Create_DepthStencilState();
    PipelineStateDesc.SampleMask = UINT_MAX;
    PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    PipelineStateDesc.NumRenderTargets = 5;
    PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    PipelineStateDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.RTVFormats[4] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.SampleDesc.Count = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? 4 : 1;
    PipelineStateDesc.SampleDesc.Quality = CGraphicDevice::Get_Instance()->Get_MSAA4X_Enable() ? (CGraphicDevice::Get_Instance()->Get_MSAA4X_QualityLevels() - 1) : 0;
    PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    ThrowIfFailed(m_pGraphicDevice->CreateGraphicsPipelineState(&PipelineStateDesc,
        IID_PPV_ARGS(&m_pPipelineState)));

    return S_OK;
}


D3D12_RASTERIZER_DESC CShader_Dissolve::Create_RasterizerState()
{
    D3D12_RASTERIZER_DESC RasterizerDesc;

    // 레스터라이저 설정.
    ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
    RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    RasterizerDesc.CullMode = m_eFileMode;
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

D3D12_BLEND_DESC CShader_Dissolve::Create_BlendState()
{
    D3D12_BLEND_DESC BlendDesc;

    // 블렌드 설정.
    ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
    BlendDesc.AlphaToCoverageEnable = TRUE;
    BlendDesc.IndependentBlendEnable = FALSE;
    BlendDesc.RenderTarget[0].BlendEnable = m_bIsAlphaBlend;
    BlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
    BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    return BlendDesc;
}

D3D12_DEPTH_STENCIL_DESC CShader_Dissolve::Create_DepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;

    // Depth & Stencil 설정.
    ZeroMemory(&DepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
    DepthStencilDesc.DepthEnable = m_bIsZwrite;
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

D3D12_INPUT_LAYOUT_DESC CShader_Dissolve::Create_InputLayout()
{
    return D3D12_INPUT_LAYOUT_DESC();
}


CComponent* CShader_Dissolve::Clone()
{
    return new CShader_Dissolve(*this);
}

CShader_Dissolve* CShader_Dissolve::Create(ID3D12Device* pGraphicDevice,
    ID3D12GraphicsCommandList* pCommandList)
{
    CShader_Dissolve* pInstance = new CShader_Dissolve(pGraphicDevice, pCommandList);

    if (FAILED(pInstance->Ready_Shader()))
        Engine::Safe_Release(pInstance);

    return pInstance;
}

void CShader_Dissolve::Free()
{
    CShader::Free();

    Engine::Safe_Release(m_pCBV_DescriptorHeap);
    Engine::Safe_Delete(m_pCB_MatrixInfo);
    Engine::Safe_Delete(m_pCB_BoneInfo);
    Engine::Safe_Delete(m_pCB_TextureInfo);
}