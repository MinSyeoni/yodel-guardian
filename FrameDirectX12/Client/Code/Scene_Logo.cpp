#include "stdafx.h"
#include "Scene_Logo.h"
#include "DirectInput.h"
#include "Scene_Menu.h"
#include "Scene_Stage.h"
#include "Management.h"
#include "GraphicDevice.h"
#include "BackGround.h"
#include "DynamicCamera.h"
#include "DirectSound.h"
CScene_Logo::CScene_Logo(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
}


CScene_Logo::~CScene_Logo()
{
}

HRESULT CScene_Logo::Ready_Scene()
{
#ifdef _DEBUG
	COUT_STR("Ready Scene_Logo");
#endif

	FAILED_CHECK_RETURN(Ready_GameObjectPrototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LayerEnvironment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerCamera(L"Layer_Camera"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LayerGameObject(L"Layer_GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerUI(L"Layer_UI"), E_FAIL);


	m_pFont = static_cast<CFont*>( CObjectMgr::Get_Instance()->Get_NewGameObject(L"Prototype_Font_NetmarbleLight",L"fuck",nullptr));
	m_pFont->Ready_GameObjectClone(L"Mesh_Loading", _vec2{ 50.f,50.f }, D2D1::ColorF::Red);
	m_pLoading = CLoading::Create(m_pGraphicDevice,m_pCommandList,CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);



	return S_OK;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	m_pFont->Update_GameObject(fTimeDelta);
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{

	m_pFont->LateUpdate_GameObject(fTimeDelta);
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CScene_Logo::Render_Scene(const _float & fTimeDelta)
{
	CScene::Render_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish()&& KEY_DOWN(DIK_RETURN))
	{
		m_pObjectMgr->Clear_Layer();
		Engine::CScene* pNewScene = CScene_Stage::Create(m_pGraphicDevice, m_pCommandList);
		Engine::CManagement::Get_Instance()->SetUp_CurrentScene(pNewScene);
	}



}


HRESULT CScene_Logo::Ready_GameObjectPrototype()
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	// Prototype - CubeObject
	CGameObject*pGameObject;
	pGameObject = CBackGround::Create(m_pGraphicDevice, m_pCommandList);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_BackGround", pGameObject), E_FAIL);


	// Prototype - DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDevice, m_pCommandList,
		CAMERA_INFO(_vec3(0.f, 10.f, -15.f),	// Eye
			_vec3(0.f, 1.f, 1.f),	// At
			_vec3(0.f, 1.f, 0.f)),	// Up
		PROJ_INFO(60.f,							// FovY
			_float(WINCX) / _float(WINCY),	// Aspect
			1.f,							// Near
			500.f),							// Far
		ORTHO_INFO(WINCX,	// Viewport Width
			WINCY,	// Viewport Height
			0.f,	// Near
			1.f));	// Far

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_DynamicCamera", pGameObject), E_FAIL);


	CDirectSound::Get_Instance()->LoadDirectSoundFile(L"GUNSHOT");//여기야시영
	CDirectSound::Get_Instance()->LoadDirectSoundFile(L"ZombiDead");//여기야시영
	CDirectSound::Get_Instance()->LoadDirectSoundFile(L"Siren");//여기야시영
	CDirectSound::Get_Instance()->LoadDirectSoundFile(L"Bgm");//여기야시영



	CDirectSound::Get_Instance()->PlayDirectSoundEffect(L"Bgm");//여기야시영
	return S_OK;
}

HRESULT CScene_Logo::Ready_LayerEnvironment(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	return S_OK;
}

HRESULT CScene_Logo::Ready_LayerGameObject(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	return S_OK;
}

HRESULT CScene_Logo::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 생성.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_BackGround", L"BackGround", nullptr), E_FAIL);


	return S_OK;
}

HRESULT CScene_Logo::Ready_LayerCamera(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);


	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	// DynamicCamera
	//FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_DynamicCamera", L"DynamicCamera", nullptr), E_FAIL);


	return S_OK;
}

CScene_Logo * CScene_Logo::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_Logo::Free()
{
	Safe_Release(m_pLoading);
	Engine::CScene::Free();
	Safe_Release(m_pFont);
}
