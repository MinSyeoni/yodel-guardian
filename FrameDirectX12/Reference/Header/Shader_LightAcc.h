#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CShader_LightAcc : public CShader
{
private:
	explicit CShader_LightAcc(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_LightAcc(const CShader_LightAcc& rhs);
	virtual ~CShader_LightAcc() = default;



public:
	HRESULT			Ready_Shader(LIGHTTYPE eType);
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0);

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture);

	CUploadBuffer<CB_LIGHT_INFO>*	Get_UploadBuffer_LightInfo() { return m_pCB_LightInfo; };
	CUploadBuffer<CB_INVERSEMATRIX_INFO>* Get_UploadBuffer_InverseInfo() { return m_pCB_InverseInfo; };

private:
	// CShader을(를) 통해 상속됨
	virtual HRESULT						Create_DescriptorHeaps() override;
	virtual HRESULT						Create_ConstantBufferView()	override;
	virtual HRESULT						Create_PipelineState() override;
private:
	virtual D3D12_RASTERIZER_DESC		Create_RasterizerState() override;
	virtual D3D12_BLEND_DESC			Create_BlendState() override;
	virtual D3D12_DEPTH_STENCIL_DESC	Create_DepthStencilState() override;
	virtual D3D12_INPUT_LAYOUT_DESC		Create_InputLayout() override;
private:
	CUploadBuffer<CB_LIGHT_INFO>*	m_pCB_LightInfo = nullptr;
	CUploadBuffer<CB_INVERSEMATRIX_INFO>* m_pCB_InverseInfo = nullptr;
	LIGHTTYPE m_eType;


public:
	virtual CComponent *		Clone() override;
	static CShader_LightAcc* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList, LIGHTTYPE eType);
private:
	virtual void				Free();
};

END