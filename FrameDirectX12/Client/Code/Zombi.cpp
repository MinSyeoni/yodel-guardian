#include "stdafx.h"
#include "Zombi.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "Player.h"
#include "PlayerStatus.h"

#include "Shepard.h"
#include "Ken.h"

CZombi::CZombi()
{
	Initialized();
}

CZombi::~CZombi()
{
	Release();
}

void CZombi::Initialized()
{
	m_bIsDeadSound = false;
	m_fCurHp = m_fMaxHp = 100.f;
}

HRESULT CZombi::Late_Initialized()
{
	if (m_iInitAni == 0)	
		m_eCurState = ZOM_DG_GetUpBack;
	else if (m_iInitAni == 1)
		m_eCurState = ZOM_DG_GetUpFront;
	else if (m_iInitAni == 2)
		m_eCurState = ZOM_CB_CombatActive_Ceiling;
	else if (m_iInitAni == 3)
		m_eCurState = ZOM_CB_CombatActive;
	else if (m_iInitAni == 4)
		m_eCurState = ZOM_EX_Run;

	m_ePreState = m_eCurState;

	return S_OK;
}

_int CZombi::Update_Zombi(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	m_vPlayerPos = pPlayer->Get_Transform()->m_vPos;

	CGameObject* pShepard = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Shepard");
	if (pShepard == nullptr)
		return E_FAIL;
	m_vShepardPos = pShepard->Get_Transform()->m_vPos;

	CGameObject* pKen = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Ken");
	if (pKen == nullptr)
		return E_FAIL;
	m_vKenPos = pKen->Get_Transform()->m_vPos;

	// Ã¼·Â 
	Update_ZombiHP();

	if (m_bIsZombiState[2])	// m_bIsHit
		m_eCurState = ZOM_EX_IdleOffset;

	if (!m_bIsIdleSound)
	{
		CSoundMgr::Get_Instance()->Play_Effect(L"ZombiIdle.mp3");
		m_bIsIdleSound = true;
	}

	return S_OK;
}

void CZombi::MoveByAstar(const _float& fTimeDelta)
{
	if (m_iAstarID == 0)
	{
		CGameObject* pNPC1 = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Shepard");
		if (pNPC1 == nullptr)
			return;
		CTransform* pNPC1TranForm = pNPC1->Get_Transform();
		CNaviMesh* pNPC1Navi = dynamic_cast<CShepard*>(pNPC1)->Get_ShepardNavi();
		m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pNPC1TranForm->m_vPos, m_pNaviMesh->GetIndex(), pNPC1Navi->GetIndex());
	}
	else if (m_iAstarID == 1)
	{
		CGameObject* pNPC2 = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Ken");
		if (pNPC2 == nullptr)
			return;
		CTransform* pNPC2TranForm = pNPC2->Get_Transform();
		CNaviMesh* pNPC2Navi = dynamic_cast<CKen*>(pNPC2)->Get_KenNavi();
		m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pNPC2TranForm->m_vPos, m_pNaviMesh->GetIndex(), pNPC2Navi->GetIndex());
	}
	else
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		CTransform* pPlayerTranForm = pPlayer->Get_Transform();
		CNaviMesh* pPlayerNavi = dynamic_cast<CPlayer*>(pPlayer)->Get_Status()->m_pNaviMesh;
		m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pPlayerTranForm->m_vPos, m_pNaviMesh->GetIndex(), pPlayerNavi->GetIndex());
	}

	//
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

		vMovePos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vecDir, fTimeDelta*m_fSpeed);
		m_pTransCom->m_vPos = vMovePos;
	}
}

void CZombi::Update_ZombiHP()
{
	if (m_fCurHp <= 0.f)
	{
		m_fCurHp = 0.f;
		m_bIsZombiState[2] = false;
		m_eCurState = ZOM_BC_Dead;
	}
	if (m_bIsDeadSound == false && m_eCurState == ZOM_BC_Dead)
	{
		CSoundMgr::Get_Instance()->Play_Effect(L"ZombiDead.wav");
		m_bIsDeadSound = true;
	}
}

void CZombi::Chase_Character(_vec3 vPos, const _float& fTimeDelta)
{
	m_vChaseDir = (vPos - m_pTransCom->m_vPos);

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

	if ((fAngle > 3.f || fAngle < -3.f) && !m_bIsZombiState[0])
	{
		m_pTransCom->m_vAngle.y += ToRadian(fAngle * 180.f * fTimeDelta);
	}
	else if ((fAngle <= 3.f && fAngle >= -3.f))
	{
		m_bIsZombiState[0] = true;
	}
	else if ((fAngle > 15.f || fAngle < -15.f))
	{
		m_bIsZombiState[0] = false;
	}

	m_pTransCom->m_vDir = m_vChaseDir;
//	m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &m_vChaseDir, m_fSpeed * fTimeDelta);
}

_bool CZombi::Check_CharacterRange(_float fRange)
{
	_bool bIsRange = false;

	_vec3 vChaseDir = _vec3(0.f, 0.f, 0.f);

	if(m_iAstarID == 0)
		vChaseDir = (m_vShepardPos - m_pTransCom->m_vPos);
	else if(m_iAstarID == 1)
		vChaseDir = (m_vKenPos - m_pTransCom->m_vPos);
	else
		vChaseDir = (m_vPlayerPos - m_pTransCom->m_vPos);

	_float fChaseRange = vChaseDir.Get_Length();

	if (fChaseRange <= fRange)
		bIsRange = true;
	else
		bIsRange = false;

	return bIsRange;
}

