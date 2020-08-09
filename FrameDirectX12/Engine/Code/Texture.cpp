#include "Texture.h"
#include "GraphicDevice.h"
USING(Engine)

CTexture::CTexture(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice, pCommandList)
{ 
}

CTexture::CTexture(const CTexture & rhs)
	:CComponent(rhs)
	,m_vecResource(rhs.m_vecResource)
	,m_vecUpload(rhs.m_vecUpload)
{
	for (auto& pTexture : m_vecResource)
		pTexture->AddRef();


	for (auto& pTexture : m_vecUpload)
		pTexture->AddRef();

	lstrcpy(m_szFileName, rhs.m_szFileName);
}

HRESULT CTexture::Ready_Texture(TEXTURETYPE eType, const _tchar * pFileName, const _uint & iCnt)
{

	Engine::CGraphicDevice::Get_Instance()->Begin_ResetCmdListThread();
	lstrcpy(m_szFileName, pFileName);

	_tchar			szFullPath[MAX_PATH] = L"";
	for (size_t i = 0; i < iCnt; ++i)
	{
		_tchar			szFullPath[MAX_PATH] = L"";
		wsprintf(szFullPath, pFileName, i);

		ComPtr<ID3D12Resource> pResource =nullptr;
		ComPtr<ID3D12Resource> pUpload =nullptr;

		switch (eType)
		{
		case TEXTURETYPE::TEX_NORMAL:


			ThrowIfFailed(CreateDDSTextureFromFile12(m_pGraphicDevice, CGraphicDevice::Get_Instance()->Get_CommandListThread(), szFullPath, pResource,pUpload));
		}


		m_vecResource.push_back(pResource);
		m_vecUpload.push_back(pUpload);

	}
	
	Engine::CGraphicDevice::Get_Instance()->End_ResetCmdListThread();
	return S_OK;
}

CTexture * CTexture::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, TEXTURETYPE eType, const _tchar * pFileName, const _uint & iCnt)
{
	CTexture* pInstance = new CTexture(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Texture( eType, pFileName, iCnt)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{


	m_vecResource.shrink_to_fit();
	m_vecResource.clear();
	m_vecUpload.shrink_to_fit();
	m_vecResource.clear();
}
