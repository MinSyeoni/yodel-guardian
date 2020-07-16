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

class COffUI : public Engine::CGameObject
{

private:
	explicit COffUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit COffUI(const COffUI& rhs);
	virtual ~COffUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	void						Set_IsOptionOn(_int iIdx, _bool bIsOn) { m_bIsOn[iIdx] = bIsOn; }

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	POINT						m_pt = {};
	_bool						m_bIsShow = false;
	_bool						m_bIsOn[7] = { false, };
	wstring						m_strOptionTag = L"";
	_uint						m_iOffIdx = 0;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static COffUI*				Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
