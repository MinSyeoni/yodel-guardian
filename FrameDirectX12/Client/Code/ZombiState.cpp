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
	m_eCurState = ZOM_CB_CombatActive_Ceiling;
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
	/////////// 방향 고쳐야 함 ////////////////
	_vec3 vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);

	_float fAngle;
	_vec3  vLook, vRight;
	_matrix matRot;

	vChaseDir.y = 0.f;
	vChaseDir.Normalize();

	memcpy(&vLook, &m_pTransCom->m_vDir, sizeof(_vec3));
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
		m_pTransCom->m_vAngle.y += ToRadian(fAngle * 180.f * fTimeDelta);
	}
	else if ((fAngle <= 3.f && fAngle >= -3.f))
	{
		m_bIsTurn = true;
	}
	else if ((fAngle > 15.f || fAngle < -15.f))
	{
		m_bIsTurn = false;
	}

	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vChaseDir, m_fSpeed * fTimeDelta);
}

_bool CZombiState::Check_PlayerRange()
{
	_bool bIsRange = false;

	_vec3 vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);
	vChaseDir.Normalize();
	_float fChaseRange =  vChaseDir.Get_Length();

	if (fChaseRange <= m_fRange)
		bIsRange = true;
	else
		bIsRange = false;

	return bIsRange;
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
	{
		m_fAniDelay = 6000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_CB_CombatActive_Ceiling))
		{
			m_eCurState = ZOM_EX_IdleOffset;
		}
	}
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
		if (Check_PlayerRange())
		{
			m_eCurState = ZOM_EX_Run;
		}
	}
		break;
	case CZombiState::ZOM_EX_IdlePose:
		break;
	case CZombiState::ZOM_EX_Run:
	{
		m_fSpeed = 5.f;

		Chase_Player(fTimeDelta);
	}
		break;
	case CZombiState::ZOM_EX_WalkSlow:
	{
		m_fSpeed = 3.f;

		Chase_Player(fTimeDelta);
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