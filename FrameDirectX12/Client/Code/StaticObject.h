#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CStaticMesh;
	class CShader_Default;
}

class CDynamicCamera;

class CStaticObject : public Engine::CGameObject
{
private:
	explicit CStaticObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	        void    SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }
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
	MeshInfo         m_tMeshInfo;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CStaticObject*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

