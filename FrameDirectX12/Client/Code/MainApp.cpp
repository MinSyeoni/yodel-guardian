#include "stdafx.h"
#include "MainApp.h"
#include "DirectInput.h"
#include "DirectSound.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "Scene_Logo.h"
#include "Scene_Menu.h"
#include "Scene_Stage.h"
#include "DynamicCamera.h"
CMainApp::CMainApp()
	: m_pDeviceClass(CGraphicDevice::Get_Instance())
	, m_pComponentMgr(CComponentMgr::Get_Instance())
	, m_pObjectMgr(CObjectMgr::Get_Instance())
	, m_pManagement(CManagement::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
{
}


CMainApp::~CMainApp()
{
	Free();
}

HRESULT CMainApp::Ready_MainApp()
{
	srand(unsigned int(time(nullptr)));

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(CGraphicDevice::MODE_WIN, WINCX, WINCY), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_ComponentPrototype(), E_FAIL);
	FAILED_CHECK_RETURN(SetUp_Resource(),E_FAIL);
	FAILED_CHECK_RETURN(SetUp_StartScene(Engine::SCENEID::SCENE_LOGO), E_FAIL);
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pManagement, -1);

	/*____________________________________________________________________
	Direct Input
	______________________________________________________________________*/
	Engine::CDirectInput::Get_Instance()->SetUp_InputState();
	
	return m_pManagement->Update_Management(fTimeDelta);
}

_int CMainApp::LateUpdate_MainApp(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pManagement, -1);

	return m_pManagement->LateUpdate_Management(fTimeDelta);
}

void CMainApp::Render_MainApp(const _float& fTimeDelta)
{
	if (nullptr == m_pDeviceClass ||
		nullptr == m_pManagement)
		return;


	m_pManagement->Render_Management(fTimeDelta);

}

HRESULT CMainApp::SetUp_DefaultSetting(CGraphicDevice::WINMODE eMode, const _uint& uiWidth, const _uint& uiHeight)
{
	NULL_CHECK_RETURN(m_pDeviceClass, E_FAIL);

	/*____________________________________________________________________
	GraphicDevice 초기화.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pDeviceClass->Ready_GraphicDevice(g_hWnd, g_hInst, eMode, uiWidth, uiHeight), E_FAIL);
	
	m_pGraphicDevice	= m_pDeviceClass->Get_GraphicDevice();
	NULL_CHECK_RETURN(m_pGraphicDevice, E_FAIL);

	m_pCommandList		= m_pDeviceClass->Get_CommandList();
	NULL_CHECK_RETURN(m_pCommandList, E_FAIL);


	/*____________________________________________________________________
	Renderer 초기화.
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Ready_Renderer(m_pGraphicDevice, m_pCommandList), E_FAIL);

	/*____________________________________________________________________
	DirectInput 장치 초기화.
	______________________________________________________________________*/
	if (FAILED(Engine::CDirectInput::Get_Instance()->Ready_InputDevice(g_hInst, g_hWnd)))
	{
		CDirectInput::Get_Instance()->Destroy_Instance();
		return E_FAIL;
	}

	/*____________________________________________________________________
	DirectSound 초기화.
	______________________________________________________________________*/
	//if (FAILED(Engine::CDirectSound::Get_Instance()->Ready_DirectSound(g_hWnd)))
	//{
	//	CDirectSound::Get_Instance()->Destroy_Instance();
	//	return E_FAIL;
	//}
	//Engine::CDirectSound::Get_Instance()->LoadDirectSoundFile(L"Logo_BGM");

	return S_OK;
}

HRESULT CMainApp::SetUp_ComponentPrototype()
{
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Transform", ID_DYNAMIC, pComponent), E_FAIL);

	// Info
	pComponent = Engine::CInfo::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Info", ID_STATIC, pComponent), E_FAIL);

	// CubeTex
	pComponent = Engine::CCubeTex::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_CubeTex", ID_STATIC, pComponent), E_FAIL);

	// RcTex
	pComponent = Engine::CRcTex::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_RcTex", ID_STATIC, pComponent), E_FAIL);

	//TerrainTex
	pComponent = Engine::CTerrainTex::Create(m_pGraphicDevice, m_pCommandList,128,128,1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_TerrainTex", ID_STATIC, pComponent), E_FAIL);




	return S_OK;
}

HRESULT CMainApp::SetUp_StartScene(Engine::SCENEID eScebeID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene* pScene = nullptr;

	switch (eScebeID)
	{
	case Engine::SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pGraphicDevice, m_pCommandList);
		break;
	case Engine::SCENE_MENU:
		pScene = CScene_Menu::Create(m_pGraphicDevice, m_pCommandList);
		break;
	case Engine::SCENE_STAGE:
		pScene = CScene_Stage::Create(m_pGraphicDevice, m_pCommandList);
	default:
		break;
	}

	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(m_pManagement->SetUp_CurrentScene(pScene), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::SetUp_Resource()
{


	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CTexture::Create(m_pGraphicDevice, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Logo", ID_STATIC, pComponent), E_FAIL);



	CGraphicDevice::Get_Instance()->Wait_ForGpuComplete();
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
}
