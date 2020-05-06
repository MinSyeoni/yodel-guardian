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

class CLightObject : public Engine::CGameObject
{
	enum Type{SYREN,ENDLIGHT};

private:
	explicit CLightObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CLightObject(const CLightObject& rhs);
	virtual ~CLightObject();

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

private:
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;

	MeshInfo					m_tMeshInfo;
	_float m_fAccTime;
	_uint m_uiLightIndex;
	_bool m_bIsLight = true;
	D3DLIGHT m_tagLight;
	Type m_eType;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CLightObject* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

