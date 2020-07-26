#pragma once
#include "Include.h"
#include "GameObject.h"

class CKen : public Engine::CGameObject
{
	enum STATE { RUNEAST, RUNNORTH, RUNSOUTH, RUNWEST, WALKEAST, WALKNORTH, WALKSOUTH, WALKWEST, FIRE, RELOAD, PATROL, SEARCH, CHECK, IDLE };
	enum CHAPTER { START, TURNPLAYER, GOTOPLAYER, FIRSTTALK, FIRSTFIGHT, MOVING };
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
	void ChapterCheck(const _float& fTimeDelta);
	void TurnToPlayer(const _float& fTimeDelta);

	void MoveByAstar(const _float& fTimeDelta);
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



	vector<vector<_matrix>> m_vecMatrix;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CKen* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CGameObject* m_pPlayer;


	STATE m_eCurState = SEARCH;
	CHAPTER m_eCurChapter = START;
private:
	virtual void			Free();
};

