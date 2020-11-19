#pragma once
#include "Include.h"
#include "GameObject.h"



class CStonePaticle : public Engine::CGameObject
{
private:
	explicit CStonePaticle(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CStonePaticle(const CStonePaticle& rhs);
	virtual ~CStonePaticle();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(_vec3 vPos);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	void            MovePaticle(const _float& fTimeDelta);

private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
	void            Set_ShadowTable(CShader_Shadow* pShader);
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CStonePaticle* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);


private:
	_vec3 m_vDir;
	_float m_fGravity = 1.5;
	_float m_fAccTime = 0.f;
private:
	virtual void			Free();
};

