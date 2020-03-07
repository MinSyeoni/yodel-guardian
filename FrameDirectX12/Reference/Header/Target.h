#pragma once

#include "Base.h"
#include "RcTex.h"
#include "Shader_DefaultTex.h"


class ENGINE_DLL CTarget : public CBase
{
private:
	explicit CTarget(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CTarget() = default;

public:
	HRESULT Ready_Target();
	
	vector<ComPtr<ID3D12Resource>> GetTargetTexture() {
		return m_vecTargetTexture;
	};

	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT Release_OnGraphicDev(const _uint& iIndex);

	
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

	_float m_ArrbedoClear[4];
	_float m_NormalClear[4];
	_float m_DepthClear[4];
	_float m_SpecClear[4];
	_float m_EmissiveClear[4];

	DXGI_FORMAT mFormat = DXGI_FORMAT_R24G8_TYPELESS;

	ID3D12DescriptorHeap*	m_pRTV_Heap = nullptr;
	ID3D12DescriptorHeap*	m_pDSV_Heap = nullptr;

	_uint					m_uiRTV_DescriptorSize = 0;
	_uint					m_uiDSV_DescriptorSize = 0;
	_uint					m_uiCBV_SRV_UAV_DescriptorSize = 0;

	vector<ComPtr<ID3D12Resource>> m_vecTargetTexture;

	CRcTex* m_pBuffer;
	CShader_DefaultTex* m_pShader;

public:
	static CTarget* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual void Free();
};
