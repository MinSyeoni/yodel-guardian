#pragma once
#include "Shader.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CShader_Shadow : public CShader
{
private:
	explicit CShader_Shadow(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_Shadow(const CShader_Shadow& rhs);
	virtual ~CShader_Shadow();



public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0);
	virtual void    End_ShadowShader(bool Bone = false);

	_uint Get_CBMeshCount() { return m_iCBMeshCount; };
	_uint Get_CBBoneCount() { return m_iCBBoneCount; };
	void Set_ShadowFinish() { m_iCBMeshCount++; };

public:
	CUploadBuffer<CB_BONE_INFO>*	Get_UploadBuffer_BoneInfo();
	CUploadBuffer<CB_SHADOW_INFO>*	Get_UploadBuffer_ShadowInfo();
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
	_uint m_iCBMeshCount = 0;
	_uint m_iCBBoneCount = 0;
public:
	virtual CComponent *		Clone() override;
	static CShader_Shadow* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	CUploadBuffer<CB_BONE_INFO>*	m_pCB_BoneInfo = nullptr;
	CUploadBuffer<CB_SHADOW_INFO>*	m_pCB_ShadowInfo = nullptr;
private:
	virtual void				Free();
};

END