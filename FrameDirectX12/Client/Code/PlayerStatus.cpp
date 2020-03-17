#include "stdafx.h"
#include "PlayerStatus.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ComponentMgr.h"
#include "ColliderMgr.h"
CPlayerStatus::CPlayerStatus()
{
}

CPlayerStatus::~CPlayerStatus()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pMesh);
	Safe_Release(m_pNaviMesh);
}

_int CPlayerStatus::UpdateState(const _float & fTimeDelta, CTransform * pTranscom)
{
	_matrix matRot;

	matRot=XMMatrixRotationY(XMConvertToRadians(-90.f));

	if(pTranscom!=nullptr)
	m_pTransCom = pTranscom;


	KeyInput();
	Rotation(fTimeDelta);
	StatusUpdate(fTimeDelta);
	PlayerDirection(fTimeDelta);

	m_ePreState = m_eCurState;

	matRot =  m_pTransCom->m_matWorld;

	m_pBoxCollider->Update_Collider(&matRot);

	return NO_EVENT;
}

_int CPlayerStatus::LateUpdate(const _float & fTimeDelta)
{
	CRenderer::Get_Instance()->Add_ColliderGroup(m_pBoxCollider);
	CRenderer::Get_Instance()->Add_NaviGroup(m_pNaviMesh);
	_vec3 vShaveDir;

	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::OBJECT))
	{

		
			if (CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCollider,pCol,&vShaveDir))
			{
				int i = 0;
			}
	}
	return S_OK;
}

void CPlayerStatus::SetMesh(CMesh * pMesh)
{
	m_pMesh = pMesh;
	m_pMesh->AddRef();

	m_pBoxCollider = static_cast<Engine::CBoxCollider*>(CComponentMgr::Get_Instance()->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, false, nullptr, _vec3(0.f, 6.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 150.f, 100.f), nullptr));
	
	m_pNaviMesh = static_cast<Engine::CNaviMesh*>(CComponentMgr::Get_Instance()->Clone_Component(L"Mesh_Navi", ID_STATIC));


}

void CPlayerStatus::KeyInput()
{
	m_eCurState = CPlayer::IDLE;

	if (KEY_PRESSING(DIKEYBOARD_W))
	{
		m_eCurState = CPlayer::WALKNORTH;
		m_fSpeed = 5.f;
	}

	if (KEY_PRESSING(DIKEYBOARD_S))
	{
		m_eCurState = CPlayer::WALKSOUTH;
		m_fSpeed = 5.f;
	}

	if (KEY_PRESSING(DIKEYBOARD_A))
	{
		m_eCurState = CPlayer::WALKEAST;
		m_fSpeed = 5.f;
	}
	if (KEY_PRESSING(DIKEYBOARD_D))
	{
		m_eCurState = CPlayer::WALKWEST;
		m_fSpeed = 5.f;
	}

}

void CPlayerStatus::StatusUpdate(const _float & fTimeDelta)
{

	if (m_pTransCom == nullptr)
		return;

	_vec3 vRight;
	memcpy(&vRight, &m_pTransCom->m_matWorld._11, sizeof(_vec3));
	vRight.Normalize();
	switch (m_eCurState)
	{
	case CPlayer::WALKNORTH:
	{
	 m_pTransCom->m_vPos=  m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed*fTimeDelta);
	 break;
	}
	case CPlayer::WALKSOUTH:
	{
	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(m_pTransCom->m_vDir*-1), m_fSpeed*fTimeDelta);
	break;
	}
	case CPlayer::WALKEAST:
	{
		m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(vRight*-1.f), m_fSpeed*fTimeDelta);
		break;
	}
	case CPlayer::WALKWEST:
	{
		m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(vRight), m_fSpeed*fTimeDelta);
		break;
	}




	default:
		break;
	}



}

void CPlayerStatus::Rotation(const _float& fTimeDelta)
{
	
	m_pTransCom->Chase_Target(m_vecTargetPos, fTimeDelta*0.05f);
}

void CPlayerStatus::PlayerDirection(const _float&fTimeDelta)
{

	_long dwMouseMove;

	if (dwMouseMove = CDirectInput::Get_Instance()->Get_DIMouseMove(CDirectInput::DIMM_X))
	{

		_vec3 vUp = _vec3{ 0.f,1.f,0.f };
		_matrix matRot;

		m_pTransCom->m_vAngle.y += (_float)dwMouseMove*fTimeDelta*1.5f;
		m_pTransCom->m_vDir = _vec3{ m_pTransCom->m_matWorld._31,m_pTransCom->m_matWorld._32,m_pTransCom->m_matWorld._33 };
		m_pTransCom->m_vDir.y = 0.f;
		m_pTransCom->m_vDir.Normalize();
		m_vecTargetPos = m_pTransCom->m_vPos + (m_pTransCom->m_vDir)*30.f;

	}
	if (dwMouseMove = CDirectInput::Get_Instance()->Get_DIMouseMove(CDirectInput::DIMM_Y))
	{
		m_fSpine += dwMouseMove * fTimeDelta;

		if (m_fSpine > 30.f)
			m_fSpine = 30.f;
		if (m_fSpine < -30.f)
			m_fSpine = -30.f;
	}




}
