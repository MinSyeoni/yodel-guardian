#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CMesh;
	class CShader_Default;
}

class CDynamicCamera;

class CSalone : public Engine::CGameObject
{
	enum STATE{IDLE,EYEBLINK,RUN,WALK};

private:
	explicit CSalone(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CSalone(const CSalone& rhs);
	virtual ~CSalone();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);

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
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*         	m_pShaderCom = nullptr;

	CDynamicCamera*	m_pDynamicCamera = nullptr;

	vector<vector<_matrix>> m_vecMatrix;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CSalone*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	STATE m_eCurState = EYEBLINK;
private:
	virtual void			Free();
};

