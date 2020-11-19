
#include "stdafx.h"
#include "Dron.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "Player.h"
#include "PlayerStatus.h"
#include "Trigger.h"

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

	if (m_iDrawID == 1)
		m_pTransCom->m_vPos.y = 15.f;
	else if (m_iDrawID == 2)
		m_pTransCom->m_vPos.y = 15.f;
	else if (m_iDrawID == 3)
		m_pTransCom->m_vPos.y = 10.f;

	m_vInitPos = m_pTransCom->m_vPos;

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

	if (m_bIsDronState[2])	// m_bIsHit
		m_eCurState = DRON_EX_IdleSway;

	if (m_eCurState != DRON_EX_IdleNoise)
	{
		Update_DronPos(fTimeDelta);
		Dron_OnTrigger(fTimeDelta);
	}

	return S_OK;
}

void CDron::Update_DronPos(const _float& fTimeDelta)
{
	if (m_iDrawID == 1)
	{
		m_pTransCom->m_vPos.y = 15.f;
		m_fSpeed = 12.f;
		if(!m_bIsTurn)
			m_pTransCom->m_vAngle = _vec3{ 0.f, -100.f, 0.f };
		else
			m_pTransCom->m_vAngle = _vec3{ 0.f, 80.f, 0.f };
	}
	else if (m_iDrawID == 2)
	{
		m_pTransCom->m_vPos.y = 15.f;
		m_fSpeed = 8.f;
		if (!m_bIsTurn)
			m_pTransCom->m_vAngle = _vec3{ 0.f, -100.f, 0.f };
		else
			m_pTransCom->m_vAngle = _vec3{ 0.f, 80.f, 0.f };
	}
	else if (m_iDrawID == 3)
	{
		m_pTransCom->m_vPos.y = 10.f;
		m_fSpeed = 15.f;
		if (!m_bIsTurn)
			m_pTransCom->m_vAngle = _vec3{ 0.f, 80.f, 0.f };
		else
			m_pTransCom->m_vAngle = _vec3{ 0.f, -100.f, 0.f };
	}
}

void CDron::Dron_OnTrigger(const _float& fTimeDelta)
{
	_vec3 vChaseDir = _vec3(0.f, 0.f, 0.f);
	list<CGameObject*>* pTriggerList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Trigger");
	if (pTriggerList != nullptr)
	{
		for (auto& pSrc : *pTriggerList)
		{
			if (CTrigger::TRIGGER_SPHERE == dynamic_cast<CTrigger*>(pSrc)->Get_ColType()
				&& m_iDrawID == dynamic_cast<CTrigger*>(pSrc)->Get_ColID()
				&& (!m_bIsTurn && !m_bIsAlreadyTurn))
			{
				Chase_Rotation(dynamic_cast<CTrigger*>(pSrc)->Get_ColPos(), fTimeDelta);
			}
		}
	}

	_vec3 vInitDir = m_vInitPos - m_pTransCom->m_vPos;
	_float fChaseRange = vInitDir.Get_Length();
	if (m_bIsTurn && !m_bIsAlreadyTurn)
	{
		Chase_Rotation(m_vInitPos, fTimeDelta);
		m_bIsAlreadyTurn = true;
	}
	else if (fChaseRange <= 1.5f && m_bIsTurn && m_bIsAlreadyTurn)
	{
		m_bIsAlreadyTurn = false;
		m_bIsTurn = false;
	}
}

void CDron::MoveByAstar(const _float& fTimeDelta)
{
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	if (pPlayer == nullptr)
		return;
	CTransform* pPlayerTranForm = pPlayer->Get_Transform();
	CNaviMesh* pPlayerNavi = dynamic_cast<CPlayer*>(pPlayer)->Get_Status()->m_pNaviMesh;
	m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pPlayerTranForm->m_vPos, m_pNaviMesh->GetIndex(), pPlayerNavi->GetIndex());
	
	list<Engine::CCell*>& BestLst = m_pAstarCom->GetBestLst();

	if (!BestLst.empty())
	{
		_vec3 vecDir = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

		BestLst.pop_front();
		if (!BestLst.empty())
		{
			_vec3 vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

			vecDir = (vecDir + vecDir2) * 0.5;
			BestLst.pop_front();
			if (!BestLst.empty())
			{
				vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;
				vecDir = (vecDir + vecDir2) * 0.5;
			}
		}

		vecDir.Normalize();
		_vec3 vMovePos;

		vMovePos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta, true);
		if (m_iDrawID == 1)
			vMovePos.y = 15.f;
		else if (m_iDrawID == 2)
			vMovePos.y = 15.f;
		else if (m_iDrawID == 3)
			vMovePos.y = 10.f;

		m_pTransCom->m_vPos = vMovePos;
	}
}

