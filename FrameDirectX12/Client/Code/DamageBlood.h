#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"

namespace Engine
{
	class CRcTex;
	class CShader_ColorBuffer;
}

class CDynamicCamera;

class CDamageBlood : public Engine::CGameObject
{
private:
	explicit CDamageBlood(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDamageBlood(const CDamageBlood& rhs);
	virtual ~CDamageBlood();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();

private:
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CShader_UI* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;

	_float m_fAccTime = 0.f;


public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CDamageBlood* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
