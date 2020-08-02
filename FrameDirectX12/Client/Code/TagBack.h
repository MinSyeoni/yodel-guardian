#pragma once
#include "Include.h"
#include "GameObject.h"
#include "GraphicDevice.h"

namespace Engine
{
	class CRcTex;
	class CShader_ColorBuffer;
}

class CDynamicCamera;

class CTagBack : public Engine::CGameObject
{
public:
	enum TAG_TYPE { TAG_START, TAG_CLEAR };

private:
	explicit CTagBack(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CTagBack(const CTagBack& rhs);
	virtual ~CTagBack();

public:
	HRESULT							Ready_GameObjectPrototype();
	virtual HRESULT					Ready_GameObject();
	virtual HRESULT					LateInit_GameObject();
	virtual _int					Update_GameObject(const _float& fTimeDelta);
	virtual _int					LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void					Render_GameObject(const _float& fTimeDelta);
private:
	virtual HRESULT					Add_Component();
	void							Set_ConstantTable(_uint iIdx);
	void							BillBoard();

public:
	void							Set_CurCardTag(TAG_TYPE eType) { m_eType = eType; }
	void							Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }

private:
	Engine::CRcTex*					m_pBufferCom = nullptr;
	Engine::CShader_UI*				m_pShaderCom[2] = { nullptr, };
	Engine::CTexture*               m_pTexture[2] = { nullptr, };

	CDynamicCamera*					m_pDynamicCamera = nullptr;
	TAG_TYPE						m_eType = TAG_START;
	_bool							m_bIsShow = false;

public:
	virtual CGameObject*			Clone_GameObject(void* pArg);
	static CTagBack*				Create(ID3D12Device* pGraphicDevice,
									ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void					Free();
};