_int CZombi::LateUpdate_Zombi(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{

	Animation_Test(fTimeDelta, m_pMeshCom);

	return S_OK;
}

void CZombi::Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom)
{
	switch (m_eCurState)
	{
	case CZombi::ZOM_BasePose:
		break;
	case CZombi::ZOM_CB_Active:
		break;
	case CZombi::ZOM_CB_CombatActive:
	{
		m_fAniDelay = 12000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_CB_CombatActive))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombi::ZOM_CB_CombatActive_Ceiling:
	{
		m_fAniDelay = 10000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_CB_CombatActive_Ceiling))
			m_eCurState = ZOM_EX_IdleOffset;
	}
		break;
	case CZombi::ZOM_CB_Idle:
		break;
	case CZombi::ZOM_CB_IdlePose:
		break;
	case CZombi::ZOM_DG_GetUpBack:
	{
		m_fAniDelay = 20000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_DG_GetUpBack))
			m_eCurState = ZOM_EX_IdleOffset;
	}
	break;
	case CZombi::ZOM_DG_GetUpFront:
	{
		m_fAniDelay = 20000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_DG_GetUpFront))
			m_eCurState = ZOM_EX_IdleOffset;
	}
	break;
	case CZombi::ZOM_EX_IdleOffset:
	{
		if (m_bIsZombiState[2])	// hit
		{
			if (!m_bIsHitSound)
			{
				CSoundMgr::Get_Instance()->Play_Effect(L"ZombiHit.wav");
				m_bIsHitSound = true;
			}

			m_fSpeed = 15.f;
		
			m_pTransCom->m_vPos = m_pNaviMesh->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(m_pTransCom->m_vDir * -1), m_fSpeed * fTimeDelta);
			
			m_fAniDelay = 300.f;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_IdleOffset))
			{
				m_fCurHp -= m_fHitDamage;
				m_bIsZombiState[2] = false;
			}
		}
		else
		{
			m_fAniDelay = 6000.f;
			if (Check_CharacterRange(100.f))
			{
				if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_IdleOffset))
					m_eCurState = ZOM_EX_Run;
			}
			else
			{
				if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_IdleOffset))
					m_eCurState = ZOM_EX_WalkSlow;
			}
		}
	}
	break;
	case CZombi::ZOM_EX_IdlePose:
		break;
	case CZombi::ZOM_EX_Run:
	{
		m_fSpeed = 5.f;
		
		m_fAniDelay = 2000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_Run))
		{
			if (Check_CharacterRange(12.f))
			{
				int iRandAni = rand() % 2;
				if (iRandAni == 0)
					m_eCurState = ZOM_LEFT_ATK;
				else if (iRandAni == 1)
					m_eCurState = ZOM_RIGHT_ATK;
			}
			else
			{
				if (m_iAstarID == 0)
					Chase_Character(m_vShepardPos, fTimeDelta);
				else if (m_iAstarID == 1)
					Chase_Character(m_vKenPos, fTimeDelta);
				else
					Chase_Character(m_vPlayerPos, fTimeDelta);

				MoveByAstar(fTimeDelta);
			}
		}
	}
		break;
	case CZombi::ZOM_EX_WalkSlow:
	{
		m_fSpeed = 5.f;

		if (m_iAstarID == 0)
			Chase_Character(m_vShepardPos, fTimeDelta);
		else if (m_iAstarID == 1)
			Chase_Character(m_vKenPos, fTimeDelta);
		else
			Chase_Character(m_vPlayerPos, fTimeDelta);

		MoveByAstar(fTimeDelta);

		if (Check_CharacterRange(100.f))
		{
			m_fAniDelay = 2000.f;
			if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_EX_WalkSlow))
				m_eCurState = ZOM_EX_Run;
		}
	}
		break;
	case CZombi::ZOM_BC_Dead:
	{
		m_bIsZombiState[2] = false;
		m_fAniDelay = 6500.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, ZOM_BC_Dead))
			m_bIsZombiState[1] = true;	//dead
	}
		break;
	case CZombi::ZOM_Base_Pose2:
		break;
	case CZombi::ZOM_LEFT_ATK:
	{
		m_fAniDelay = 6000.f;

		if (!m_bIsAtkSound)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"ZombiAtkL.wav");
			m_bIsAtkSound = true;
		}

		Attak_Player(m_pMeshCom, ZOM_LEFT_ATK);
	}
		break;
	case CZombi::ZOM_Base_Pose3:
		break;
	case CZombi::ZOM_RIGHT_ATK:
	{
		m_fAniDelay = 6000.f;

		if (!m_bIsAtkSound)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"ZombiAtkR.wav");
			m_bIsAtkSound = true;
		}

		Attak_Player(m_pMeshCom, ZOM_RIGHT_ATK);
	}
		break;
	default:
		break;
	}
}

void CZombi::Attak_Player(Engine::CMesh* m_pMeshCom, CZombi::ZOMBISTATE eState)
{
	if (!m_bIsZombiState[3])
	{
		m_bIsZombiState[3] = true;
		m_fAtkDamage = rand() % 15 + 15.f;
	}
	
	if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, eState))
	{
		int iRandAni = rand() % 2;
		m_bIsZombiState[3] = false;
		m_bIsAtkSound = false;

		if (Check_CharacterRange(12.f))
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

void CZombi::Release()
{
	Safe_Release(m_pTransCom);
	Safe_Release(m_pNaviMesh);
	Safe_Release(m_pAstarCom);
}