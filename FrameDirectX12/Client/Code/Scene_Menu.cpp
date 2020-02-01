#include "stdafx.h"
#include "Scene_Menu.h"
#include "DirectInput.h"
#include "Scene_Stage.h"
#include "Management.h"


CScene_Menu::CScene_Menu(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
}


CScene_Menu::~CScene_Menu()
{
}

HRESULT CScene_Menu::Ready_Scene()
{
#ifdef _DEBUG
	COUT_STR("Ready Scene_Menu");
#endif

	FAILED_CHECK_RETURN(Ready_GameObjectPrototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LayerEnvironment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerGameObject(L"Layer_GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerUI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

_int CScene_Menu::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CScene_Menu::LateUpdate_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CScene_Menu::Render_Scene(const _float & fTimeDelta)
{

	CScene::Render_Scene(fTimeDelta);

	if (KEY_DOWN(DIK_RETURN))
	{
		m_pObjectMgr->Clear_Layer();

		Engine::CScene* pNewScene = CScene_Stage::Create(m_pGraphicDevice, m_pCommandList);
		Engine::CManagement::Get_Instance()->SetUp_CurrentScene(pNewScene);
	}
}

HRESULT CScene_Menu::Ready_GameObjectPrototype()
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);


	return S_OK;
}

HRESULT CScene_Menu::Ready_LayerEnvironment(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	Environment Layer 持失.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 持失.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

HRESULT CScene_Menu::Ready_LayerGameObject(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	GameLogic Layer 持失.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 持失.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

HRESULT CScene_Menu::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 持失.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 持失.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

CScene_Menu * CScene_Menu::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CScene_Menu* pInstance = new CScene_Menu(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_Menu::Free()
{
	Engine::CScene::Free();
}
