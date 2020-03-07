#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

typedef struct tagMeshTexture
{
	_uint  m_iAlbedo = 999;
	_uint m_iNormal = 999;
	_uint m_iSpecular = 999;
	_uint m_iEmissive = 999;

}MESHTEXTURE;



class ENGINE_DLL CShader_Mesh : public CShader
{
public:
	enum STATETYPE{NONALPHA,SKYDOME,ALPHA};


private:
	explicit CShader_Mesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_Mesh(const CShader_Mesh& rhs);
	virtual ~CShader_Mesh()=default;

public:
	HRESULT			Ready_Shader(STATETYPE eType);
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0,_int boneIndex = 0);

public:
	void Set_Shader_Texture(vector< ComPtr<ID3D12Resource>> pVecTexture, vector< ComPtr<ID3D12Resource>> pVecNormalTexture, vector< ComPtr<ID3D12Resource>> pVecSpecularTexture, vector< ComPtr<ID3D12Resource>> pEmissiveTexture);;

	CUploadBuffer<CB_BONE_INFO>*	Get_UploadBuffer_BoneInfo();
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
	CUploadBuffer<CB_BONE_INFO>*	m_pCB_BoneInfo = nullptr;
	CUploadBuffer<CB_TEXTURE_INFO>* m_pCB_TextureInfo = nullptr;

	vector<MESHTEXTURE> m_vecTextureType;



public:
	virtual CComponent *		Clone() override;
	static CShader_Mesh* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList,STATETYPE eType = NONALPHA);
private:
	STATETYPE m_eType = NONALPHA;
	D3D12_CULL_MODE m_eFileMode = D3D12_CULL_MODE_BACK;
	bool   m_bIsZwrite = true;
	bool   m_bIsAlphaBlend = false;


private:
	virtual void				Free();
};

END