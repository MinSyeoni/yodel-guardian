#pragma once
#include "Player.h"

class CDynamicCamera;
class CPlayerStatus 
{
public:
	enum EQUIPTYPE { NONE, RIFLE };


public:
	CPlayerStatus();
	~CPlayerStatus();
	void LateInit();
	_int UpdateState(const _float& fTimeDelta,CTransform* pTranscom);
	_int LateUpdate(const _float&fTimeDelta);
	void SetMesh(CMesh* m_pMesh);
	void KeyInput();
	void WeaponChange();
	void StatusUpdate(const _float& fTimeDelta);
	void Rotation(const _float& fTimeDelta);
	void PlayerDirection(const _float&fTimeDelta);
	void AttackCheck();
	float Get_Spine() { return m_fSpine; };
public:
	CPlayer::STATE m_eCurState= CPlayer::RIFLEIDLE;
	CPlayer::STATE m_ePreState = CPlayer::RIFLEIDLE;

	CPlayer::STATE m_eLegState = CPlayer::RIFLEIDLE;

	CTransform* m_pTransCom;
	CMesh*  m_pMesh;
	CBoxCollider* m_pBoxCollider;
	CNaviMesh* m_pNaviMesh;

	float m_fSpine = 0.f;;
	//움직일 방향 
	_vec3 m_vecMoveDirection = _vec3{ 0.f,0.f,1.0f };
	_vec3 m_vecTargetPos = _vec3{ 0.f,0.f,0.f };
	float m_fSpeed = 5.0f;//속도

	_bool m_bIsShoot = false;

	EQUIPTYPE m_eEquip = NONE;


	_bool m_bIsInit = false;
	//카메라
	CDynamicCamera* m_pCamera;


};

