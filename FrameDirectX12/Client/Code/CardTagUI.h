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

class CCardTagUI : public Engine::CGameObject
{
private:
	explicit CCardTagUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCardTagUI(const CCardTagUI& rhs);
	virtual ~CCardTagUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

public:
	void						TagReaderAndZoom(const _float& fTimeDelta);
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	void						Set_IsTagOn(_bool bIsTagOn) { m_bIsTagOn = bIsTagOn; }
	_bool						Get_IsTagOn() { return m_bIsTagOn; }
	_bool						Get_IsTagClear() { return m_bIsClear; }

private:
	virtual HRESULT				Add_Component();
	void						Set_ConstantTable();

private:
	void						ZoomCamera(bool bIsZoom);
	void						DoorAndTag_Interaction();
	void						BillBoard();
	void						CardTagOn_Clear(const _float& fTimeDelta);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	_bool						m_bIsShow = true;
//	_matrix						m_matTagWorld;

	_float						m_fCurTag = 0.f;
	_float						m_fTagDelay = 0.f;

	_bool						m_bIsTagOn = false;
	_bool						m_bIsClear = false;
	_bool						m_bIsZoom = false;
	_bool						m_bIsAlreadyZoom = false;

	_bool						m_bIsTagClearSound = false;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static CCardTagUI*			Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
