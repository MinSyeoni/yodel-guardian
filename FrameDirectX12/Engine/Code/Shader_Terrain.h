#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CShader_Terrain : public CShader
{
	enum TEXTURETYPE{DIFFUSS,NORMAL,SHADOW};

private:
	explicit CShader_Terrain(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_Terrain(const CShader_Terrain& rhs);
	virtual ~CShader_Terrain();



public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0);

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, vector< ComPtr<ID3D12Resource>> pNoramlTexture);

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
	vector<TEXTURETYPE> m_eTextureType;
	_int  m_iDiffussMapCount;
public:
	CUploadBuffer<CB_LIGHTMATRIX_INFO>*	Get_UploadBuffer_LightMatrixInfo() { return m_pCB_LightMatrixInfo; };
private:

	CUploadBuffer<CB_LIGHTMATRIX_INFO>*	m_pCB_LightMatrixInfo = nullptr;
public:
	virtual CComponent *		Clone() override;
	static CShader_Terrain* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};

END