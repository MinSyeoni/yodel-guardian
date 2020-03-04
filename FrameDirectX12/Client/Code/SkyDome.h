#pragma once
#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader_Mesh;
}
class CDynamicCamera;
class CSkyDome : public Engine::CGameObject
{
private:
	explicit CSkyDome(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome();


public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	void FollowCamera();
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


public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CSkyDome*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();

};

