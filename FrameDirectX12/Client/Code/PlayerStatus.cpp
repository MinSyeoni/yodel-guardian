#include "stdafx.h"
#include "PlayerStatus.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
CPlayerStatus::CPlayerStatus()
{
}

CPlayerStatus::~CPlayerStatus()
{
}

_int CPlayerStatus::UpdateState(const _float & fTimeDelta, CTransform * pTranscom)
{

	if(pTranscom!=nullptr)
	m_pTransCom = pTranscom;


	KeyInput();
	Rotation(fTimeDelta);
	StatusUpdate(fTimeDelta);
	PlayerDirection();

	m_ePreState = m_eCurState;

	return NO_EVENT;
}

void CPlayerStatus::KeyInput()
{
	m_eCurState = CPlayer::IDLE;

	if (KEY_PRESSING(DIKEYBOARD_W))
	{
		m_eCurState = CPlayer::WALK;
		m_fSpeed = 5.f;
	}

		


}

void CPlayerStatus::StatusUpdate(const _float & fTimeDelta)
{

	if (m_pTransCom == nullptr)
		return;

	switch (m_eCurState)
	{
	case CPlayer::WALK:
		m_pTransCom->m_vPos += m_pTransCom->m_vDir*m_fSpeed*fTimeDelta;



	default:
		break;
	}



}

void CPlayerStatus::Rotation(const _float& fTimeDelta)
{
	
	m_pTransCom->Chase_Target(m_vecTargetPos, fTimeDelta*0.05f);
}

void CPlayerStatus::PlayerDirection()
{
	if (m_eCurState == CPlayer::WALK)
	{
		_matrix matViewInv = CGraphicDevice::Get_Instance()->GetViewMatrix();

		matViewInv = XMMatrixInverse(nullptr, matViewInv);

		_vec3 vCamDir;
		memcpy(&vCamDir, &matViewInv._31, sizeof(_vec3));

		vCamDir.y = 0.f;
		m_pTransCom->m_vDir = vCamDir;
		m_pTransCom->m_vDir.Normalize();
	
	}

	m_vecTargetPos = m_pTransCom->m_vPos + (m_pTransCom->m_vDir)*30.f;
}
