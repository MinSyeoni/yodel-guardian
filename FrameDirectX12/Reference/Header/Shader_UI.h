#pragma once
#include "Shader.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CShader_UI : public CShader
{
public:
	enum TYPE { NONE, ALPHA, HPBAR, HPBAR1, HPBAR2, MPBAR, CARDTAG, ALPHABLEND};

private:
    explicit CShader_UI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
    explicit CShader_UI(const CShader_UI& rhs);
    virtual ~CShader_UI();

public:
    HRESULT        Ready_Shader(TYPE eType);
    virtual void   Begin_Shader();
    virtual void   End_Shader(_uint Texnum = 0, _uint uiOffset = 0);

public:
    void           Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, _uint offset = 1);
private:
    // CShader을(를) 통해 상속됨
    virtual HRESULT                     Create_DescriptorHeaps()        override;
    virtual HRESULT                     Create_ConstantBufferView()     override;
    virtual HRESULT                     Create_PipelineState()          override;
private:
    virtual D3D12_RASTERIZER_DESC       Create_RasterizerState()        override;
    virtual D3D12_BLEND_DESC            Create_BlendState()             override;
    virtual D3D12_DEPTH_STENCIL_DESC    Create_DepthStencilState()      override;
    virtual D3D12_INPUT_LAYOUT_DESC     Create_InputLayout()            override;

private:
	TYPE            m_eType = NONE;
    bool            m_bIsAlpha = false;
    bool            m_bIsAlphaBlend = false;
	D3D12_FILL_MODE m_bIsWire = D3D12_FILL_MODE_SOLID;

public:
    virtual CComponent* Clone() override;
    static  CShader_UI* Create(ID3D12Device* pGraphicDevice,
        ID3D12GraphicsCommandList* pCommandList, TYPE eType = NONE);
private:
    virtual void            Free();
};

END