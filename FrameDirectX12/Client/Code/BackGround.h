#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CRcTex;
	class CShader_ColorBuffer;
}

class CDynamicCamera;

class CBackGround : public Engine::CGameObject
{
private:
	explicit CBackGround(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();

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
	Engine::CRcTex*					m_pBufferCom = nullptr;
	Engine::CShader_DefaultTex*	    m_pShaderCom = nullptr;
	Engine::CTexture*               m_pTexture = nullptr;
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	CDynamicCamera*	m_pDynamicCamera = nullptr;

public:
	virtual CGameObject*	Clone_GameObject(void* pArg);
	static CBackGround*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

