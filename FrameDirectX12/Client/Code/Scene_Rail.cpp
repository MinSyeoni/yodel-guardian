#include "stdafx.h"
#include "Scene_Rail.h"
#include "DirectInput.h"
#include "Scene_Stage.h"
#include "Management.h"


CScene_Rail::CScene_Rail(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
}


CScene_Rail::~CScene_Rail()
{
}

HRESULT CScene_Rail::Ready_Scene()
{
#ifdef _DEBUG
	COUT_STR("Ready Scene_Rail");
#endif

	FAILED_CHECK_RETURN(Ready_GameObjectPrototype(), E_FAIL);


	FAILED_CHECK_RETURN(Ready_LayerCamera(L"Layer_Camera"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerEnvironment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerGameObject(L"Layer_GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerUI(L"Layer_UI"), E_FAIL);

	return S_OK;
}

_int CScene_Rail::Update_Scene(const _float& fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CScene_Rail::LateUpdate_Scene(const _float& fTimeDelta)
{
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CScene_Rail::Render_Scene(const _float& fTimeDelta)
{

	CScene::Render_Scene(fTimeDelta);
}

HRESULT CScene_Rail::Ready_GameObjectPrototype()
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);


	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerCamera(wstring wstrLayerTag)
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
	______________________________________________________________________*/
	// DynamicCamera
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_DynamicCamera", L"DynamicCamera", nullptr), E_FAIL);

	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerEnvironment(wstring wstrLayerTag)
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

HRESULT CScene_Rail::Ready_LayerGameObject(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	GameLogic Layer 持失.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Terrain", L"Terrain", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Player", L"Player", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Sniper", L"Weapon", nullptr), E_FAIL);

	/*____________________________________________________________________
	GameObject 持失.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 持失.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_UI", L"Quest", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Aim", L"Aim", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OptionUI", L"OptionUI", nullptr), E_FAIL);

	//////// 焼戚嬬 //////
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_IconUI", L"IconUI", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_InvenUI", L"InvenUI", nullptr), E_FAIL);

	_uint iType = 0;
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &iType), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 1)), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 2)), E_FAIL);

	for (int i = 0; i < 7; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OnUI", L"OnUI", &(iType = i)), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_RifleUI", L"GunUI", nullptr), E_FAIL);

	for (int i = 0; i < 6; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_EquipUI", L"EquipUI", &(iType = i)), E_FAIL);
	/*____________________________________________________________________
	GameObject 持失.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

CScene_Rail* CScene_Rail::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CScene_Rail* pInstance = new CScene_Rail(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_Rail::Free()
{
	Engine::CScene::Free();
}
