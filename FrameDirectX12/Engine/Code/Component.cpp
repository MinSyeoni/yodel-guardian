#include "Component.h"

USING(Engine)


CComponent::CComponent()
{
}

CComponent::CComponent(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: m_pGraphicDevice(pGraphicDevice)
	, m_pCommandList(pCommandList)
	, m_bIsClone(false)
{
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice)
	, m_pCommandList(rhs.m_pCommandList)
	, m_bIsClone(true)
{
	if (rhs.m_pGraphicDevice != nullptr)
		m_pGraphicDevice->AddRef();
	if (rhs.m_pCommandList != nullptr)
		m_pCommandList->AddRef();
}


CComponent::~CComponent()
{
}

void CComponent::Update_Component(const _float & fTimeDelta)
{
}

void CComponent::Free()
{
}
