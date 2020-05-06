#include "BoxCollider.h"
#include "CubeTex.h"
#include "Mesh.h"
#include "ComponentMgr.h"

USING(Engine)

CBoxCollider::CBoxCollider(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
    :CCollider(pGraphicDevice, pCommandList)
{

}

CBoxCollider::CBoxCollider(const CBoxCollider& rhs)
    : CCollider(rhs)
{

}

HRESULT CBoxCollider::Ready_Collider_ProtoType(const COLLSHAPE& eColShape)
{


    if (eColShape == COL_BOX)
    {
        CComponent* pComponent = nullptr;
        pComponent = Engine::CCubeTex::Create(m_pGraphicDevice, m_pCommandList);
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_CubeColor", ID_STATIC, pComponent), E_FAIL);
    }

    return S_OK;
}

HRESULT CBoxCollider::Ready_Collider(CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, CGameObject* pOwner)
{

    m_eShape = COL_BOX;

    m_vMax = pMesh->Get_MeshComponent()->Get_MaxPos();
    m_vMin = pMesh->Get_MeshComponent()->Get_MinPos();

    _vec3 vScale = m_vMax - m_vMin;

    m_matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

    m_vAddPos = vAddPos;

    m_matTrans = XMMatrixTranslation(0.f, vScale.y / 2, 0.f);

    m_vAngle = vAngle;

    if (pOwner != nullptr)
        m_pOwner = pOwner;


	m_pDrawMesh = static_cast<CVIBuffer*>(CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_CubeColor", ID_STATIC));
	if (m_pDrawMesh == nullptr)
		return E_FAIL;


    return S_OK;
}

HRESULT CBoxCollider::Ready_Collider(const _vec3& vAddPos, const _vec3& vAngle, const _vec3& vScale, CGameObject* pOwner)
{
    m_eShape = COL_BOX;

    m_pDrawMesh = static_cast<CVIBuffer*>(CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_CubeColor", ID_STATIC));
    if (m_pDrawMesh == nullptr)
        return E_FAIL;

    m_matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

    m_vAddPos = vAddPos;
    m_vAngle = vAngle;

    m_vMin = _vec3{ vScale.x * (-0.5f) ,vScale.y * (-0.5f) ,vScale.z * (-0.5f) };
    m_vMax = _vec3{ vScale.x * 0.5f ,vScale.y * 0.5f ,vScale.z * 0.5f };

    if (pOwner != nullptr)
        m_pOwner = pOwner;

    return S_OK;
}

CComponent* CBoxCollider::Clone()
{
    return nullptr;
}

CComponent* CBoxCollider::Clone_Collider(const _bool& bIsAttatch, CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, const _float& fRadius, const _vec3& vScale, CGameObject* pOwner)
{
    CBoxCollider* pInstance = new CBoxCollider(*this);

    HRESULT hr = NOERROR;

    if (bIsAttatch)
        hr = pInstance->Ready_Collider(pMesh, vAddPos, vAngle, pOwner);
    else
        hr = pInstance->Ready_Collider(vAddPos, vAngle, vScale, pOwner);
    if (FAILED(hr))
        Safe_Release(pInstance);

    return pInstance;
}

CBoxCollider* CBoxCollider::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const COLLSHAPE& eColShape)
{
    CBoxCollider* pInstance = new CBoxCollider(pGraphicDevice, pCommandList);

    if (FAILED(pInstance->Ready_Collider_ProtoType(eColShape)))
        Engine::Safe_Release(pInstance);


    return pInstance;
}

void CBoxCollider::Free(void)
{

    CCollider::Free();
}