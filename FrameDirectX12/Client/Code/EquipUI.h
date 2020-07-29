#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"

class CEquipUI : public Engine::CGameObject
{
public:
	enum EQUIP_TYPE { E_KITEQUIP1, E_KITEQUIP2, E_KITEQUIP3, E_CONVERSATION, E_KEYEQUIP, E_HIDING, E_DOOROPEN_L, E_DOOROPEN_P };

private:
	explicit CEquipUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CEquipUI(const CEquipUI& rhs);
	virtual ~CEquipUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	void						Init_TypePos();
	virtual void				Render_GameObject(const _float& fTimeDelta);

public:
	void						Set_ShowUI(_bool bIsShow);
	EQUIP_TYPE&					Get_EquipType() { return m_eEquipType; }
	void						Set_EquipPos(_float fPosX, _float fPosY) { m_vPos.x = fPosX; m_vPos.y = fPosY; }

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	_bool						m_bIsShow = false;
	_vec3						m_vPos = _vec3{ 0.f,0.f,0.f };
	EQUIP_TYPE					m_eEquipType;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static CEquipUI*			Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
