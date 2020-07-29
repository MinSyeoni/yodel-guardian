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

class CHPBar : public Engine::CGameObject
{
public:
	enum HPBAR_TYPE { PLAYER_HPBAER, COLLEAGUE_HPBAR, COLLEAGUE2_HPBAR};

private:
	explicit CHPBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CHPBar(const CHPBar& rhs);
	virtual ~CHPBar();

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

	void						Set_HP_Damage(const _float& fTimeDelta);
	void						Set_CurHpType(_uint iType) { m_iHPType = iType; }
	_uint						Get_CurHpType() { return m_iHPType; }
	HPBAR_TYPE					Get_CurHpState() { return m_eType; }

private:
	virtual HRESULT				Add_Component();
	void						Init_CharacterHp();

private:
	void						Set_ConstantTable();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	_bool						m_bIsShow = true;
	_uint						m_iHPType = 0;	// 0 - 피 깎임, 1 - 피 회복

	_float						m_fCurHp = 0.f;
	_float						m_fPreHp = 0.f;
	_matrix						m_matHPWorld = INIT_MATRIX;

	HPBAR_TYPE					m_eType;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static  CHPBar*				Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
