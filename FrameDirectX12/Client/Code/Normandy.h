#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CNormandy : public Engine::CGameObject
{
private:
	explicit CNormandy(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CNormandy(const CNormandy& rhs);
	virtual ~CNormandy();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;


	float m_fTime = 0.f;
	vector<vector<_matrix>> m_vecMatrix;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CNormandy* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

