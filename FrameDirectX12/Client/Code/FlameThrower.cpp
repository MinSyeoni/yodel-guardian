#include "stdafx.h"
#include "FlameThrower.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "LightMgr.h"
#include "FlameBullet.h"
#include <random>
CFlameThrower::CFlameThrower()
{
	Initialized();
}

CFlameThrower::~CFlameThrower()
{
	Release();
}

void CFlameThrower::Initialized()
{
	m_eCurState = CB_Idle;
	m_ePreState = m_eCurState;

}

HRESULT CFlameThrower::Late_Initialized()
{
	
	
	return S_OK;
}

_int CFlameThrower::Update_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	

	m_pTransCom = pTransform;

	m_fTime += fTimeDelta;

	if (m_ePreState != m_eCurState)
	{
		m_fTime = 0.f;
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

void CFlameThrower::Chase_Player(const _float& fTimeDelta)
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

void CFlameThrower::Fire_Attak(const _float& fTimeDelta)
{
	//CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	//pPlayerHP = pPlayer->Get_Transform()->Get_~~HP();
	Attack(fTimeDelta);
	m_bIsHit = true;
}

_int CFlameThrower::LateUpdate_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	Animation_Test(fTimeDelta, m_pMeshCom);

	return S_OK;
}

void CFlameThrower::Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom)
{
	switch (m_eCurState)
	{
	case CFlameThrower::FLAME_IDLE:
		break;
	case CFlameThrower::BC_Start:
		break;
	case CFlameThrower::CB_Enter:
		break;
	case CFlameThrower::CB_Exit:
		break;
	case CFlameThrower::CB_FireLoop:	
	{
		m_fAniDelay = 2500.f;

		Fire_Attak(fTimeDelta);

		//if (!m_bIsFireSound)
		//{
		//	CSoundMgr::Get_Instance()->Play_Effect(L"FlameFire.mp3");
		//	m_bIsFireSound = true;
		//}
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, CB_FireLoop))
			m_eCurState = CB_Idle;
	}
		break;
	case CFlameThrower::CB_FireRecoil:
		break;
	case CFlameThrower::CB_FireStart:
		break;
	case CFlameThrower::CB_Idle: // °¡¸¸È÷ 
	{
		m_bIsFireSound = false;
		m_fAniTime += fTimeDelta;
		m_bIsHit = false;
		m_bIsMovingSound = false;

		int iRandAni = rand() % 3;

		if (iRandAni == 0)
		{
			if (m_fAniTime >= 2.5f)
			{
				m_fAniTime = 0.f;
				m_eCurState = CB_Twitch;
			}
		}
		else
		{
			if (m_fAniTime >= 2.f)
			{
				m_fAniTime = 0.f;
				m_eCurState = CB_FireLoop;
			}
		}
	}
		break;
	case CFlameThrower::CB_Twitch: // Á¤Âû
	{
		m_fAniDelay = 18000.f;

		if (!m_bIsMovingSound)
		{
			CSoundMgr::Get_Instance()->Play_Effect(L"MovingRobot.mp3");
			m_bIsMovingSound = true;
		}

		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, CB_Twitch))
			m_eCurState = CB_Idle;	
	}
		break;
	case CFlameThrower::CB_WalkDown:
		break;
	case CFlameThrower::CB_WalkEast:
		break;
	case CFlameThrower::CB_WalkNorth:
		break;
	case CFlameThrower::CB_WalkSouth:
		break;
	case CFlameThrower::CB_WalkUp:
		break;
	case CFlameThrower::CB_WalkWest:
		break;
	default:
		break;
	}
}

void CFlameThrower::Release()
{
	Safe_Release(m_pTransCom);
}

void CFlameThrower::Attack(const _float& fTimeDelta)
{
	bool bIsFire = false;
	m_fAccFireTime += fTimeDelta;
	if (m_fAccFireTime > 0.1f)
	{
		bIsFire = true;
		m_fAccFireTime = 0.f;
	}

	if (!bIsFire)
		return;
	srand((unsigned int)time(0));
	
		FlameInfo tInfo;
		tInfo.vPos = m_pTransCom->m_vPos;
		tInfo.vPos.y += 8.f;
		_vec3 randDir = _vec3(0.f, 1.f, 0.f);

		if (m_pTransCom->m_vAngle.y == 90)
		{
			randDir.x = 0.f;
			randDir.x = 0.f;
			randDir.y = 0.f;
			randDir.y = 0.f;

			randDir.z = -1.f;
			tInfo.vPos.z -= 5.f;

			randDir.Normalize();
		}
		else if (m_pTransCom->m_vAngle.y == -90)
		{
			randDir.x = 0.f;
			randDir.x = 0.f;
			randDir.y = 0.f;
			randDir.y = 0.f;

			randDir.z = 1.f;
			tInfo.vPos.z += 5.f;

			randDir.Normalize();


		}
		else if (m_pTransCom->m_vAngle.y == 0)
		{
			randDir.z = 0.f;
			randDir.z = 0.f;
			randDir.y = 0.f;
			randDir.y = 0.f;

			randDir.x = 1.f;
			tInfo.vPos.x += 5.f;

			randDir.Normalize();


		}
		else if (m_pTransCom->m_vAngle.y == 180)
		{
			randDir.z = 0.f;
			randDir.z = 0.f;
			randDir.y = 0.f;
			randDir.y = 0.f;

			randDir.x = -1.f;
			tInfo.vPos.x -= 5.f;

			randDir.Normalize();

		}
		
		tInfo.vDir = randDir;
		tInfo.flifeTime = 3;
		tInfo.fSpeed = 5.f;

		CObjectMgr::Get_Instance()->Add_GameObject(L"Layer_GameObject", L"Prototype_FlameBullet", L"FlameBullet", &tInfo);



	
}


