#pragma once

#include "Include.h"
#include "Transform.h"
#include "NaviMesh.h"
#include "BoxCollider.h"
#include "Astar.h"

namespace Engine
{
	class CTransform;
	class CMesh;
	class CBoxCollider;
	class CNaviMesh;
	class CAstar;
}

class CDron
{
public:
	enum DRONSTATE { DRON_BasePose, DRON_CB_WalkDown, DRON_CB_WalkEast, DRON_CB_WalkNorth,
					 DRON_CB_WalkSouth, DRON_CB_WalkUp, DRON_CB_WalkWest, DRON_DG_Front,
					 DRON_DG_Idle, DRON_EX_IdleHover, DRON_EX_IdleHoverTwitch, DRON_EX_IdleNoise,
					 DRON_EX_IdleSway, DRON_FlightPose, DRON_OLD_Idle, DRON_DG_Death };

public:
	CDron();
	virtual ~CDron();

public:
	void					Initialized();
	HRESULT					Late_Initialized();
	_int					Update_Dron(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom);
	_int					LateUpdate_Dron(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom);
	void					Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom);
	void					Attak_Player(Engine::CMesh* m_pMeshCom, CDron::DRONSTATE eState);
	void					Release();

public:
	const DRONSTATE&		Get_CurState() { return m_eCurState; }
	const DRONSTATE&		Get_PreState() { return m_ePreState; }
	void					Set_CurState(DRONSTATE eState) { m_eCurState = eState; }
	void					Set_PreState(DRONSTATE eState) { m_ePreState = eState; }
	
	void					Set_Transform(CTransform* pTransform) { m_pTransCom = pTransform; m_pTransCom->AddRef(); }
	void					Set_NaviMesh(CNaviMesh* pNavimesh) { m_pNaviMesh = pNavimesh; m_pNaviMesh->AddRef(); }
	void					Set_Astar(CAstar* pAstar) { m_pAstarCom = pAstar; m_pAstarCom->AddRef(); }
public:		// 상호작용 
	const _bool&			Get_IsDeadDron() const { return m_bIsDronState[1]; }
	const _bool&			Get_IsHit() const { return m_bIsDronState[2]; }
	const _bool&			Get_IsAtkPlayer() const { return m_bIsDronState[3]; }
	void					Set_IsHit(_bool bIsHit) { m_bIsDronState[2] = bIsHit; }
	const _float&			Get_CurHp() { return m_fCurHp; }	
	void					Set_HitDamage(_float fDamage) { m_fHitDamage = fDamage; }
	void					Set_InitDrawID(_uint iID) { m_iDrawID = iID; }

	void					Set_IsTurn(_bool bIsTurn) { m_bIsTurn = bIsTurn; }
	_bool					Get_IsTurn() { return m_bIsTurn; }
	void					Set_Direction(_vec3 vDir) { m_pTransCom->m_vDir = vDir; }
	_vec3					Get_Direction() { return m_pTransCom->m_vDir; }

private:	
	void					Update_DronPos(const _float& fTimeDelta);
	void					Dron_OnTrigger(const _float& fTimeDelta);
	void					MoveByAstar(const _float& fTimeDelta);
	void					Update_DronHP();
	void					Chase_Rotation(_vec3 vPos, const _float& fTimeDelta);
	_bool					Check_PlayerRange(_float fRange);

private:
	DRONSTATE				m_eCurState;
	DRONSTATE				m_ePreState;

	CTransform*				m_pTransCom = nullptr;
	CNaviMesh*				m_pNaviMesh = nullptr;
	CAstar*					m_pAstarCom = nullptr;
	Engine::CMesh*			m_pMeshCom = nullptr;

private:
	_bool					m_bIsTurn = false;
	_bool					m_bIsHitBullet = false;

	_float					m_fTime = 0.f;
	_float					m_fAniTime = 0.f;
	_float					m_fAniDelay = 0.f;

	_vec3					m_vPlayerPos = _vec3(0.f, 0.f, 0.f);
	_vec3					m_vChaseDir = _vec3(0.f, 0.f, 0.f);
	_vec3					m_vInitPos = _vec3(0.f, 0.f, 0.f);

	_bool					m_bIsDronState[4] = {false};	// 0=m_bIsTurn, 1=m_bIsDead, 2=m_bIsHit, 3=m_bIsATK;

	_float					m_fHitDamage = 0.f; // 맞을때
	_float					m_fSpeed = 0.f;
	_float					m_fCurHp = 0.f;
	_float					m_fMaxHp = 0.f;
	_float					m_fShootDelay = 0.f;

	_uint					m_iDrawID;

	_bool					m_bIsShoot = false;
	_bool					m_bIsAlreadyTurn = false;

	//여기좀추가햇으
	_bool					m_bIsDeadSound = false;
	_bool					m_bIsShootSound = false;
};

