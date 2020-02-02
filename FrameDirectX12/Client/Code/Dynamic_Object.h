#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CMesh;
	class CShader_Default;
}

class CDynamicCamera;

class CDynamicObject : public Engine::CGameObject
{
private:
	explicit CDynamicObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDynamicObject(const CDynamicObject& rhs);
	virtual ~CDynamicObject();

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

	float m_fTime = 0.f;
	vector<_matrix> vecMatrix;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CDynamicObject*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

