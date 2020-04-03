#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"

namespace Engine
{
	class CFont;
	class CRcTex;
	class CShader_ColorBuffer;
	class CDirectInput;
}

class CDynamicCamera;

class CGunUI : public Engine::CGameObject
{
private:
	explicit CGunUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CGunUI(const CGunUI& rhs);
	virtual ~CGunUI();

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
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

	POINT						m_pt = {};
	const _tchar*				m_pGunTag = nullptr;

	_int						m_iCurBullet = 0;
	_int						m_iMaxBullet = 0;

	Engine::CFont*				m_pBulletFont = nullptr;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static  CGunUI*				Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList,
										const _tchar* pTag);
private:
	virtual void				Free();
};
