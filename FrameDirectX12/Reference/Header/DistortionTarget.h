#pragma once

#include "Base.h"
#include "RcTex.h"
#include "Shader_DefaultTex.h"


class ENGINE_DLL CDistortionTarget : public CBase
{
private:
	explicit CDistortionTarget(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CDistortionTarget() = default;

public:
	HRESULT Ready_Target();

	vector<ComPtr<ID3D12Resource>> GetTargetTexture() {
		return m_vecDestortionTarget;
	};


	_uint Get_Widht() { return m_uiWidth; };
	_uint Get_Height() { return m_uiHeight; };

	HRESULT SetUp_OnGraphicDev();
	HRESULT Release_OnGraphicDev();
	HRESULT BuildResource();

	void Render_RenderTarget();
	void SetUp_ConstateTable(const int offset);

	HRESULT BuildDescriptors();

	void OnResize(_uint newWidth, _uint newHeight);

private:
	ID3D12Device* m_pGraphicDev = nullptr;
	ID3D12GraphicsCommandList* m_pCommandLst = nullptr;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT    m_ScissorRect;

	_uint m_uiWidth = 0;
	_uint m_uiHeight = 0;
	_float m_ClearColor[4];

	DXGI_FORMAT mFormat = DXGI_FORMAT_R24G8_TYPELESS;


	ID3D12DescriptorHeap*	m_pRTV_Heap = nullptr;
	_uint					m_uiRTV_DescriptorSize = 0;

	vector<ComPtr<ID3D12Resource>> m_vecDestortionTarget;

	CRcTex* m_pBuffer;
	CShader_DefaultTex* m_pShader;


public:
	static CDistortionTarget* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual void Free();
};
