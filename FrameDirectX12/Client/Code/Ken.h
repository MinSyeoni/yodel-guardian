#pragma once
#include "Include.h"
#include "GameObject.h"
class CPlayer;
class CMonster;
class CKen : public Engine::CGameObject
{
public:
	enum STATE { RUNEAST, RUNNORTH, RUNSOUTH, RUNWEST, WALKEAST, WALKNORTH, WALKSOUTH, WALKWEST, FIRE, RELOAD, PATROL, SEARCH, CHECK, IDLE };
	enum CHAPTER { START, TURNPLAYER, GOTOPLAYER, FIRSTTALK, ATTACK, PATROLCUT, SEARCHCUT, MOVINGPOINT, GOTOHOME };
private:
	explicit CKen(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CKen(const CKen& rhs);
	virtual ~CKen();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);
	Engine::CNaviMesh* Get_KenNavi() { return m_pNaviCom; }	// 추가함
	void MonsterCheck(const _float& fTimeDelta);
	void ChapterCheck(const _float& fTimeDelta);
	void TurnToPlayer(const _float& fTimeDelta);
	void GotoPlayer(const _float& fTimeDelta);
	void TalkCheck(const _float& fTimeDelta);
	void SetChapter(CHAPTER eChapter) { m_eCurChapter = eChapter; };
	void AttackCheck(const _float& fTimeDelta);
	void MoveByAstar(const _float& fTimeDelta);
	void PatrolCheck();
	void MoveDirectionCheck(const _float& fTimeDelta, _vec3 vTargetPos);
	void ReloadCheck();
	void ShootingCheck(const _float& fTimeDelta, CMonster* pMonster);

	void CollisionWithObject();

	STATE			Get_CurState() { return m_eCurState; };
	CHAPTER			Get_CurChapter() { return m_eCurChapter; };
	_float			Get_KenCurHP() { return m_iCurHP; }	// ui 때문에 추가함 나중에 피격할때 플레이어처럼 깎아줘야함
	void			Set_KenCurHP(_uint iHP);			// UI 때문에 추가함

private:
	void            Set_ShadowTable(CShader_Shadow* pShader);
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;
	Engine::CAstar* m_pAstarCom = nullptr;
	Engine::CNaviMesh* m_pNaviCom = nullptr;
public:
	_uint Get_FightingCount() { return m_iFightCount; };

	CBoxCollider* m_pBoxCollider = nullptr;
	vector<vector<_matrix>> m_vecMatrix;

public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CKen* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CGameObject* m_pPlayer;

	int m_iFightCount = 0;
	STATE m_eCurState = SEARCH;
	STATE m_eLegState = SEARCH;
	CHAPTER m_eCurChapter = START;

	_vec3 m_vMove1Pos;
	_vec3 m_vMove2Pos;
	_vec3 m_vMove3Pos;
	_vec3 m_vMove4Pos;

	_uint				m_iCurHP = 314;	// hpbar 때문에 추가함

	_uint m_uiMove1NaviIndex;
	_uint m_uiMove2NaviIndex;
	_uint m_uiMove3NaviIndex;
	_uint m_uiMove4NaviIndex;


	_int    m_fRifleBulletCount = 7;

	_float m_fDelayTime = 0.f;
	_bool  m_bIsReload = false;
	_bool m_bIsFinish = false;

private:
	virtual void			Free();
};

