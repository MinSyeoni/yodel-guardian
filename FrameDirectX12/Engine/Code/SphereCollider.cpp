#include "SphereCollider.h"
#include "SphereTex.h"
#include "Mesh.h"
#include "ComponentMgr.h"

USING(Engine)

CSphereCollider::CSphereCollider(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CCollider(pGraphicDevice, pCommandList)
{

}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	: CCollider(rhs)
{

}

HRESULT CSphereCollider::Ready_Collider_ProtoType(const COLLSHAPE& eColShape)
{
	CCubeTex;

	if (eColShape == COL_SPHERE)
	{
		CComponent* pComponent = nullptr;
		pComponent = Engine::CSphereTex::Create(m_pGraphicDevice, m_pCommandList);
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_SphereColor", ID_STATIC, pComponent), E_FAIL);
	}

	return S_OK;
}

HRESULT CSphereCollider::Ready_Collider(CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, CGameObject* pOwner)
{

	m_eShape = COL_SPHERE;


	m_fOriRadius = m_fRadius;
	m_vAddPos = vAddPos;
	m_vAngle = vAngle;

	if (pOwner != nullptr)
		m_pOwner = pOwner;

	m_pDrawMesh = static_cast<CVIBuffer*>(CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_SphereColor", ID_STATIC));
	if (m_pDrawMesh == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CSphereCollider::Ready_Collider(const _vec3& vAddPos, const _vec3& vAngle, const _float& fRadius, CGameObject* pOwner)
{
	m_eShape = COL_SPHERE;

	m_pDrawMesh = static_cast<CVIBuffer*>(CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_SphereColor", ID_STATIC));
	if (m_pDrawMesh == nullptr)
		return E_FAIL;
	m_fRadius = fRadius;
	m_fOriRadius = m_fRadius;

	m_vAddPos = vAddPos;
	m_vAngle = vAngle;



	if (pOwner != nullptr)
		m_pOwner = pOwner;

	return S_OK;
}

CComponent* CSphereCollider::Clone()
{
	return nullptr;
}

CComponent* CSphereCollider::Clone_Collider(const _bool& bIsAttatch, CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, const _float& fRadius, const _vec3& vScale, CGameObject* pOwner)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);

	HRESULT hr = NOERROR;

	if (bIsAttatch)
		hr = pInstance->Ready_Collider(pMesh, vAddPos, vAngle, pOwner);
	else
		hr = pInstance->Ready_Collider(vAddPos, vAngle, fRadius, pOwner);
	if (FAILED(hr))
		Safe_Release(pInstance);

	return pInstance;
}

CSphereCollider* CSphereCollider::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const COLLSHAPE& eColShape)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Collider_ProtoType(eColShape)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CSphereCollider::Free(void)
{

	CCollider::Free();
}
