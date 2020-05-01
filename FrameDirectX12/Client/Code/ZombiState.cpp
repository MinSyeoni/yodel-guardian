#include "stdafx.h"
#include "ZombiState.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"

CZombiState::CZombiState()
{
	Initialized();
}

CZombiState::~CZombiState()
{
	Release();
}

void CZombiState::Initialized()
{
	m_eCurState = ZOM_EX_IdleOffset;
	m_ePreState = m_eCurState;
}

HRESULT CZombiState::Late_Initialized()
{

	return S_OK;
}

_int CZombiState::Update_Zombi(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	srand((unsigned int)time(0));

	m_fTime += fTimeDelta;

	if (m_ePreState != m_eCurState)
	{
		m_fTime = 0.f;
		m_ePreState = m_eCurState;
	}

	return S_OK();
}

void CZombiState::Chase_Player(const _float& fTimeDelta)
{
	//_vec3 vLook, vRight, vDir;
	//vDir = m_pPlayerPos - m_pFlamePos;
	//memcpy(&vLook, &m_pTransCom->Get_LookVector(), sizeof(_vec3));
	//memcpy(&vRight, &m_pTransCom->Get_RightVector(), sizeof(_vec3));
	//vDir.Normalize();
	//vRight.Normalize();
	//vLook.Normalize();

	//_float fAngle = vLook.Dot(vDir);
	//if (fAngle < -1.f)
	//	fAngle = -1.f;
	//_float fAngleTmp = XMConvertToDegrees(acosf(fAngle));
	////_vec3 vTemp = vLook.Cross_InputDst(vDir);
	////if (vTemp.y > 0.f)
	////	fAngleTmp *= -1.f;

	//_float fDistance = sqrt(pow(m_pPlayerPos.x - m_pFlamePos.x, 2)
	//	+ pow(m_pPlayerPos.y - m_pFlamePos.y, 2));

	//if (fDistance <= 6.f)
	//	m_bIsInArea = true;
	//	//	m_pTransCom->m_vAngle.y += (fAngleTmp * 5.f * fTimeDelta);
	//else
	//	m_bIsInArea = false;

	//if (m_bIsInArea == true)
	//{
	//	m_fTurnTime += fTimeDelta;

	//	if (false == m_bIsTurn)
	//		m_pTransCom->m_vAngle.y += (fAngleTmp * 5.f * fTimeDelta);

	//	if (false == m_bIsTurn && m_fTurnTime >= 2.f
	//		/*true == m_pTransCom->Chase_Target(m_pPlayerPos, 0.3f * fTimeDelta)*/)
	//	{
	//		m_fTurnTime = 0.f;
	//		m_bIsTurn = true;
	//	}
	//}
	//else
	//	m_bIsHit = false;

	//if (m_bIsTurn == true) 
	//	m_eCurState = CB_FireLoop;
}

void CZombiState::Fire_Attak()
{
	//CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	//pPlayerHP = pPlayer->Get_Transform()->Get_~~HP();

}

_int CZombiState::LateUpdate_Zombi(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	Animation_Test(fTimeDelta, m_pMeshCom);

	return S_OK();
}

void CZombiState::Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom)
{
	switch (m_eCurState)
	{
	case CZombiState::ZOM_BasePose:
		break;
	case CZombiState::ZOM_CB_Active:
		break;
	case CZombiState::ZOM_CB_CombatActive:
		break;
	case CZombiState::ZOM_CB_CombatActive_Ceiling:
		break;
	case CZombiState::ZOM_CB_Idle:
		break;
	case CZombiState::ZOM_CB_IdlePose:
		break;
	case CZombiState::ZOM_DG_GetUpBack:
		break;
	case CZombiState::ZOM_DG_GetUpFront:
		break;
	case CZombiState::ZOM_EX_IdleOffset:
		break;
	case CZombiState::ZOM_EX_IdlePose:
		break;
	case CZombiState::ZOM_EX_Run:
		break;
	case CZombiState::ZOM_EX_WalkSlow:
		break;
	case CZombiState::ZOM_BC_Dead:
		break;
	case CZombiState::ZOM_Base_Pose2:
		break;
	case CZombiState::ZOM_BC_End2:
		break;
	case CZombiState::ZOM_Base_Pose3:
		break;
	case CZombiState::ZOM_BC_End3:
		break;
	default:
		break;
	}
}

void CZombiState::Release()
{
	Safe_Release(m_pTransCom);
}