#pragma once

#include "Base.h"
#include "Shader_LightAcc.h"
#include "RcTex.h"
#include "Texture.h"
BEGIN(Engine)


class CLight final : public CBase
{
private:
	explicit CLight(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CLight() = default;

public:
	HRESULT Ready_Light(D3DLIGHT tagLightInfo);
	void SetUp_ConstateTable();
	HRESULT Render_Light(vector<ComPtr<ID3D12Resource>> pTargetTexture);
public:
	const D3DLIGHT* Get_LightInfo() const
	{
		return &m_tagLightInfo;
	}

	HRESULT Set_LightInfo(D3DLIGHT LightInfo)
	{
		m_tagLightInfo = LightInfo;
		return S_OK;
	}

	HRESULT LightOnoff(_bool blsOn) { m_blsLight = blsOn;
	return S_OK;
	};


private:
	D3DLIGHT m_tagLightInfo;
	_bool m_blsLight;
	ID3D12Device* m_pGraphicDevice;
	ID3D12GraphicsCommandList* m_pCommandList;

	CRcTex*            m_pBuffer;
	CShader_LightAcc*  m_pShader;
	_bool m_bIsInit = false;
public:
	static CLight* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, D3DLIGHT LightInfo);
protected:
	virtual void Free();
};
END