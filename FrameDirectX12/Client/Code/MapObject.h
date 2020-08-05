#pragma once
#include "Include.h"
#include "GameObject.h"
#include <string>

namespace Engine
{
	class CStaticMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CMapObject : public Engine::CGameObject
{
private:
	explicit CMapObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CMapObject(const CMapObject& rhs);
	virtual ~CMapObject();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	virtual void    SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }

private:
	virtual HRESULT Add_Component();

private:
	void			Set_ConstantTable();
    void            Set_ShadowTable(CShader_Shadow* pShader);
	void			Render_ShadowDepth(CShader_Shadow* pShader);

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*       m_pShaderCom = nullptr;

	MeshInfo					m_tMeshInfo;
	vector<vector<_matrix>>		m_vecMatrix;
	Engine::CBoxCollider* m_pBoxCom = nullptr;

	bool                  m_bIsDrawShadow = true;
	bool                  m_bIsBigObject = false;

	bool                  m_bIsColliderObject = false;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CMapObject*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

