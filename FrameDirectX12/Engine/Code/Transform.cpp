#include "Transform.h"

USING(Engine)

CTransform::CTransform()
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_vPos(rhs.m_vPos)
	, m_vDir(rhs.m_vDir)
	, m_matWorld(rhs.m_matWorld)
{
}


CTransform::~CTransform()
{
}


HRESULT CTransform::Ready_Component()
{
	return S_OK;
}

void CTransform::Update_Component(const _float & fTimeDelta)
{
	if (m_vAngle.x > 360.f)
		m_vAngle.x -= 360.f;
	else if (m_vAngle.x < -360.f)
		m_vAngle.x += 360.f;

	if (m_vAngle.y > 360.f)
		m_vAngle.y -= 360.f;
	else if (m_vAngle.y < -360.f)
		m_vAngle.y += 360.f;

	if (m_vAngle.z > 360.f)
		m_vAngle.z -= 360.f;
	else if (m_vAngle.z < -360.f)
		m_vAngle.z += 360.f;

	_matrix matScale	= XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	_matrix matRotX		= XMMatrixRotationX(XMConvertToRadians(m_vAngle.x));
	_matrix matRotY		= XMMatrixRotationY(XMConvertToRadians(m_vAngle.y));
	_matrix matRotZ		= XMMatrixRotationZ(XMConvertToRadians(m_vAngle.z));
	_matrix matTrans	= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

CTransform * CTransform::Create()
{
	CTransform* pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Component()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}
