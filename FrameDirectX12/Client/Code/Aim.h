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

class CAim : public Engine::CGameObject
{

private:
	explicit CAim(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CAim(const CAim& rhs);
	virtual ~CAim();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	void SetRender(_bool bIsRender, int iDrawId = 0) { m_bIsRender = bIsRender; m_iDrawId = iDrawId; };
private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();
	void						Show_OtherUI(_bool bIsShow);

private:
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CShader_UI* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;

	CDynamicCamera* m_pDynamicCamera = nullptr;

	POINT						m_pt = {};
	_bool                      m_bIsRender = false;
	_int                       m_iDrawId = 0;
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CAim* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
