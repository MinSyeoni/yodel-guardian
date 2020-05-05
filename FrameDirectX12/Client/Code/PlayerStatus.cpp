#include "stdafx.h"
#include "PlayerStatus.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ComponentMgr.h"
#include "ColliderMgr.h"
#include "ObjectMgr.h"
#include "Weapon.h"
#include "DynamicCamera.h"
CPlayerStatus::CPlayerStatus()
{
}

CPlayerStatus::~CPlayerStatus()
{
    Safe_Release(m_pBoxCollider);
    Safe_Release(m_pMesh);
    Safe_Release(m_pNaviMesh);
}

void CPlayerStatus::LateInit()
{
    m_pCamera = static_cast<CDynamicCamera*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));

    m_bIsInit = true;
}

_int CPlayerStatus::UpdateState(const _float& fTimeDelta, CTransform* pTranscom)
{
    LateInit();


    _matrix matRot;

    matRot = XMMatrixRotationY(XMConvertToRadians(-90.f));

    if (pTranscom != nullptr)
        m_pTransCom = pTranscom;

    if (m_eCurState != CPlayer::RIFLEDRAW)
        KeyInput();
    Rotation(fTimeDelta);
    StatusUpdate(fTimeDelta);
    PlayerDirection(fTimeDelta);
    WeaponChange();
    if (m_bIsShoot)
    {
        m_eCurState = CPlayer::RIFLEATTACK;
    }




    m_ePreState = m_eCurState;

    //   AttackCheck();

    m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);

    return NO_EVENT;
}

_int CPlayerStatus::LateUpdate(const _float& fTimeDelta)
{



    CRenderer::Get_Instance()->Add_ColliderGroup(m_pBoxCollider);
    CRenderer::Get_Instance()->Add_NaviGroup(m_pNaviMesh);
    _vec3 vShaveDir;

    for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::OBJECT))
    {


        if (CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCollider, pCol, &vShaveDir))
        {
            int i = 0;
        }
    }
    return S_OK;
}

void CPlayerStatus::SetMesh(CMesh* pMesh)
{
    m_pMesh = pMesh;
    m_pMesh->AddRef();

    m_pBoxCollider = static_cast<Engine::CBoxCollider*>(CComponentMgr::Get_Instance()->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, false, nullptr, _vec3(0.f, 6.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 150.f, 100.f), nullptr));

    m_pNaviMesh = static_cast<Engine::CNaviMesh*>(CComponentMgr::Get_Instance()->Clone_Component(L"Mesh_Navi", ID_STATIC));


}

void CPlayerStatus::KeyInput()
{
    if (m_eCurState == CPlayer::RIFLEDRAW || m_eCurState == CPlayer::RIFLEHOLSTER)
        return;

    if (m_pCamera == nullptr)
        return;
    if (m_eEquip == RIFLE)
    {
        if (!m_bIsShoot)
        {
            m_eCurState = CPlayer::RIFLEIDLE;

            m_eLegState = CPlayer::RIFLEIDLE;
        }
        else
        {
            m_eCurState = CPlayer::RIFLEATTACK;
            m_eLegState = CPlayer::RIFLEATTACK;
        }
    }
    else
    {
        m_pCamera->Set_ZoomInOut(false);
        m_eCurState = CPlayer::NONEIDLE;
        m_eLegState = CPlayer::NONEIDLE;

    }

    if (MOUSE_PRESSING(MOUSEBUTTON::DIM_LB))
    {
        if (m_eEquip == RIFLE)
        {
            m_eCurState = CPlayer::RIFLEATTACK;
            m_bIsShoot = true;
        }
    }
    else
        m_bIsShoot = false;


    if (KEY_PRESSING(DIKEYBOARD_W))
    {

        if (m_eEquip == RIFLE)
        {
            m_pCamera->Set_ZoomInOut(false);

            if (!m_bIsShoot)
                m_eCurState = CPlayer::RIFLEWALKNORTH;

            m_eLegState = CPlayer::RIFLEWALKNORTH;
        }
        else if (m_eEquip == NONE)
        {
            m_pCamera->Set_ZoomInOut(false);
            if (!m_bIsShoot)
                m_eCurState = CPlayer::NONEWALK;

            m_eLegState = CPlayer::NONEWALK;
        }
        //m_fSpeed = 5.f;
        m_fSpeed = 10.f;
    }


    if (KEY_PRESSING(DIKEYBOARD_2) && m_eEquip == NONE)
    {
        if (!m_bIsShoot)
        {
            m_eCurState = CPlayer::RIFLEDRAW;
            m_eLegState = CPlayer::RIFLEDRAW;
        }
    }


    if (KEY_PRESSING(DIKEYBOARD_F))
    {
        if (m_eEquip == RIFLE)
        {
            m_pCamera->Set_ZoomInOut(false);
            if (!m_bIsShoot)
            {
                m_eCurState = CPlayer::RIFLEHOLSTER;
                m_eLegState = CPlayer::RIFLEHOLSTER;
            }

        }
    }


    if (m_eEquip == NONE)
        return;


    if (KEY_PRESSING(DIKEYBOARD_S))
    {
        m_pCamera->Set_ZoomInOut(false);
        if (!m_bIsShoot)
            m_eCurState = CPlayer::RIFLEWALKSOUTH;

        m_eLegState = CPlayer::RIFLEWALKSOUTH;
        m_fSpeed = 5.f;
    }

    if (KEY_PRESSING(DIKEYBOARD_A))
    {
        m_pCamera->Set_ZoomInOut(false);
        if (!m_bIsShoot)
            m_eCurState = CPlayer::RIFLEWALKWEST;

        m_eLegState = CPlayer::RIFLEWALKWEST;
        m_fSpeed = 5.f;
    }



    if (KEY_PRESSING(DIKEYBOARD_D))
    {
        m_pCamera->Set_ZoomInOut(false);
        if (!m_bIsShoot)
            m_eCurState = CPlayer::RIFLEWALKEAST;

        m_eLegState = CPlayer::RIFLEWALKEAST;
        m_fSpeed = 5.f;
    }
    if (MOUSE_KEYDOWN(MOUSEBUTTON::DIM_RB))
    {
        if (m_eEquip != NONE)
        {
            _bool IsZoom = m_pCamera->Get_ZoomOut();
            if (IsZoom == false)
                m_pCamera->Set_ZoomInOut(true);
            else
                m_pCamera->Set_ZoomInOut(false);

        }
    }

}

