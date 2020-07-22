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

class COnUI : public Engine::CGameObject
{
private:
	explicit COnUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit COnUI(const COnUI& rhs);
	virtual ~COnUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	_uint						Get_OptionIdx() { return m_iOnIdx; }
	/*
	0 - OPTION_SSAO, 1 - OPTION_METALIC, 2 - OPTION_EMISSIVE, 3 - OPTION_DISTORITION,
	4 - OPTION_SHADOW, 5 - OPTION_RIMLIGHT, 6 - OPTION_TONEMAPPING
	*/
	void						CheckRenderer();//이거내가만들엇다 시영아
	void						Set_IsOptionOff(_bool bIsOn) { m_bIsOff = bIsOn; }
	void						Set_OnUI_Pos();
	void						Show_Option_OnOff();

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable(int iIdx);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom[2] = {};
	Engine::CTexture*			m_pTexture[2] = {};

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	POINT						m_pt = {};
	_bool						m_bIsShow = false;
	_bool						m_bIsOff = false;

	_uint						m_iOnIdx = 0;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static COnUI*				Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
