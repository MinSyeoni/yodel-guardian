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

	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	m_vPlayerPos = pPlayer->Get_Transform()->Get_PositionVector();

	return S_OK();
}

void CZombiState::Chase_Player(const _float& fTimeDelta)
{
	_vec3 vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);

	_float fAngle;
	_vec3  vLook, vRight;
	_matrix matRot;

	vChaseDir.y = 0.f;
	vChaseDir.Normalize();

	memcpy(&vLook, &m_pTransCom->Get_LookVector(), sizeof(_vec3));
	memcpy(&vRight, &m_pTransCom->Get_RightVector(), sizeof(_vec3));
	vRight.Normalize();
	vLook.Normalize();

	if (90.f >= XMConvertToDegrees(acosf(vChaseDir.Dot(vRight))))
	{
		vChaseDir.Normalize();
		fAngle = XMConvertToDegrees(acosf(vChaseDir.Dot(vLook)));
	}
	else
	{
		vChaseDir.Normalize();
		fAngle = -XMConvertToDegrees(acosf(vChaseDir.Dot(vLook)));
	}

	if ((fAngle > 3.f || fAngle < -3.f) && !m_bIsTurn)
	{
		m_pTransCom->m_vAngle.y += fAngle * 10.f * fTimeDelta;
	}
	else if ((fAngle <= 3.f && fAngle >= -3.f))
	{
		m_bIsTurn = true;
	}
	else if ((fAngle > 15.f || fAngle < -15.f))
	{
		m_bIsTurn = false;
	}

	//m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vChaseDir, m_fSpeed * fTimeDelta);

	/*
		_float fAngle;

	_vec3  vLook, vRight, vDirection;

	_matrix matRot;

	vDirection = *pTargetPos - m_vInfo[STATE_POSITION];
	vDirection.y = 0.f;

	//내각 오른쪽에 있다
	if (90.f >= D3DXToDegree(acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDirection, &vDirection),
		D3DXVec3Normalize(&vRight, &m_vInfo[STATE_RIGHT])))))
	{
		fAngle = D3DXToDegree(acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDirection, &vDirection),
			D3DXVec3Normalize(&vLook, &m_vInfo[STATE_LOOK]))));
	}
	//둔각 왼쪽에 있다
	else
	{
		fAngle = -D3DXToDegree(acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDirection, &vDirection),
			D3DXVec3Normalize(&vLook, &m_vInfo[STATE_LOOK]))));
	}

	if ((fAngle > 3.f || fAngle < -3.f) && !m_bIsTurn)
	{
		Set_Rotation_Axis(ROT_Y, D3DXToRadian((fAngle * 180.f * fTimeDelta)));		
	}
	else if ((fAngle <= 3.f && fAngle >= -3.f))
	{
		m_bIsTurn = true;
	}
	else if((fAngle > 15.f || fAngle < -15.f))
	{
		m_bIsTurn = false;
	}
	*/


	/*
		_matrix	 matRotY;

	_vec3  vDirection, vOriginDir;

	vDirection  = vOriginDir = *pTargetPos - m_vInfo[STATE_POSITION];
	
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(30.f));
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matRotY);

	Chase_Target(pTargetPos, fTimeDelta);

	return (*D3DXVec3Normalize(&vDirection, &vDirection));
	*/
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
	{
		m_fAniTime += fTimeDelta;

		_int iRandAni = rand() % 3;

		//if (iRandAni == 0)
		//{
			if (m_fAniTime >= 4.f)
			{
				m_fAniTime = 0.f;
				m_eCurState = ZOM_EX_Run;
			}
		//}
		//else if (iRandAni == 1)
		//{
		//	if (m_fAniTime >= 3.f)
		//	{
		//		m_fAniTime = 0.f;
		//		m_eCurState = ZOM_EX_WalkSlow;
		//	}
		//}


	}
		break;
	case CZombiState::ZOM_EX_IdlePose:
		break;
	case CZombiState::ZOM_EX_Run:
	{
		m_fSpeed = 5.f;

		Chase_Player(fTimeDelta);
	//	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(
	//		&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta);
	}
		break;
	case CZombiState::ZOM_EX_WalkSlow:
	{
		m_fSpeed = 5.f;

		m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(
			&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta);

	}
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
	Safe_Release(m_pNaviMesh);
}