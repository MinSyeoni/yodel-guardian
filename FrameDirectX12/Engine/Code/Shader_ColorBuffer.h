#pragma once
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_ColorBuffer : public CShader
{
public:
	enum TYPE{NONE,WIREFRAME};
private:
	explicit CShader_ColorBuffer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_ColorBuffer(const CShader_ColorBuffer& rhs);
	virtual ~CShader_ColorBuffer();


public:
	HRESULT			Ready_Shader(TYPE etype);
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0);
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

	TYPE  m_eType = NONE;
	_bool m_bIsDepth = true;
	D3D12_CULL_MODE m_CurMode = D3D12_CULL_MODE_BACK;
	D3D12_FILL_MODE m_bIsWire = D3D12_FILL_MODE_SOLID;
public:
	virtual CComponent *		Clone() override;
	static CShader_ColorBuffer* Create(ID3D12Device* pGraphicDevice, 
									   ID3D12GraphicsCommandList* pCommandList,TYPE eType=NONE);
private:
	virtual void				Free();
};

END