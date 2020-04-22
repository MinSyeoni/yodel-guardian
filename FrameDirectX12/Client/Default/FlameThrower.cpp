#include "stdafx.h"
#include "FlameThrower.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"

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

	//////////////// 테스트 ////////////////////////////////
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	m_pPlayerPos = pPlayer->Get_Transform()->Get_PositionVector();
	m_pFlamePos = m_pTransCom->Get_PositionVector();

	_float fDistance = sqrt(pow(m_pPlayerPos.x - m_pFlamePos.x, 2) + pow(m_pPlayerPos.y - m_pFlamePos.y, 2));
	
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
	//_vec3 vTemp = vLook.Cross_InputDst(vDir);
	////if (vTemp.y > 0.f)
	////	fAngleTmp *= -1.f;


	if (fDistance <= 0.1f)
		m_bIsInArea = true;
	else
		m_bIsInArea = false;

	/////////////////////////////////////////////

	return S_OK();
}

_int CFlameThrower::LateUpdate_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	Animation_Test(fTimeDelta, m_pMeshCom);

	return S_OK();
}

void CFlameThrower::Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom)
{
	m_fAniTime += fTimeDelta;

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
		m_fAniDelay = 50.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, CB_FireLoop)
			&& m_bIsInArea == false)
		{
			m_eCurState = CB_Idle;
			m_fAniTime = 0.f;
		}
		break;
	case CFlameThrower::CB_FireRecoil:
		break;
	case CFlameThrower::CB_FireStart:
		break;
	case CFlameThrower::CB_Idle: // 가만히 
		if (m_bIsInArea == true)
			m_eCurState = CB_FireLoop;
		else
		{
			_float fTime = rand() % 6 + 3.f;
			_float fAngle = rand() % 180 - 90.f;
			m_pTransCom->m_vAngle.y += (fAngle * 1.f * fTimeDelta);

			if (m_fAniTime >= fTime)
			{
				m_eCurState = CB_Twitch;
				m_fAniTime = 0.f;
			}
		}
		break;
	case CFlameThrower::CB_Twitch: // 정찰
		m_fAniDelay = 18000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, CB_Twitch))
			m_eCurState = CB_Idle;
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