void CPlayerStatus::WeaponChange()
{
    if (m_eCurState == CPlayer::RIFLEDRAW)
    {

        list<CGameObject*>* pList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Weapon");

        for (auto& pSrc : *pList)
        {
            if (static_cast<CWeapon*>(pSrc)->Get_WeaponType() == CWeapon::RIFLE && static_cast<CWeapon*>(pSrc)->Get_WeaponState() == CWeapon::BAG)
            {
                if (m_pMesh->Set_FindAnimation(1800.f, (int)CPlayer::RIFLEDRAW))
                {
                    static_cast<CWeapon*>(pSrc)->SetWeaponState(CWeapon::EQUIP);
                }
            }
        }

        if (m_pMesh->Set_FindAnimation(5500.f, (int)CPlayer::RIFLEDRAW))
        {
            m_eCurState = CPlayer::RIFLEIDLE;
            m_eLegState = CPlayer::RIFLEIDLE;
            m_eEquip = RIFLE;
        }

    }

    if (m_eCurState == CPlayer::RIFLEHOLSTER)
    {
        list<CGameObject*>* pList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Weapon");

        for (auto& pSrc : *pList)
        {
            if (static_cast<CWeapon*>(pSrc)->Get_WeaponType() == CWeapon::RIFLE && static_cast<CWeapon*>(pSrc)->Get_WeaponState() == CWeapon::EQUIP)
            {
                if (m_pMesh->Set_FindAnimation(5000.f, (int)CPlayer::RIFLEHOLSTER))
                {
                    static_cast<CWeapon*>(pSrc)->SetWeaponState(CWeapon::BAG);
                }
            }
        }

        if (m_pMesh->Set_FindAnimation(5000.f, (_int)CPlayer::RIFLEHOLSTER))
        {
            m_eCurState = CPlayer::NONEIDLE;
            m_eLegState = CPlayer::NONEIDLE;
            m_eEquip = NONE;
        }

    }

}




void CPlayerStatus::StatusUpdate(const _float& fTimeDelta)
{

    if (m_pTransCom == nullptr)
        return;

    _vec3 vRight;
    memcpy(&vRight, &m_pTransCom->m_matWorld._11, sizeof(_vec3));
    vRight.Normalize();
    switch (m_eLegState)
    {
    case CPlayer::RIFLEWALKNORTH:
    {
        m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta);
        break;
    }
    case CPlayer::RIFLEWALKSOUTH:
    {
        m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(m_pTransCom->m_vDir * -1), m_fSpeed * fTimeDelta);
        break;
    }
    case CPlayer::RIFLEWALKEAST:
    {
        m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(vRight), m_fSpeed * fTimeDelta);
        break;
    }
    case CPlayer::RIFLEWALKWEST:
    {
        m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(vRight * -1.f), m_fSpeed * fTimeDelta);
        break;
    }
    case CPlayer::NONEWALK:
    {
        m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta);
        break;

    }
    default:
        break;
    }



}

void CPlayerStatus::Rotation(const _float& fTimeDelta)
{

    //m_pTransCom->Chase_Target(m_vecTargetPos, fTimeDelta*0.05f);
}

void CPlayerStatus::PlayerDirection(const _float& fTimeDelta)
{

    _long dwMouseMove;

    if (dwMouseMove = CDirectInput::Get_Instance()->Get_DIMouseMove(CDirectInput::DIMM_X))
    {

        _vec3 vUp = _vec3{ 0.f,1.f,0.f };
        _matrix matRot;

        m_pTransCom->m_vAngle.y += (_float)dwMouseMove * fTimeDelta * 1.5f;
        m_pTransCom->m_vDir = _vec3{ m_pTransCom->m_matWorld._31,m_pTransCom->m_matWorld._32,m_pTransCom->m_matWorld._33 };
        m_pTransCom->m_vDir.y = 0.f;
        m_pTransCom->m_vDir.Normalize();
        m_vecTargetPos = m_pTransCom->m_vPos + (m_pTransCom->m_vDir) * 30.f;

    }
    if (dwMouseMove = CDirectInput::Get_Instance()->Get_DIMouseMove(CDirectInput::DIMM_Y))
    {
        m_fSpine -= dwMouseMove * fTimeDelta;

        if (m_fSpine > 30.f)
            m_fSpine = 30.f;
        if (m_fSpine < -30.f)
            m_fSpine = -30.f;
    }




}

void CPlayerStatus::AttackCheck()
{
    if (m_eCurState == CPlayer::RIFLEATTACK)
    {

        if (m_pMesh->Set_FindAnimation(2000.f, (int)CPlayer::RIFLEATTACK))
        {
            //m_eCurState = CPlayer::RIFLEIDLE;
            //m_ePreState = CPlayer::RIFLEIDLE;

        }
    }

}