#pragma once
#include "Include.h"
#include "GameObject.h"






class CShepard : public Engine::CGameObject
{
	enum STATE { SEARCH,CHECK, RUNNORTH,TURNRIGHT };
	enum CHAPTER{START,TURNPLAYER,GOTOPLAYER,FIRSTTALK,FIRSTFIGHT,MOVING};
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
	void ChapterCheck();
	void TurnToPlayer();
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


	vector<vector<_matrix>> m_vecMatrix;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CShepard* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	STATE m_eCurState = SEARCH;
	CHAPTER m_eCurChapter = START;
private:
	virtual void			Free();
};

