#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const D3DLIGHT*  CLight_Manager::Get_LightInfo(const _uint & iIndex)
{
	if (m_LightList.size() <= iIndex)
		return nullptr;

	auto	iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;


	return (*iter)->Get_LightInfo();
}

HRESULT CLight_Manager::Set_LightInfo(const _uint & iIndex, D3DLIGHT LightInfo)
{
	if (m_LightList.size() < iIndex)
		return E_FAIL;

	auto iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Set_LightInfo(LightInfo);
}

HRESULT CLight_Manager::Set_LightOnOff(const _uint & iIndex, _bool blsLight)
{
	if (m_LightList.size() < iIndex)
		return E_FAIL;

	auto iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->LightOnoff(blsLight);
}

HRESULT CLight_Manager::Add_Light(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const D3DLIGHT* pLightInfo)
{
	if (nullptr == pLightInfo)
		return E_FAIL;

	CLight*		pLight = CLight::Create(pGraphicDevice, pCommandList, *pLightInfo);
	if (nullptr == pLight)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return NOERROR;
}

HRESULT CLight_Manager::Render_Light(vector<ComPtr<ID3D12Resource>> pTargetTexture)
{

	for (auto& pLight : m_LightList)
	{
		if (nullptr != pLight)
			pLight->Render_Light(pTargetTexture);
	}


	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);
	m_LightList.clear();
}
