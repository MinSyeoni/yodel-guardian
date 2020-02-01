#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CCubeTex;
	class CShader_ColorBuffer;
}

class CDynamicCamera;

class CCubeObject : public Engine::CGameObject
{
private:
	explicit CCubeObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCubeObject(const CCubeObject& rhs);
	virtual ~CCubeObject();

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
	Engine::CCubeTex*				m_pBufferCom = nullptr;
	Engine::CShader_ColorBuffer*	m_pShaderCom = nullptr;

	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	CDynamicCamera*	m_pDynamicCamera = nullptr;

public:
	virtual CGameObject*	Clone_GameObject(void* pArg);
	static CCubeObject*		Create(ID3D12Device* pGraphicDevice, 
								   ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

