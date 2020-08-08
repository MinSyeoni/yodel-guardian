#pragma once
#include "Include.h"
#include "GameObject.h"
#include "DirectInput.h"

namespace Engine
{
	class CBoxCollider;
	class CStaticMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CDronBullet : public Engine::CGameObject
{
private:
	explicit CDronBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDronBullet(const CDronBullet& rhs);
	virtual ~CDronBullet();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	virtual void				SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();
    void						Set_ShadowTable(CShader_Shadow* pShader);
	void						Render_ShadowDepth(CShader_Shadow* pShader);

private:
	Engine::CBoxCollider*		m_pBoxCollider = nullptr;

	MeshInfo					m_tMeshInfo;
	CGameObject*				m_pGameObject = nullptr;
	_vec3						m_vPlayerPos = _vec3(0.f, 0.f, 0.f);

public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static CDronBullet*			Create(ID3D12Device* pGraphicDevice,
								ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

