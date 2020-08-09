#include "stdafx.h"
#include "FlameThrower.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"

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
	srand((unsigned int)time(0));

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

void CFlameThrower::Fire_Attak()
{
	//CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	//pPlayerHP = pPlayer->Get_Transform()->Get_~~HP();

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
		m_fAniDelay = 2500.f;
		Fire_Attak();
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, CB_FireLoop))
		{
			m_eCurState = CB_Idle;
		}		
		break;
	case CFlameThrower::CB_FireRecoil:
		break;
	case CFlameThrower::CB_FireStart:
		break;
	case CFlameThrower::CB_Idle: // °¡¸¸È÷ 
	{
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
			m_bIsMovingSound = true;
			CSoundMgr::Get_Instance()->Play_Effect(L"MovingRobot.mp3");
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