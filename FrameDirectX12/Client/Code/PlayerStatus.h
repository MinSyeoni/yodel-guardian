#pragma once
#include "Player.h"


class CPlayerStatus 
{
public:
	CPlayerStatus();
	~CPlayerStatus();

	_int UpdateState(const _float& fTimeDelta,CTransform* pTranscom);
	_int LateUpdate(const _float&fTimeDelta);
	void SetMesh(CMesh* m_pMesh);
	void KeyInput();
	void StatusUpdate(const _float& fTimeDelta);
	void Rotation(const _float& fTimeDelta);
	void PlayerDirection(const _float&fTimeDelta);
	float Get_Spine() { return m_fSpine; };
public:
	CPlayer::STATE m_eCurState= CPlayer::IDLE;
	CPlayer::STATE m_ePreState = CPlayer::IDLE;


	CTransform* m_pTransCom;
	CMesh*  m_pMesh;
	CBoxCollider* m_pBoxCollider;
	CNaviMesh* m_pNaviMesh;

	float m_fSpine = 0.f;;
	//框流老 规氢 
	_vec3 m_vecMoveDirection = _vec3{ 0.f,0.f,1.0f };
	_vec3 m_vecTargetPos = _vec3{ 0.f,0.f,0.f };
	float m_fSpeed = 5.0f;//加档


};