void CDron::Update_DronHP()
{
	if (m_fCurHp <= 0.f)
	{
		if (!m_bIsCreateEffect)
		{
			CObjectMgr::Get_Instance()->Add_GameObject(L"Layer_GameObject", L"Prototype_Effect_DronBomb", L"Effect", &m_pTransCom->m_vPos);
			m_bIsCreateEffect = true;
		}
		m_fCurHp = 0.f;
		m_bIsDronState[2] = false;
		m_eCurState = DRON_DG_Death;
	}

}

void CDron::Chase_Rotation(_vec3 vPos, const _float& fTimeDelta)
{
	m_vChaseDir = (vPos - m_pTransCom->m_vPos);
	_float fAngle;
	_vec3  vLook, vRight;

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
		m_bIsShoot = false;

		if (m_bIsDronState[2])	// hit
			m_eCurState = DRON_EX_IdleSway;

		m_fAniDelay = 20000.f;

		m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_pTransCom->m_vDir, m_fSpeed * fTimeDelta, true);

		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, DRON_EX_IdleHoverTwitch))
		{
			if(Check_PlayerRange(40.f))
				m_eCurState = DRON_EX_IdleNoise;
			else
				m_eCurState = DRON_EX_IdleHoverTwitch;
		}
	}
		break;
	case CDron::DRON_EX_IdleNoise:		// 공격
	{
		m_fAniDelay = 4000.f;
		
		if (m_bIsShoot)
		{
			m_fShootDelay += fTimeDelta;
			if (m_fShootDelay >= 3.f)
			{
				m_bIsShootSound = false;
				m_fShootDelay = 0.f;
				m_bIsShoot = false;
			}
		}

		Chase_Rotation(m_vPlayerPos, fTimeDelta);
		Attak_Player(m_pMeshCom, DRON_EX_IdleNoise);
	}
		break;
	case CDron::DRON_EX_IdleSway:	// 맞을 때
	{
		if (m_bIsDronState[1])
			return;

		m_fAniDelay = 2000.f;

		if (!m_bIsHitSound)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"Machine_1.mp3");
			m_bIsHitSound = true;
		}

		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, DRON_EX_IdleSway))
		{
			m_fHitDamage = rand() % 10 + 10.f;
			m_fCurHp -= m_fHitDamage;
			m_bIsDronState[2] = false;
			m_bIsHitBullet = true;
			m_bIsHitSound = false;

			if (Check_PlayerRange(40.f))
				m_eCurState = DRON_EX_IdleNoise;
			else
				m_eCurState = DRON_EX_IdleHoverTwitch;
		}
	}
		break;
	case CDron::DRON_FlightPose:
		break;
	case CDron::DRON_OLD_Idle:
		break;
	case CDron::DRON_DG_Death:
	{
		m_bIsDronState[2] = false;
		m_fAniDelay = 300.f;
		m_pTransCom->m_vPos.y -= 5.f * fTimeDelta;

		if (m_bIsDeadSound == false)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"DronBomb.mp3");
			m_bIsDeadSound = true;
		}

		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, DRON_DG_Death))
			m_bIsDronState[1] = true;	//dead
	}
		break;
	default:
		break;
	}
}

void CDron::Attak_Player(Engine::CMesh* m_pMeshCom, CDron::DRONSTATE eState)
{
	if (!m_bIsDronState[3])
	{
		MeshInfo tMeshInfo;
		tMeshInfo.Pos = m_pTransCom->m_vPos;
		if (!m_bIsShoot)
		{
			CObjectMgr::Get_Instance()->Add_GameObject(L"Layer_GameObject", L"Prototype_DronBullet", L"DronBullet", &tMeshInfo);
			m_bIsShoot = true;
		}

		if (!m_bIsShootSound)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"DronShoot.mp3");
			m_bIsShootSound = true;
		}

		m_bIsDronState[3] = true;
	}

	if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, eState))
	{
		m_bIsDronState[3] = false;

		if (Check_PlayerRange(40.f))
			m_eCurState = DRON_EX_IdleNoise;
		else
			m_eCurState = DRON_EX_IdleHoverTwitch;
	}
}

void CDron::Release()
{
	Safe_Release(m_pTransCom);
	Safe_Release(m_pNaviMesh);
	Safe_Release(m_pAstarCom);
}