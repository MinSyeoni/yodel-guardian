#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CShader_DownSample : public CShader
{
private:
	explicit CShader_DownSample(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_DownSample(const CShader_DownSample& rhs);
	virtual ~CShader_DownSample() = default;



public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader();

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture);


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

	CUploadBuffer<CB_SAMPLE_INFO>*	m_pCB_SampleInfo = nullptr;


public:
	virtual CComponent *		Clone() override;
	static CShader_DownSample* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

	_bool m_bIsInit = false;
private:
	virtual void				Free();
};

END