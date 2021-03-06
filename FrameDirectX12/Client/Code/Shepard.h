#pragma once
#include "Include.h"
#include "GameObject.h"
class CPlayer;
class CMonster;
class CShepard : public Engine::CGameObject
{
public:
	enum STATE { RUNEAST, RUNNORTH, RUNSOUTH, RUNWEST, WALKEAST,WALKNORTH,WALKSOUTH,WALKWEST,FIRE,RELOAD,PATROL,SEARCH,CHECK,IDLE };
	enum CHAPTER { START, TURNPLAYER, GOTOPLAYER, FIRSTTALK, ATTACK, PATROLCUT,SEARCHCUT, MOVINGPOINT, GOTOHOME };
private:
	explicit CShepard(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShepard(const CShepard& rhs);
	virtual ~CShepard();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);

	void CheckPlayer();
	void MonsterCheck(const _float& fTimeDelta);
	void ChapterCheck(const _float& fTimeDelta);
	void TurnToPlayer(const _float& fTimeDelta);
	void GotoPlayer(const _float& fTimeDelta);
	void TalkCheck(const _float& fTimeDelta);
	void SetChapter(CHAPTER eChapter) { m_eCurChapter = eChapter; };
	void AttackCheck(const _float& fTimeDelta);
	void MoveByAstar(const _float& fTimeDelta);
	void PatrolCheck();
	void MoveDirectionCheck(const _float& fTimeDelta,_vec3 vTargetPos);
	void ReloadCheck();
	void ShootingCheck(const _float& fTimeDelta,CMonster* pMonster);
	void FightingCheck();
	void CollisionWithObject();

	Engine::CNaviMesh* Get_ShepardNavi() { return m_pNaviCom; }	// 추가함
	_float			Get_ShepardCurHP() { return m_iCurHP; }	// ui 때문에 추가함 나중에 피격할때 플레이어처럼 깎아줘야함
	void			Set_ShepardCurHP(_uint iHP);

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


	CBoxCollider* m_pBoxCollider = nullptr;
	vector<vector<_matrix>> m_vecMatrix;

	_matrix* m_matChestOffset;
	_matrix* m_matChest;

public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CShepard* Create(ID3D12Device* pGraphicDevice,
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


	_uint m_uiMove1NaviIndex;
	_uint m_uiMove2NaviIndex;
	_uint m_uiMove3NaviIndex;
	_uint m_uiMove4NaviIndex;

	_uint				m_iCurHP = 314;	// hpbar 때문에 추가함
	_int    m_fRifleBulletCount = 7;

	_float m_fDelayTime = 0.f;
	_bool  m_bIsReload = false;
	_bool m_bIsFinish = false;
	

	_bool m_bIsStartConver = false;//플레이어 시야에 들어왓늕ㅣ 
	_bool m_bIsTalk = false;//플레이어와 대화시작





	_bool m_bIsSound1 = false;
	_bool m_bIsSound2 = false;
private:


	_bool			m_bIsNextSound2 = false;		// 추가가ㅏㅏ가
	_bool			m_bIsNextSound3 = false;		// 추가가ㅏㅏ가
	_bool			m_bIsNextSound4 = false;		// 추가가ㅏㅏ가
	_bool			m_bIsNextSound5 = false;


private:
	virtual void			Free();
};

