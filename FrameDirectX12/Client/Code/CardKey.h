#pragma once
#include "Include.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "DirectInput.h"

namespace Engine
{
	class CBoxCollider;
	class CStaticMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CCardKey : public Engine::CGameObject
{
private:
	explicit CCardKey(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCardKey(const CCardKey& rhs);
	virtual ~CCardKey();

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
	void						PutTheCard_OnTheDoor();
	void						Coliision_CardAndPlayer();

private:
	void						Set_ConstantTable();
    void						Set_ShadowTable(CShader_Shadow* pShader);
	void						Render_ShadowDepth(CShader_Shadow* pShader);

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*       m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCollider = nullptr;

	MeshInfo					m_tMeshInfo;
	_bool						m_bIsEquip = false;
	CGameObject*				m_pGameObject = nullptr;
public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static CCardKey*			Create(ID3D12Device* pGraphicDevice,
								ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

