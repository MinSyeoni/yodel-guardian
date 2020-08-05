#pragma once
#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CBoxCollider;
	class CSphereCollider;
	class CShader_Mesh;
}

class CTrigger : public Engine::CGameObject
{
public:
	enum TRIGGER_SHAPE{ TRIGGER_BOX, TRIGGER_SPHERE, TRIGGER_END };

private:
	explicit CTrigger(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CTrigger(const CTrigger& rhs);
	virtual ~CTrigger();

public:
	HRESULT					Ready_GameObjectPrototype();
	virtual HRESULT			Ready_GameObject();
	virtual HRESULT			LateInit_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject(const _float& fTimeDelta);
	

private:
	void					Set_ConstantTable();
	virtual HRESULT			Add_Component();

public:
	_uint					Get_ColID() { return m_iColID; }
	TRIGGER_SHAPE&			Get_ColType() { return m_eCurShape; }
	_vec3					Get_ColPos() { return m_pTransCom->m_vPos; }
	_bool					Get_IsGoBoss() { return m_bIsGoBoss; }

private:
	Engine::CBoxCollider*		m_pBoxCol = nullptr;
	Engine::CSphereCollider*	m_pShereCol = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	COLLIDER					m_tColInfo;

private:
	TRIGGER_SHAPE				m_eCurShape;

public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static	CTrigger*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
	_uint						m_iColID = 0;
	_bool						m_bIsActive = false;
	_bool						m_bIsGoBoss = false;
};

