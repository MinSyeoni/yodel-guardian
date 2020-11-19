#pragma once
#include "Include.h"
#include "Base.h"
#include "GraphicDevice.h"

namespace Engine
{
	class CComponentMgr;
	class CObjectMgr;
	class CManagement;
	class CRenderer;
	class CFont;
}

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	_int	LateUpdate_MainApp(const _float& fTimeDelta);
	void	Render_MainApp(const _float& fTimeDelta);

private:
	HRESULT	SetUp_DefaultSetting(CGraphicDevice::WINMODE eMode, const _uint& dwWidth, const _uint& dwHeight);
	HRESULT	SetUp_ComponentPrototype();
	HRESULT	SetUp_StartScene(Engine::SCENEID eScebeID);
	HRESULT SetUp_Resource();
	HRESULT	SetUp_Font();
private:
	/*____________________________________________________________________
	[ GraphicDevice / Mgr ]
	______________________________________________________________________*/
	Engine::CGraphicDevice*		m_pDeviceClass		= nullptr;
	ID3D12Device*				m_pGraphicDevice	= nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList		= nullptr;

	Engine::CComponentMgr*	m_pComponentMgr		= nullptr;
	Engine::CObjectMgr*		m_pObjectMgr		= nullptr;
	Engine::CManagement*	m_pManagement		= nullptr;
	Engine::CRenderer*		m_pRenderer			= nullptr;

	_float         m_fTime = 0.f;
	_uint         m_uiFPS = 0;
	_tchar         m_szFPS[MIN_STR] = L"";
public:
	static CMainApp*	Create();
private:
	virtual void		Free();
};

