#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CMesh;
	class CShader_Mesh;
}

class CPlayerHed : public Engine::CGameObject
{

public:
	enum HEDTYPE { BOLD, SPORT, NONE };

private:
	explicit CPlayerHed(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPlayerHed(const CPlayerHed& rhs);
	virtual ~CPlayerHed();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(HEDTYPE eType);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
public:
	void            Set_Animation(_uint Animation) { m_iAnimationKey = Animation; };
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
	_uint   m_iAnimationKey = 0;
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*         	m_pShaderCom = nullptr;


	float m_fTime = 0.f;
	vector<vector<_matrix>> m_vecMatrix;


	HEDTYPE m_eArmType = BOLD;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CPlayerHed*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

