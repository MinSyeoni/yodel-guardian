#pragma once

#include "Include.h"
#include "Transform.h"

namespace Engine
{
	class CTransform;
	class CMesh;
	class CSphereCollider;
	class CBoxCollider;
}

class CFlameThrower
{
public:
	enum FLAMESTATE { FLAME_IDLE, BC_Start/*¿¬¼Ó½î±â*/, CB_Enter/*ÇÑ¹ß½î±â*/, CB_Exit/*½î°í ³­ÈÄ ÂøÁö?*/, CB_FireLoop/*È­¿°¹æ»ç*/,
					CB_FireRecoil/*¹ºÁö¸ð¸£°ÚÀ½*/, CB_FireStart/*¹ºÁö¸ð¸£°ÚÀ½*/, CB_Idle/*°¡¸¸È÷*/, CB_Twitch/*µÎ¸®¹øµÎ¸®¹ø*/, CB_WalkDown/*?¸ØÃçÀÖÀ½*/,
					CB_WalkEast/*¸Ø*/, CB_WalkNorth/*Ãç*/, CB_WalkSouth/*ÀÖ*/, CB_WalkUp/*´Ù*/, CB_WalkWest/*??*/ };

public:
	CFlameThrower();
	virtual ~CFlameThrower();

public:
	void					Initialized();
	HRESULT					Late_Initialized();
	_int					Update_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom);
	_int					LateUpdate_FlameThrower(const _float& fTimeDelta, CTransform* pTransform, CMesh* m_pMeshCom);
	void					Animation_Test(const _float& fTimeDelta, CMesh* m_pMeshCom);
	void					Release();

public:
	const FLAMESTATE&		Get_CurState() { return m_eCurState; }
	const FLAMESTATE&		Get_PreState() { return m_ePreState; }
	void					Set_CurState(FLAMESTATE eState) { m_eCurState = eState; }
	void					Set_PreState(FLAMESTATE eState) { m_ePreState = eState; }
	
	void					Set_Transform(CTransform* pTransform) { m_pTransCom = pTransform; m_pTransCom->AddRef(); };
	
	_bool					IsPlayer_HitFireDemage() { return m_bIsHit; }
	_float					Get_FireDemage() { return m_fFireDemage; }

private:
	void					Chase_Player(const _float& fTimeDelta);
	void					Fire_Attak();

private:
	FLAMESTATE						m_eCurState;
	FLAMESTATE						m_ePreState;

	CTransform*						m_pTransCom = nullptr;
	CMesh*							m_pMeshCom = nullptr;

	Engine::CSphereCollider*		m_pSphereCol = nullptr;

private:
	_float					m_fTime = 0.f;
	_float					m_fSpineAngle = 0.f;
	_float					m_fAniTime = 0.f;

	_int					m_iRandState = 0;
	_vec3					m_pPlayerPos = _vec3(0.f,0.f,0.f);
	_vec3					m_pFlamePos = _vec3(0.f, 0.f, 0.f);
	_float					m_fAniDelay = 0.f;

	_float					m_fFireDemage = 10.f;
	_bool					m_bIsHit = false;

};

