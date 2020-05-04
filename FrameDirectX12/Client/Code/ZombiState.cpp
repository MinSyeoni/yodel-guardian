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
	srand((unsigned int)time(0));

	int iRandAni = rand() % 4;

	if(iRandAni == 0)
		m_eCurState = ZOM_CB_CombatActive_Ceiling;
	else if (iRandAni == 1)
		m_eCurState = ZOM_CB_CombatActive;
	else if (iRandAni == 2)
		m_eCurState = ZOM_DG_GetUpBack;
	else if (iRandAni == 3)
		m_eCurState = ZOM_DG_GetUpFront;

	m_ePreState = m_eCurState;

	m_fCurHp = m_fMaxHp = 100.f;
}

HRESULT CZombiState::Late_Initialized()
{
	srand((unsigned int)time(0));

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
	m_vPlayerPos = pPlayer->Get_Transform()->m_vPos;

	// 체력 
	if (m_fCurHp <= 0.f)
	{
		m_fCurHp = 0.f;
		m_eCurState = ZOM_BC_Dead;
	}

	// 맞았는지 임시임시 플레이어에서 SetIsHit 해주세
	if (Engine::CDirectInput::Get_Instance()->Mouse_KeyDown(DIM_LB))
		m_bIsHit = true;	

	if (m_bIsHit)
		m_eCurState = ZOM_EX_IdleOffset;

	return S_OK();
}

void CZombiState::Chase_Player(const _float& fTimeDelta)
{
	/////////// 방향 고쳐야 함 ////////////////
	m_vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);

	_float fAngle;
	_vec3  vLook, vRight;
	_matrix matRot;

	m_vChaseDir.y = 0.f;
	m_vChaseDir.Normalize();

	memcpy(&vLook, &m_pTransCom->Get_LookVector(), sizeof(_vec3));
	memcpy(&vRight, &m_pTransCom->Get_RightVector(), sizeof(_vec3));
	vRight.Normalize();
	vLook.Normalize();

	if (90.f >= XMConvertToDegrees(acosf(m_vChaseDir.Dot(vRight))))
	{
		fAngle = XMConvertToDegrees(acosf(m_vChaseDir.Dot(vLook)));
	}
	else
	{
		fAngle = -XMConvertToDegrees(acosf(m_vChaseDir.Dot(vLook)));
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

	m_pTransCom->m_vDir = m_vChaseDir;
	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_vChaseDir, m_fSpeed * fTimeDelta);
}

_bool CZombiState::Check_PlayerRange(_float fRange)
{
	_bool bIsRange = false;

	_vec3 vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);
	_float fChaseRange = vChaseDir.Get_Length();

	if (fChaseRange <= fRange)
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
	{
		m_fAniDelay = 10000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_CB_CombatActive))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombiState::ZOM_CB_CombatActive_Ceiling:
	{
		m_fAniDelay = 12000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_CB_CombatActive_Ceiling))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombiState::ZOM_CB_Idle:
		break;
	case CZombiState::ZOM_CB_IdlePose:
		break;
	case CZombiState::ZOM_DG_GetUpBack:
	{
		m_fAniDelay = 24000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_DG_GetUpBack))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombiState::ZOM_DG_GetUpFront:
	{
		m_fAniDelay = 24000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_DG_GetUpFront))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombiState::ZOM_EX_IdleOffset:
	{
		_float fRandDamage = rand() % 15 + 15.f;
		if (m_bIsHit)
		{
			m_fSpeed = 10.f;
			m_fCurHp -= fRandDamage;
			m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &(m_pTransCom->m_vDir * -1), m_fSpeed * fTimeDelta);
			
			m_fAniDelay = 200.f;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_IdleOffset))			
				m_bIsHit = false;
		}

		if (Check_PlayerRange(100.f))
		{
			m_fAniDelay = 6000.f;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_IdleOffset))
				m_eCurState = ZOM_EX_Run;
		}		
	}
		break;
	case CZombiState::ZOM_EX_IdlePose:
		break;
	case CZombiState::ZOM_EX_Run:
	{
		m_fSpeed = 7.f;

		if (Check_PlayerRange(8.f))
		{
			m_fAniDelay = 2000.f;
			int iRandAni = rand() % 2;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_Run))
			{
				if(iRandAni == 0)
					m_eCurState = ZOM_LEFT_ATK;
				else if(iRandAni == 1)
					m_eCurState = ZOM_RIGHT_ATK;
			}
		}
		else
		{
			Chase_Player(fTimeDelta);
		}
	}
		break;
	case CZombiState::ZOM_EX_WalkSlow:
	{
		m_fSpeed = 2.f;

		m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta);

		if (Check_PlayerRange(100.f))
		{
			m_fAniDelay = 2000.f;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_WalkSlow))
				m_eCurState = ZOM_EX_Run;
		}
	}
		break;
	case CZombiState::ZOM_BC_Dead:
	{
		m_fAniDelay = 6500.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_BC_Dead))
			m_bIsDead = true;
	}
		break;
	case CZombiState::ZOM_Base_Pose2:
		break;
	case CZombiState::ZOM_LEFT_ATK:
	{
		m_fAniDelay = 6000.f;

		Attak_Player(m_pMeshCom, ZOM_LEFT_ATK);
	}
		break;
	case CZombiState::ZOM_Base_Pose3:
		break;
	case CZombiState::ZOM_RIGHT_ATK:
	{
		m_fAniDelay = 6000.f;

		Attak_Player(m_pMeshCom, ZOM_RIGHT_ATK);
	}
		break;
	default:
		break;
	}
}

void CZombiState::Attak_Player(Engine::CMesh* m_pMeshCom, CZombiState::ZOMBISTATE eState)
{
	if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, eState))
	{
		int iRandAni = rand() % 2;

		if (Check_PlayerRange(10.f))
		{
			if (iRandAni == 0)
				m_eCurState = ZOM_LEFT_ATK;
			else if (iRandAni == 1)
				m_eCurState = ZOM_RIGHT_ATK;
		}
		else
			m_eCurState = ZOM_EX_Run;
	}
}

void CZombiState::Release()
{
	Safe_Release(m_pTransCom);
	Safe_Release(m_pNaviMesh);
}