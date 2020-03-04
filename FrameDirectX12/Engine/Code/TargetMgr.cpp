#include "TargetMgr.h"

IMPLEMENT_SINGLETON(CTarget_Manager)


CTarget_Manager::CTarget_Manager()
{

}


HRESULT CTarget_Manager::Add_Target(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const _tchar * pTargetTag, const _uint & iWidth, const _uint & iHeight, DXGI_FORMAT format, _vec4 ClearColor)
{
	if (nullptr != Find_Target(pTargetTag))
		return E_FAIL;
	CTarget*	pTarget = CTarget::Create(pGraphicDevice, pCommandList);

	if (nullptr == pTarget)
		return E_FAIL;

	m_mapTarget.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget* pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>	MRTList;

		MRTList.push_back(pTarget);

		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}

	else
		pMRTList->push_back(pTarget);

	return NOERROR;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar * pMRTTag)
{

	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	_uint iIndex = 0;
	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
			pTarget->SetUp_OnGraphicDev(iIndex++);

	}

	return NO_ERROR;
}

HRESULT CTarget_Manager::End_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	_uint	iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
			pTarget->Release_OnGraphicDev(iIndex++);
	}

	return NOERROR;
}

CTarget * CTarget_Manager::Find_Target(const _tchar * pTargetTag)
{
	auto	iter = find_if(m_mapTarget.begin(), m_mapTarget.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapTarget.end())
		return nullptr;

	return iter->second;
}

list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapTarget)
		Safe_Release(Pair.second);

	m_mapTarget.clear();
}
