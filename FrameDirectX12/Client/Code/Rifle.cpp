#include "stdafx.h"
#include "Rifle.h"
#include "Player.h"
#include "PlayerArm.h"
#include "PlayerLeg.h"
#include "ObjectMgr.h"

CRifle::CRifle(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
    :CWeapon(pGraphicDevice, pCommandList)
{

}

CRifle::CRifle(const CRifle& rhs)
    : CWeapon(rhs)
{
}

HRESULT CRifle::Ready_GameObjectPrototype()
{

    return S_OK;
}

HRESULT CRifle::Ready_GameObject()
{
    CWeapon::AddComponent();
    AddComponent();

    return S_OK;
}

HRESULT CRifle::LateInit_GameObject()
{
    m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());
    m_pTransCom->m_vScale = _vec3(1.0f, 1.0f, 1.0f);
    m_pTransCom->m_vPos = _vec3(0.f, 0.f, 0.f);
    CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");

    if (pPlayer != nullptr)
    {
        m_pPlayerArm = static_cast<CPlayer*>(pPlayer)->Get_PlayerArm();
    }


    m_pPlayerEquipMatrix = static_cast<CMesh*>(m_pPlayerArm->Get_Component(L"Com_Mesh", COMPONENTID::ID_STATIC))->Get_AnimationComponent()->Get_WeaponMatrix();



    m_pPlayerBodyMatrix = static_cast<CMesh*>(m_pPlayerArm->Get_Component(L"Com_Mesh", COMPONENTID::ID_STATIC))->Find_BoneMatrix("Chest2");
    m_pPlayerBodyMatrixOffset = static_cast<CMesh*>(m_pPlayerArm->Get_Component(L"Com_Mesh", COMPONENTID::ID_STATIC))->Find_BoneOffset("Chest2");


    m_pPlayerMatrix = &(m_pPlayerArm->Get_Transform()->m_matWorld);

    m_eWeaponType = RIFLE;

    return S_OK;
}

_int CRifle::Update_GameObject(const _float& fTimeDelta)
{
    FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


    if (m_bIsDead)
        return DEAD_OBJ;

    Engine::CGameObject::Update_GameObject(fTimeDelta);

    AniCheck();


    dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation(m_eCurAniState);

    m_vecBoneMatirx = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);


    return NO_EVENT;
}

_int CRifle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    NULL_CHECK_RETURN(m_pRenderer, -1);

    FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
    FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

    if (m_eWeaponState == EQUIP)
        FallowPlayer();
    else if (m_eWeaponState == BAG)
        FallowBag();


    return S_OK;
}

HRESULT CRifle::AddComponent()
{

    m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Rifle", COMPONENTID::ID_STATIC));
    NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


    return S_OK;
}

void CRifle::FallowPlayer()
{

    _matrix Rotation = XMMatrixRotationY(XMConvertToRadians(-90.f));

    _matrix matBlend;
    if (m_pPlayerEquipMatrix != nullptr)
    {

        matBlend = (*m_pPlayerEquipMatrix * 1.0f);
        m_pTransCom->m_matWorld = matBlend * (Rotation * *m_pPlayerMatrix);

    };
}

void CRifle::FallowBag()
{
    _matrix RotationX = XMMatrixRotationX(XMConvertToRadians(90.f));
    _matrix RotationY = XMMatrixRotationY(XMConvertToRadians(-90.f));

    _matrix RotationZ = XMMatrixRotationZ(XMConvertToRadians(0.f));
    _matrix matBlend;
    if (m_pPlayerBodyMatrix != nullptr)
    {

        _matrix Offset = XMMatrixInverse(nullptr, *m_pPlayerBodyMatrixOffset);

        matBlend = Offset * *m_pPlayerBodyMatrix;

        _vec3  vLook, vRight, vUp, vPos;

        memcpy(&vRight, &matBlend._11, sizeof(_vec3));
        memcpy(&vLook, &matBlend._21, sizeof(_vec3));
        memcpy(&vUp, &matBlend._31, sizeof(_vec3));
        memcpy(&vPos, &matBlend._41, sizeof(_vec3));

        vPos += vLook * -15.f + vRight * -8.f;

        matBlend._41 = vPos.x;
        matBlend._42 = vPos.y;
        matBlend._43 = vPos.z;


        m_pTransCom->m_matWorld = RotationX * RotationY * RotationZ * matBlend * (RotationY * *m_pPlayerMatrix);
    }




}

void CRifle::AniCheck()
{
    if (m_eCurAniState == COLLAPSEBASE && m_eWeaponState == EQUIP)
    {
        m_eCurAniState = EXPAND;
        return;
    }
    if (m_eCurAniState == EXPAND && m_pMeshCom->Set_FindAnimation(1000.f, (int)EXPAND))
    {
        m_eCurAniState = BASE;
        return;
    }
    if (m_eCurAniState == BASE && m_eWeaponState == BAG)
    {
        m_eCurAniState = COLLAPSE;
        return;
    }
    if (m_eCurAniState == COLLAPSE && m_pMeshCom->Set_FindAnimation(1000.f, (int)COLLAPSE))
    {
        m_eCurAniState = COLLAPSEBASE;
        return;
    }


}

CGameObject* CRifle::Clone_GameObject(void* prg)
{
    CGameObject* pInstance = new CRifle(*this);

    if (FAILED(dynamic_cast<CRifle*>(pInstance)->Ready_GameObject()))
        return nullptr;

    return pInstance;
}

CRifle* CRifle::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
    CRifle* pInstance = new CRifle(pGraphicDevice, pCommandList);

    if (FAILED(pInstance->Ready_GameObjectPrototype()))
        Engine::Safe_Release(pInstance);

    return pInstance;
}

void CRifle::Free()
{
    CWeapon::Free();
}