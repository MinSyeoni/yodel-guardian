#pragma once
#include "Player.h"

class CDynamicCamera;
class CPlayerStatus
{
public:
	enum EQUIPTYPE { NONE, RIFLE, SNIPER };

	enum CutSceneType{NOSCENE,FIRSTSCENE,SECONDSCENE};
public:
	CPlayerStatus();
	~CPlayerStatus();
	void LateInit();

	_int UpdateState(const _float& fTimeDelta, CTransform* pTranscom);
	_int LateUpdate(const _float& fTimeDelta);
	void SetMesh(CMesh* m_pMesh);
	void KeyInput();
	void WeaponChange();
	bool WeaponStateCheck(EQUIPTYPE eType);

	void CollisionWithObject(const _float& fTimeDelta);//오브젝트와 충돌;
	void CutSceneCheck();
	void ShootingCheck();

	void StatusUpdate(const _float& fTimeDelta);
	void Rotation(const _float& fTimeDelta);
	void PlayerDirection(const _float& fTimeDelta);
	void AttackCheck(const _float& fTimeDelta);
	void DamageByMonster(const _float& fTimeDelta);
	float Get_Spine() { return m_fSpine; };
	void CheckAim();
	void ReloadCheck();
	void CheckSniping();

	void CameraShakeCheck();

	void WeaponSwap();
public:
	CPlayer::STATE m_eCurState = CPlayer::RIFLEIDLE;
	CPlayer::STATE m_ePreState = CPlayer::RIFLEIDLE;

	CPlayer::STATE m_eLegState = CPlayer::RIFLEIDLE;

	_matrix* m_matChestOffset;
	_matrix* m_matChest;

	CTransform* m_pTransCom;
	CMesh* m_pMesh;
	CBoxCollider* m_pBoxCollider;
	CSphereCollider* m_pSphereCollider;

	CNaviMesh* m_pNaviMesh;

	float m_fSpine = 0.f;;
	//움직일 방향 
	_vec3 m_vecMoveDirection = _vec3{ 0.f,0.f,1.0f };
	_vec3 m_vecTargetPos = _vec3{ 0.f,0.f,0.f };
	float m_fSpeed = 5.0f;//속도

	_float m_fShootingTime = 0;//총프레임제한
	_bool m_bIsShoot = false;//총쏘는중
	EQUIPTYPE m_eEquip = NONE;

	EQUIPTYPE m_eSwapEquip = NONE;
	bool      m_bIsSwapingWeapon = false;//무기든상태에서 다른무기로 바꿀때.


	_bool m_bIshit = false;
	_float m_fhitCool = 0.f;
	_uint m_uiHp = 314;

	bool m_bIsZoom = false;
	_bool m_bIsInit = false;
	//카메라
	CDynamicCamera* m_pCamera;

	_bool m_bIsKeyLock = false;

	CutSceneType m_eCurScene = NOSCENE;
	
};

