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
	void					Load_MonsterPos(const wstring& wstrFilePath);

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CBoxCollider*		m_pBoxCol = nullptr;
	Engine::CSphereCollider*	m_pShereCol = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	COLLIDER					m_tColInfo;
	MeshInfo					m_tMeshInfo;
private:
	TRIGGER_SHAPE				m_eCurShape = TRIGGER_END;

public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static	CTrigger*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

