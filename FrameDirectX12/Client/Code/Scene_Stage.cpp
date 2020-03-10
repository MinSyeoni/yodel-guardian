#include "stdafx.h"
#include "Scene_Stage.h"
#include "GraphicDevice.h"
#include "RectObject.h"
#include "CubeObject.h"
#include "Terrain.h"
#include "StaticObject.h"
#include "Dynamic_Object.h"
#include "Player.h"
#include "LightMgr.h"
#include "SkyDome.h"
#include "Frustom.h"
#include "Pistol.h"
CScene_Stage::CScene_Stage(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
}


CScene_Stage::~CScene_Stage()
{
}

HRESULT CScene_Stage::Ready_LightInfo()
{
	D3DLIGHT tagLight;
	tagLight.m_eType = LIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tagLight.m_vDiffuse = _vec4{ 0.3f,0.3f,0.3f,1.0f };
	tagLight.m_vAmbient = _vec4{ 0.1f,0.1f,0.1f,1.0f };
	tagLight.m_vSpecular = _vec4{ 0.7f,0.7f,0.7f,1.0f };
	tagLight.m_vDirection= _vec4{ -1.0f,-1.0f,1.f,1.0f };
	if(FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &tagLight)))
	   return E_FAIL;


	tagLight.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
	tagLight.m_vDiffuse = _vec4{ 1.0f,1.0f,0.0f,1.0f };
	tagLight.m_vAmbient = _vec4{ 0.35f,0.35f,0.35f,1.0f };
	tagLight.m_vSpecular = _vec4{ 0.3f,0.3f,0.3f,1.0f };
	tagLight.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
	tagLight.m_vPosition = _vec4{ 300.f,10.f,400.f,0.f };
	tagLight.m_fRange = 100.f;


	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &tagLight)))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Stage::Ready_Scene()
{
#ifdef _DEBUG
	COUT_STR("Ready Scene_Stage");
#endif

	FAILED_CHECK_RETURN(Ready_GameObjectPrototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerCamera(L"Layer_Camera"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerEnvironment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerGameObject(L"Layer_GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerUI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	return S_OK;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	CFrustom::Get_Instance()->Update_Frustom_Manager();
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LateUpdate_Scene(const _float & fTimeDelta)
{
	return Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CScene_Stage::Render_Scene(const _float & fTimeDelta)
{
	CScene::Render_Scene(fTimeDelta);
}

HRESULT CScene_Stage::Ready_GameObjectPrototype()
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	盔屈 按眉甫 积魂窍绰 累诀 荐青.
	______________________________________________________________________*/
	CGameObject* pGameObject = nullptr;



	pGameObject = CTerrain::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Terrain", pGameObject), E_FAIL);

	pGameObject = CStaticObject::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_StaticObject", pGameObject), E_FAIL);


	pGameObject = CPlayer::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Player", pGameObject), E_FAIL);

	pGameObject = CSkyDome::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_SkyDome", pGameObject), E_FAIL);

	pGameObject = CPistol::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Pistol", pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerEnvironment(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	Environment Layer 积己.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 积己.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_SkyDome", L"SkyDome", nullptr), E_FAIL);
	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerCamera(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	Environment Layer 积己.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 积己.
	______________________________________________________________________*/
	// DynamicCamera
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_DynamicCamera", L"DynamicCamera",nullptr), E_FAIL);

	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerGameObject(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	// Terrain
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Terrain", L"Terrain", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Player", L"Player",nullptr), E_FAIL);

	MeshInfo tMeshInfo;
	tMeshInfo.MeshTag = L"Mesh_Missile";
	tMeshInfo.Pos = _vec3(350.f,-3.f,400.f );
	tMeshInfo.Rotation = _vec3( 0.f,0.f,0.f );
	tMeshInfo.Scale = _vec3(0.1f, 0.1f, 0.1f);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_StaticObject", L"Static", &tMeshInfo), E_FAIL);

	tMeshInfo.MeshTag = L"Mesh_Console";
	tMeshInfo.Pos = _vec3(250.f, 0.f, 300.f);
	tMeshInfo.Rotation = _vec3(0.f, 0.f, 0.f);
	tMeshInfo.Scale = _vec3(0.1f, 0.1f, 0.1f);

	

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_StaticObject", L"Static", &tMeshInfo), E_FAIL);


	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Pistol", L"Weapon", &tMeshInfo), E_FAIL);
	return S_OK;
		
}

HRESULT CScene_Stage::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 积己.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	/*____________________________________________________________________
	GameObject 积己.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/



	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	//FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_BackGround", L"BackGround", nullptr), E_FAIL);

	return S_OK;
}

CScene_Stage * CScene_Stage::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CScene_Stage* pInstance = new CScene_Stage(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CScene_Stage::Free()
{
	CFrustom::Get_Instance()->Destroy_Instance();
	Engine::CScene::Free();
	
}
