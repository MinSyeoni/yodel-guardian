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

class CUI : public Engine::CGameObject
{
private:
	explicit CUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();

private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	CDynamicCamera*				m_pDynamicCamera = nullptr;
	_bool						m_bIsShow = true;
public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static CUI*					Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
