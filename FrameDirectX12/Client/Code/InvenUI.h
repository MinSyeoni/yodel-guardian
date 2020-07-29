#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"

namespace Engine
{
	class CFont;
	class CRcTex;
	class CShader_ColorBuffer;
}

class CDynamicCamera;

class CInvenUI : public Engine::CGameObject
{
private:
	explicit CInvenUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CInvenUI(const CInvenUI& rhs);
	virtual ~CInvenUI();

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	void						Set_AddItemNum(_uint iItemIdx, _uint iAddNum) { m_iItemNum[iItemIdx] += iAddNum; }

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	void Use_ItemBandage();
	virtual void				Render_GameObject(const _float& fTimeDelta);

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();
	void						Set_FontText();
	void						Set_InvenIconPos();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;
	Engine::CFont*				m_pFont[3] = {};

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	POINT						m_pt = {};
	_bool						m_bIsShow = true;

	_uint						m_iItemNum[3] = {};		// 0 - bandage, 1 - medicine, 2 - syringe 

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static  CInvenUI*			Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
