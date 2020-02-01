#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTexture  : public CComponent
{

private:
	explicit CTexture(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Ready_Texture(TEXTURETYPE eType, const _tchar* pFileName, const _uint& iCnt);
public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_vecResource; };

public:
	static CTexture* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,TEXTURETYPE eType, const _tchar* pFileName, const _uint& iCnt = 1);
public:
	virtual CComponent* Clone();

private:
	_tchar									m_szFileName[MAX_PATH] = L"";

	vector< ComPtr<ID3D12Resource>> m_vecResource;
	vector< ComPtr<ID3D12Resource>> m_vecUpload;
protected:
	virtual void Free();

};

END