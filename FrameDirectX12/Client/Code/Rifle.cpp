#include "stdafx.h"
#include "Rifle.h"
#include "Player.h"
#include "PlayerArm.h"
#include "PlayerLeg.h"
#include "ObjectMgr.h"
#include "LightMgr.h"
#include "DirectInput.h"
#include "Frustom.h"
#include "DirectInput.h"
#include "EquipUI.h"
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


  
   m_tagLight.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
   m_tagLight.m_vDiffuse = _vec4{ 0.6f,0.6f,0.6f,1.0f };
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

    m_uiLightIndex= CLight_Manager::Get_Instance()->Get_LightIndex();
    CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, false);



    m_tagSpotLight.m_eType = LIGHTTYPE::D3DLIGHT_SPOT;
    m_tagSpotLight.m_vDiffuse = _vec4{ 5.0f,5.0f,5.0f,1.0f };
    m_tagSpotLight.m_vAmbient = _vec4{ 0.3f,0.3f,0.3f,1.0f };
    m_tagSpotLight.m_vSpecular = _vec4{ 0.4f,0.4f,0.4f,1.0f };
    m_tagSpotLight.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
    m_tagSpotLight.m_vPosition = _vec4{ 300.f,10.f,300.f,0.f };
    m_tagSpotLight.m_fRange = 50.f;

    if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagSpotLight)))
        return E_FAIL;

    m_uiSpotLightIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
    CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightIndex, true);

    m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
    m_pTransCom->m_vPos = _vec3(299.f, 9.f, 494.f);
    m_pTransCom->m_vAngle = _vec3(0.f, 0.f, 0.f);
    m_eWeaponState = BAG;//�ӽ�
    
    return S_OK;
}

HRESULT CRifle::LateInit_GameObject()
{
    m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());
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


    m_pFireMatrix = m_pMeshCom->Find_BoneMatrix("Mid_Assembly_Front");//�ѱ�
    m_pFireMatrixOffset = m_pMeshCom->Find_BoneOffset("Mid_Assembly_Front");



    return S_OK;
}

_int CRifle::Update_GameObject(const _float& fTimeDelta)
{
    FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


    if (m_bIsDead)
        return DEAD_OBJ;

    Engine::CGameObject::Update_GameObject(fTimeDelta);

    AniCheck();
    LightCheck(fTimeDelta);
    SpotLightCheck();
    DropCheck();
    dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation(m_eCurAniState);

    m_vecBoneMatirx = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);


    return NO_EVENT;
}

_int CRifle::LateUpdate_GameObject(const _float& fTimeDelta)
{


    NULL_CHECK_RETURN(m_pRenderer, -1);

    FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
    FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

    if(m_bIsLimLight)
        FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_LIMLIGHT, this), -1);

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

        matBlend = *m_pPlayerEquipMatrix;
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
    if (m_eWeaponState == DROP)
        m_eCurAniState = BASE;


}

void CRifle::DropCheck()
{
    if (m_eWeaponState == DROP && CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
    {
        m_bIsLimLight = true;

      //  ShowEquipUI(true);

    }
    else
    {

       // ShowEquipUI(false);
        m_bIsLimLight = false;

    }

    if (m_eWeaponState == DROP &&m_bIsLimLight && KEY_DOWN(DIK_E))
    {

        CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");

        _vec3 vPlayerPos = pPlayer->Get_Transform()->m_vPos;

        _vec3 vDist = vPlayerPos - m_pTransCom->m_vPos;
        
        float fDist = vDist.Get_Length();

        if (fDist < 15.f)
        {

            m_eCurAniState = COLLAPSEBASE;
            m_eWeaponState = BAG;
            m_pTransCom->m_vPos = _vec3(0.f, 0.f, 0.f);
            m_pTransCom->m_vAngle = _vec3(0.f, 0.f, 0.f);
            m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);
            m_bIsLimLight = false;
        }


    }




}

void CRifle::LightCheck(const _float& fTimeDelta)
{
    if (!m_bIsLight)
    {
        m_fLightAccTime = 0.f;
        CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, false);
        return;

    }
    m_fLightAccTime += fTimeDelta;

    if (m_fLightAccTime > 0.1)
    {
        m_fLightAccTime = 0.f;
        m_bIsLight = false;
    }


    _vec4 vLightPos = _vec4{ m_vLightPos.x,m_vLightPos.y,m_vLightPos.z,1.f };
    m_tagLight.m_vPosition = vLightPos;


    CLight_Manager::Get_Instance()->Set_LightInfo(m_uiLightIndex, m_tagLight);

    CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiLightIndex, true);
}

void CRifle::CreateShootEffect()
{

    _matrix matBlend;

    matBlend=  XMMatrixInverse(nullptr, *m_pFireMatrixOffset);

    matBlend = matBlend * *m_pFireMatrix;



    _matrix Rotation = XMMatrixRotationY(XMConvertToRadians(-90.f));

    _matrix matShootPos = matBlend * *m_pPlayerEquipMatrix * (Rotation * *m_pPlayerMatrix);;

    _vec3 vPos = _vec3{ matShootPos._41,matShootPos._42,matShootPos._43 };
    _vec3 vDir = _vec3{ matShootPos._31,matShootPos._32,matShootPos._33 };

    m_vLightPos = vPos;
    m_vLightDir = vDir;

    m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Effect_GunFire", L"Effect", &vPos);


    m_bIsLight = true;
}

void CRifle::SpotLightCheck()
{
    if (EQUIP == m_eWeaponState)
    {
        if (CDirectInput::Get_Instance()->Key_Down(DIKEYBOARD_Q))
        {

            if (m_bIsSpotLight)
                m_bIsSpotLight = false;
            else if (!m_bIsSpotLight)
                m_bIsSpotLight = true;
        }

    }
    else
        m_bIsSpotLight = false;


        _matrix matBlend;

        matBlend = XMMatrixInverse(nullptr, *m_pFireMatrixOffset);

        matBlend =  *m_pFireMatrix;


        _matrix Rotation = XMMatrixRotationY(XMConvertToRadians(205.f));

        _matrix matShootPos = matBlend *  *m_pPlayerEquipMatrix * (Rotation * *m_pPlayerMatrix);
        _vec3 vPos = _vec3{ matShootPos._41,matShootPos._42,matShootPos._43 };

        _vec3 vDir = _vec3{ matShootPos._31,matShootPos._32,matShootPos._33 };


        vDir.Normalize();


        _vec4 vLightPos = _vec4{ vPos.x,vPos.y,vPos.z,1.f };
        m_tagSpotLight.m_vPosition = vLightPos;

        _vec4 vLightDir = _vec4{ -vDir.x,-vDir.y,-vDir.z,1.f };
        m_tagSpotLight.m_vDirection = vLightDir;

        CLight_Manager::Get_Instance()->Set_LightInfo(m_uiSpotLightIndex, m_tagSpotLight);

        CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightIndex, m_bIsSpotLight);

    

}

void CRifle::ShowEquipUI(bool bIsRender)
{

    CGameObject* pGameObject = nullptr;

    list<CGameObject*>* pEquipUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"EquipUI");
    for (auto& pSrc : *pEquipUIList)
        if (CEquipUI::E_KEYEQUIP == dynamic_cast<CEquipUI*>(pSrc)->Get_EquipType())
            pGameObject = dynamic_cast<CEquipUI*>(pSrc);

    dynamic_cast<CEquipUI*>(pGameObject)->Set_ShowUI(bIsRender);
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