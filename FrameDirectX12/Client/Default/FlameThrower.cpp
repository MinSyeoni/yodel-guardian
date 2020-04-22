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
	m_fTime += fTimeDelta;

	if (m_ePreState != m_eCurState)
	{
		m_fTime = 0.f;
		m_ePreState = m_eCurState;
	}

	// 테스트
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	m_pPlayerPos = pPlayer->Get_Transform()->Get_PositionVector();
	m_pFlamePos = m_pTransCom->Get_PositionVector();

	_float fDisTmp = sqrt(pow(m_pPlayerPos.x - m_pFlamePos.x, 2) + pow(m_pPlayerPos.y - m_pFlamePos.y, 2));
	
	_vec3 vLook, vRight, vDir;
	vDir = m_pPlayerPos - m_pFlamePos;
	memcpy(&vLook, &m_pTransCom->Get_LookVector(), sizeof(_vec3));
	memcpy(&vRight, &m_pTransCom->Get_RightVector(), sizeof(_vec3));
	vDir.Normalize();
	vRight.Normalize();
	vLook.Normalize();

	//if (fDisTmp <= 4.f)
	//{
		
	//	m_pTransCom->Chase_Target(m_pPlayerPos, fTimeDelta);
	//}

	//XMConvertToDegrees(acosf(vDirection.Dot(vLook)));
	
	_float fAngle = vLook.Dot(vDir);
	if (fAngle < -1.f)
		fAngle = -1.f;
	_float fAngleTmp = XMConvertToDegrees(acosf(fAngle));
	_vec3 vTemp;
	//D3DXVec3Cross(&vTemp, &m_vDirection, &vDirTemp);
	vTemp = vLook.Cross_InputDst(vDir);
	if (vTemp.y > 0.f)
		fAngleTmp *= -1.f;
	m_pTransCom->m_vAngle.y += fAngleTmp * 5.f * fTimeDelta;

	/*

		m_pPlayerTransCom->Get_Info(Engine::INFO_POS, &m_vPlayerPos);
	m_pPlayerTransCom->Get_Info(Engine::INFO_LOOK, &m_vPlayerDir);
	D3DXVec3Normalize(&m_vPlayerDir, &m_vPlayerDir);

	m_pTransCom->Get_Info(Engine::INFO_POS, &m_vPos);
	m_pTransCom->Get_Info(Engine::INFO_LOOK, &m_vDirection);
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	m_vAttackDir = m_vPos - m_vPlayerPos;
	D3DXVec3Normalize(&m_vAttackDir, &m_vAttackDir);
	*/

	/*
		_vec3 vDirTemp = *vTargetPos - m_vPos;

	D3DXVec3Normalize(&vDirTemp, &vDirTemp);

	float fAngleTemp = D3DXVec3Dot(&m_vDirection, &vDirTemp);

	if(fAngleTemp < -1.f)
		fAngleTemp = -1.f;

	float fChaseAngle = D3DXToDegree(acosf(fAngleTemp));
	if (FP_NAN == fpclassify(fChaseAngle))
		int a = 0;

	_vec3	vTemp;
	D3DXVec3Cross(&vTemp, &m_vDirection, &vDirTemp);
	
	if (vTemp.y > 0.f)
		fChaseAngle *= -1.f;

	return fChaseAngle;
	*/

	/*
					m_fChaseAngle = Chase_Target(&m_vPlayerPos);
				m_pTransCom->Rotation(Engine::ROT_Y, D3DXToRadian(m_fChaseAngle * fTimeDelta));
	*/

	//		*(((_float*)&m_vAngle) + eType) += fAngle;
	return S_OK();
}

_int CFlameThrower::LateUpdate_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom)
{
	srand((unsigned int)time(0));

//	Animation_Test(fTimeDelta, m_pMeshCom);

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
		break;
	case CFlameThrower::CB_FireRecoil:
		break;
	case CFlameThrower::CB_FireStart:
		break;
	case CFlameThrower::CB_Idle: // 가만히 
		if (m_fAniTime >= 4.f)
		{
			m_eCurState = CB_Twitch;
			m_fAniTime = 0.f;
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
