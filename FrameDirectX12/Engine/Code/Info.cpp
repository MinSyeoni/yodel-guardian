#include "Info.h"

USING(Engine)

CInfo::CInfo()
{
}

CInfo::CInfo(const CInfo & rhs)
	: m_fSpeed(rhs.m_fSpeed)
	, m_fRadius(rhs.m_fRadius)
{
}


CInfo::~CInfo()
{
}

HRESULT CInfo::Ready_Component()
{
	return S_OK;
}

void CInfo::Update_Component(const _float & fTimeDelta)
{
}

CComponent * CInfo::Clone()
{
	return new CInfo(*this);
}

CInfo * CInfo::Create()
{
	CInfo* pInstance = new CInfo;

	if (FAILED(pInstance->Ready_Component()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CInfo::Free()
{
	CComponent::Free();
}
