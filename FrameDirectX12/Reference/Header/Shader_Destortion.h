#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CShader_Destortion : public CShader
{
private:
	explicit CShader_Destortion(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_Destortion(const CShader_Destortion& rhs);
	virtual ~CShader_Destortion() = default;



public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0,_int boneIndex = 0);

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pNormalTexture);

	CUploadBuffer<CB_DISTORT_INFO>* Get_UploadBuffer_DistortInfo() { return m_pCB_DistortInfo; };

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
	CUploadBuffer<CB_DISTORT_INFO>*	m_pCB_DistortInfo = nullptr;
	_uint  m_iTexSize=0;
public:
	virtual CComponent *		Clone() override;
	static CShader_Destortion* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};

END