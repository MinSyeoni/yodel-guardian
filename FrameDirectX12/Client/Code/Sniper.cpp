#include "stdafx.h"
#include "Sniper.h"
#include "Player.h"
#include "PlayerArm.h"
#include "ObjectMgr.h"
#include "LightMgr.h"
CSniper::CSniper(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
    :CWeapon(pGraphicDevice, pCommandList)
{

}

CSniper::CSniper(const CSniper& rhs)
    : CWeapon(rhs)
{
}

HRESULT CSniper::Ready_GameObjectPrototype()
{

    return S_OK;
}

HRESULT CSniper::Ready_GameObject()
{
    CWeapon::AddComponent();
    AddComponent();



    m_tagLight.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
    m_tagLight.m_vDiffuse = _vec4{ 0.3f,0.3f,0.0f,1.0f };
    m_tagLight.m_vAmbient = _vec4{ 0.4f,0.4f,0.4f,1.0f };
    m_tagLight.m_vSpecular = _vec4{ 0.4f,0.4f,0.4f,1.0f };
    m_tagLight.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
    m_tagLight.m_vPosition = _vec4{ 300.f,10.f,300.f,0.f };
    m_tagLight.m_fRange = 30.f;

    m_tagLight.m_vPosition.x = m_pTransCom->m_vPos.x;
    m_tagLight.m_vPosition.y = m_pTransCom->m_vPos.y;
    m_tagLight.m_vPosition.z = m_pTransCom->m_vPos.z;

    if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagLight)))
        return E_FAIL;

    m_uiLightIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
    CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, false);

    return S_OK;
}

HRESULT CSniper::LateInit_GameObject()
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

    m_eWeaponType = SNIPER;


    m_pFireMatrix = m_pMeshCom->Find_BoneMatrix("Mid_Assembly_Front");//ÃÑ±¸
    m_pFireMatrixOffset = m_pMeshCom->Find_BoneOffset("Mid_Assembly_Front");



    return S_OK;
}

_int CSniper::Update_GameObject(const _float& fTimeDelta)
{
    FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


    if (m_bIsDead)
        return DEAD_OBJ;

    Engine::CGameObject::Update_GameObject(fTimeDelta);

    AniCheck();
    LightCheck(fTimeDelta);

    dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation(m_eCurAniState);

    m_vecBoneMatirx = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);


    return NO_EVENT;
}

_int CSniper::LateUpdate_GameObject(const _float& fTimeDelta)
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

HRESULT CSniper::AddComponent()
{

    m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Sniper", COMPONENTID::ID_STATIC));
    NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


    return S_OK;
}

void CSniper::FallowPlayer()
{

    _matrix Rotation = XMMatrixRotationY(XMConvertToRadians(-90.f));

    _matrix matBlend;
    if (m_pPlayerEquipMatrix != nullptr)
    {

        matBlend = *m_pPlayerEquipMatrix;
        m_pTransCom->m_matWorld = matBlend * (Rotation * *m_pPlayerMatrix);

    };
}

void CSniper::FallowBag()
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


        vPos += vLook * -15.f + vRight * 15.f + vUp * 15.f;



        matBlend._41 = vPos.x;
        matBlend._42 = vPos.y;
        matBlend._43 = vPos.z;




        m_pTransCom->m_matWorld = RotationX * RotationY * RotationZ * matBlend * (RotationY * *m_pPlayerMatrix);
    }






}

void CSniper::AniCheck()
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

void CSniper::LightCheck(const _float& fTimeDelta)
{
    if (!m_bIsLight)
    {
        m_fLightAccTime = 0.f;
        CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, false);
        return;

    }
    m_fLightAccTime += fTimeDelta;

    if (m_fLightAccTime > 0.12)
    {
        m_fLightAccTime = 0.f;
        m_bIsLight = false;
    }


    _vec4 vLightPos = _vec4{ m_vLightPos.x,m_vLightPos.y,m_vLightPos.z,1.f };
    m_tagLight.m_vPosition = vLightPos;


    CLight_Manager::Get_Instance()->Set_LightInfo(m_uiLightIndex, m_tagLight);

    CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, true);
}

void CSniper::CreateShootEffect()
{

    _matrix matBlend;

    matBlend = XMMatrixInverse(nullptr, *m_pFireMatrixOffset);

    matBlend = matBlend * *m_pFireMatrix;



    _matrix Rotation = XMMatrixRotationY(XMConvertToRadians(-90.f));

    _matrix matShootPos = matBlend * *m_pPlayerEquipMatrix * (Rotation * *m_pPlayerMatrix);;

    _vec3 vPos = _vec3{ matShootPos._41,matShootPos._42,matShootPos._43 };

    m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Effect_GunFire", L"Effect", &vPos);

    m_vLightPos = vPos;
    m_bIsLight = true;
}

CGameObject* CSniper::Clone_GameObject(void* prg)
{
    CGameObject* pInstance = new CSniper(*this);

    if (FAILED(dynamic_cast<CSniper*>(pInstance)->Ready_GameObject()))
        return nullptr;

    return pInstance;
}

CSniper* CSniper::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
    CSniper* pInstance = new CSniper(pGraphicDevice, pCommandList);

    if (FAILED(pInstance->Ready_GameObjectPrototype()))
        Engine::Safe_Release(pInstance);

    return pInstance;
}

void CSniper::Free()
{
    CWeapon::Free();
}