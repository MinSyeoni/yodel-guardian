#include "stdafx.h"
#include "Dron.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "DirectSound.h"
#include "Player.h"
#include "PlayerStatus.h"

CDron::CDron()
{
	Initialized();
}

CDron::~CDron()
{
	Release();
}

void CDron::Initialized()
{
	m_bIsDeadSound = false;
	m_fCurHp = m_fMaxHp = 100.f;
}

HRESULT CDron::Late_Initialized()
{
	m_eCurState = DRON_EX_IdleHoverTwitch;
	m_ePreState = m_eCurState;

	return S_OK;
}

_int CDron::Update_Dron(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	srand((unsigned int)time(0));

	m_fTime += fTimeDelta;

	if (m_ePreState != m_eCurState)
	{
		m_fTime = 0.f;
		m_ePreState = m_eCurState;
	}
	
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	if (pPlayer == nullptr)
		return E_FAIL;

	m_vPlayerPos = pPlayer->Get_Transform()->m_vPos;

	// 체력 
	Update_DronHP();

	//m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vecDir, fTimeDelta * m_fSpeed);

	if (m_bIsDronState[2])	// m_bIsHit
		m_eCurState = DRON_EX_IdleSway;

	return S_OK;
}

void CDron::MoveByAstar(const _float& fTimeDelta)
{
	//CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	//if (pPlayer == nullptr)
	//	return;

	//CTransform* pPlayerTranForm = pPlayer->Get_Transform();
	//CNaviMesh* pPlayerNavi = dynamic_cast<CPlayer*>(pPlayer)->Get_Status()->m_pNaviMesh;

	//m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pPlayerTranForm->m_vPos, m_pNaviMesh->GetIndex(), pPlayerNavi->GetIndex());

	//list<Engine::CCell*>& BestLst = m_pAstarCom->GetBestLst();

	//if (!BestLst.empty())
	//{
	//	_vec3 vecDir = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

	//	BestLst.pop_front();
	//	if (!BestLst.empty())
	//	{
	//		_vec3 vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

	//		vecDir = (vecDir + vecDir2) * 0.5;
	//		BestLst.pop_front();
	//		if (!BestLst.empty())
	//		{
	//			vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;
	//			vecDir = (vecDir + vecDir2) * 0.5;
	//		}
	//	}

	//	vecDir.Normalize();
	//	_vec3 vMovePos;

	//	vMovePos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vecDir, fTimeDelta * m_fSpeed);

	//	m_pTransCom->m_vPos = vMovePos;
	//}
}

void CDron::Update_DronHP()
{
	if (m_fCurHp <= 0.f)
	{
		m_fCurHp = 0.f;
		m_bIsDronState[2] = false;
		m_eCurState = DRON_DG_Death;
	}
	//if (m_bIsDeadSound == false && m_eCurState == ZOM_BC_Dead)
	//{
	//	m_bIsDeadSound = true;
	//	CDirectSound::Get_Instance()->PlayDirectSoundFile(L"ZombiDead");
	//}
}

void CDron::Chase_Player(const _float& fTimeDelta)
{
	m_vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);

	_float fAngle;
	_vec3  vLook, vRight;

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

	if ((fAngle > 3.f || fAngle < -3.f) && !m_bIsDronState[0])
	{
		m_pTransCom->m_vAngle.y += ToRadian(fAngle * 180.f * fTimeDelta);
	}
	else if ((fAngle <= 3.f && fAngle >= -3.f))
	{
		m_bIsDronState[0] = true;
	}
	else if ((fAngle > 15.f || fAngle < -15.f))
	{
		m_bIsDronState[0] = false;
	}

	m_pTransCom->m_vDir = m_vChaseDir;
//	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_vChaseDir, m_fSpeed * fTimeDelta);
}

_bool CDron::Check_PlayerRange(_float fRange)
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

_int CDron::LateUpdate_Dron(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	Animation_Test(fTimeDelta, m_pMeshCom);

	return S_OK;
}

void CDron::Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom)
{
	switch (m_eCurState)
	{
	case CDron::DRON_BasePose:
	{
		m_eCurState = DRON_CB_WalkUp;
	}
		break;
	case CDron::DRON_CB_WalkDown:
		break;
	case CDron::DRON_CB_WalkEast:
		break;
	case CDron::DRON_CB_WalkNorth:
		break;
	case CDron::DRON_CB_WalkSouth:
		break;
	case CDron::DRON_CB_WalkUp:
	{
		m_fSpeed = 3.5f;

		if (Check_PlayerRange(8.f))
		{
			m_fAniDelay = 2000.f;
			int iRandAni = rand() % 2;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, DRON_CB_WalkUp))
				m_eCurState = DRON_EX_IdleNoise;			
		}
		else
		{
			Chase_Player(fTimeDelta);
		//	MoveByAstar(fTimeDelta);
		}
	}
	break;
	case CDron::DRON_CB_WalkWest:
		break;
	case CDron::DRON_DG_Front:
		break;
	case CDron::DRON_DG_Idle:
		break;
	case CDron::DRON_EX_IdleHover:
		break;
	case CDron::DRON_EX_IdleHoverTwitch:	// 정찰
	{
		if (Check_PlayerRange(8.f))
		{
			m_fAniDelay = 4000.f;

			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, DRON_EX_IdleHoverTwitch))
				m_eCurState = DRON_EX_IdleNoise;
		}
		else
			Chase_Player(fTimeDelta);
	}
		break;
	case CDron::DRON_EX_IdleNoise:		// 공격
	{
		m_fAniDelay = 6000.f;
		Attak_Player(m_pMeshCom, DRON_EX_IdleNoise);
	}
		break;
	case CDron::DRON_EX_IdleSway:
		break;
	case CDron::DRON_FlightPose:
		break;
	case CDron::DRON_OLD_Idle:
		break;
	case CDron::DRON_DG_Death:
		break;
	default:
		break;
	}
}

void CDron::Attak_Player(Engine::CMesh* m_pMeshCom, CDron::DRONSTATE eState)
{
	if (!m_bIsDronState[3])
	{
		m_bIsDronState[3] = true;
		m_fAtkDamage = rand() % 15 + 15.f;
	}
	
	if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, eState))
	{
	//	int iRandAni = rand() % 2;
		m_bIsDronState[3] = false;

		if (Check_PlayerRange(8.f))
			m_eCurState = DRON_EX_IdleNoise;
	}
}

void CDron::Release()
{
	Safe_Release(m_pTransCom);
	Safe_Release(m_pNaviMesh);
//	Safe_Release(m_pAstarCom);
}