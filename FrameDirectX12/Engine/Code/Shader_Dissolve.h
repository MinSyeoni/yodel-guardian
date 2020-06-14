#pragma once
#include "Shader.h"
#include "Texture.h"

BEGIN(Engine)

typedef struct tagDissolveTexture
{
	_uint  m_iAlbedo = 999;
	_uint m_iNormal = 999;
	_uint m_iSpecular = 999;
	_uint m_iEmissive = 999;

}DISSOLVETEXTURE;

class ENGINE_DLL CShader_Dissolve : public CShader
{
private:
	explicit CShader_Dissolve(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_Dissolve(const CShader_Dissolve& rhs);
	virtual ~CShader_Dissolve()=default;

public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0,_int boneIndex = 0);

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, vector< ComPtr<ID3D12Resource>> pVecNormalTexture, vector< ComPtr<ID3D12Resource>> pVecSpecularTexture, vector< ComPtr<ID3D12Resource>> pEmissiveTexture,
							vector< ComPtr<ID3D12Resource>> pVecDissolveTexture);;


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
	CUploadBuffer<CB_TEXTURE_INFO>* m_pCB_TextureInfo = nullptr;

	vector<DISSOLVETEXTURE> m_vecTextureType;


public:
	virtual CComponent *		Clone() override;
	static CShader_Dissolve* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	D3D12_CULL_MODE m_eFileMode = D3D12_CULL_MODE_BACK;
	bool			m_bIsZwrite = true;
	_bool			m_bIsAlphaBlend = false;
	_int			m_iDissolveOffset = 0;

private:
	virtual void				Free();
};

END