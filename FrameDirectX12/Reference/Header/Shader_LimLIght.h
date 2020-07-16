#pragma once
#include "Shader.h"
BEGIN(Engine)

class ENGINE_DLL CShader_LimLight : public CShader
{

private:
	explicit CShader_LimLight(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader_LimLight(const CShader_LimLight& rhs);
	virtual ~CShader_LimLight() = default;

public:
	HRESULT			Ready_Shader();
	virtual void	Begin_Shader();
	virtual void	End_Shader(_uint Texnum = 0, _int boneIndex = 0);

	virtual void    End_LimShader(bool Bone = false);

	_uint Get_CBMeshCount() { return m_iCBMeshCount; };
	_uint Get_CBBoneCount() { return m_iCBBoneCount; };
	void Set_LimFinish() { m_iCBMeshCount++; };
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
public:
	virtual CComponent* Clone() override;
	static CShader_LimLight* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	D3D12_CULL_MODE m_eFileMode = D3D12_CULL_MODE_FRONT;
private:
	_uint m_iCBMeshCount = 0;
	_uint m_iCBBoneCount = 0;
private:
	virtual void				Free();
};

END