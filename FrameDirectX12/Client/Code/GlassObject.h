#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CStaticMesh;
	class CShader_Mesh;
	class CBoxCollider;
	class CShader_Destortion;
}

class CDynamicCamera;

class CGlassObject : public Engine::CGameObject
{
private:
	explicit CGlassObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CGlassObject(const CGlassObject& rhs);
	virtual ~CGlassObject();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_Distortion(const _float& fTimeDelta);
	virtual void    SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }


private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTableDistort();
	void            Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Destortion*         	m_pDistortShaderCom = nullptr;
	Engine::CShader_Mesh*            m_pShaderCom = nullptr;
	MeshInfo         m_tMeshInfo;
	_float m_fTime = 0.f;
	_float m_fSize = 0.5f;
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CGlassObject*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

