#pragma once
#include "Player.h"


class CPlayerStatus
{
public:
	CPlayerStatus();
	~CPlayerStatus();

	_int UpdateState(const _float& fTimeDelta,CTransform* pTranscom);

	void KeyInput();
	void StatusUpdate(const _float& fTimeDelta);
	void Rotation(const _float& fTimeDelta);
	void PlayerDirection();
public:
	CPlayer::STATE m_eCurState= CPlayer::IDLE;
	CPlayer::STATE m_ePreState = CPlayer::IDLE;
	CTransform* m_pTransCom;

	//框流老 规氢 
	_vec3 m_vecMoveDirection = _vec3{ 0.f,0.f,1.0f };
	_vec3 m_vecTargetPos = _vec3{ 0.f,0.f,0.f };
	float m_fSpeed = 5.0f;//加档


};

