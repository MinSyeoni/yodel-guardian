#include "stdafx.h"
#include "Scene_Rail.h"
#include "DirectInput.h"
#include "Scene_Stage.h"
#include "Management.h"
#include "NaviMesh.h"

CScene_Rail::CScene_Rail(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
	CComponentMgr::Get_Instance()->Delete_Component(L"Mesh_Navi", ID_STATIC);
	Engine::CComponent* pComponent = Engine::CNaviMesh::Create(m_pGraphicDevice, m_pCommandList, L"../../Data/Navi/pass.dat");
	CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Navi", ID_STATIC, pComponent);
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

HRESULT CScene_Rail::Ready_ComponentPrototype()
{
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Delete_Component(L"Mesh_Navi", ID_STATIC), E_FAIL)
		return E_FAIL;
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CNaviMesh::Create(m_pGraphicDevice, m_pCommandList, L"../../Data/Navi/pass.dat");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Navi", ID_STATIC, pComponent), E_FAIL);

	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerCamera(wstring wstrLayerTag)
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
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_DynamicCamera", L"DynamicCamera", nullptr), E_FAIL);

	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerEnvironment(wstring wstrLayerTag)
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

	return S_OK;
}

HRESULT CScene_Rail::Ready_LayerGameObject(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	GameLogic Layer 积己.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Terrain", L"Terrain", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Player", L"Player", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Sniper", L"Weapon", nullptr), E_FAIL);

	Load_StageObject(L"../../Data/StaticObj/pass.dat", wstrLayerTag);
	//Load_MonsterPos(L"../../Data/Collider/Flame.dat", wstrLayerTag);
	//Load_TriggerPos(L"../../Data/Collider/Flame.dat", wstrLayerTag);

	/*____________________________________________________________________
	GameObject 积己.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	return S_OK;
}

void CScene_Rail::Load_TriggerPos(const wstring& wstrFilePath, wstring wstrLayerTag)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK(hFile);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	COLLIDER tColData = {};

	while (true)
	{
		ReadFile(hFile, &tColData, sizeof(COLLIDER), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		if (3 == tColData.iOptionID)
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Trigger", L"Trigger", &tColData);
	}
	CloseHandle(hFile);
}

void CScene_Rail::Load_MonsterPos(const wstring& wstrFilePath, wstring wstrLayerTag)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK(hFile);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	COLLIDER tColData = {};

	while (true)
	{
		ReadFile(hFile, &tColData, sizeof(COLLIDER), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		InitMesh_FromFile(wstrFilePath);

		m_tMeshInfo.Pos = tColData.vCenter;
		m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Monster", m_tMeshInfo.MeshTag, &m_tMeshInfo);
	}
	CloseHandle(hFile);
}

void CScene_Rail::InitMesh_FromFile(const std::wstring& wstrFilePath)
{
	if (wstrFilePath == L"../../Data/Collider/Flame.dat")
		m_tMeshInfo.MeshTag = L"Flame";
	else if (wstrFilePath == L"../../Data/Collider/Flame.dat")
	{
		m_tMeshInfo.MeshTag == L"Dron";
		m_tMeshInfo.iMeshID = 0;
	}
}

void CScene_Rail::Load_StageObject(const wstring& wstrFilePath, wstring wstrLayerTag)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK(hFile);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	MESHDATA tObjData = {};

	//DeleteAll_GameObject(wstrLayerTag, L"ItemObject");
	//DeleteAll_GameObject(wstrLayerTag, L"Medi_Syringe");
	//DeleteAll_GameObject(wstrLayerTag, L"Medi_Bandage");
	//DeleteAll_GameObject(wstrLayerTag, L"Medi_vaccine");
	//DeleteAll_GameObject(wstrLayerTag, L"LobbyDoor");
	//DeleteAll_GameObject(wstrLayerTag, L"PassageDoor");
	//DeleteAll_GameObject(wstrLayerTag, L"CardKey");
	//DeleteAll_GameObject(wstrLayerTag, L"MapObject");

	int			 iTagLength = 0;

	while (true)
	{
		ReadFile(hFile, &iTagLength, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tObjData, sizeof(MESHDATA), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		m_tMeshInfo.MeshTag = tObjData.m_MeshTag;
		m_tMeshInfo.Pos = tObjData.vPos;
		m_tMeshInfo.Rotation = tObjData.vRotate;
		m_tMeshInfo.Scale = tObjData.vScale;

		if (m_tMeshInfo.MeshTag == L"Siren.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_LightObject", L"LightObject", &m_tMeshInfo);//炼疙贸府
		else if (m_tMeshInfo.MeshTag == L"medikit.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_ItemObject", L"ItemObject", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"medikit_syringe.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Medi_Syringe", L"Medi_Syringe", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"medikit_bandage.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Medi_Bandage", L"Medi_Bandage", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"medikit_vaccine.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Medi_Medicine", L"Medi_vaccine", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"door1.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_LobbyDoor", L"LobbyDoor", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"door2.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_PassageDoor", L"PassageDoor", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"card.X")
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_CardKey", L"CardKey", &m_tMeshInfo);
		else
			m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_MapObject", L"MapObject", &m_tMeshInfo);
	}
	CloseHandle(hFile);
}

void CScene_Rail::DeleteAll_GameObject(wstring& wstrLayerTag, wstring wstrObjTag)
{
	list<CGameObject*>* pObjList;
	if (nullptr != CObjectMgr::Get_Instance()->Get_OBJLIST(wstrLayerTag, wstrObjTag))
	{
		pObjList = CObjectMgr::Get_Instance()->Get_OBJLIST(wstrLayerTag, wstrObjTag);
		for (auto& pSrc : *pObjList)
			pSrc->Dead_GameObject();
		pObjList->clear();
	}
}

HRESULT CScene_Rail::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 积己.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);


	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_UI", L"Quest", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Aim", L"Aim", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OptionUI", L"OptionUI", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_InvenUI", L"InvenUI", nullptr), E_FAIL);

	_uint iType = 0;
	//////// 酒捞能 //////
	for (int i = 0; i < 3; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_IconUI", L"IconUI", &(iType = i)), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 0)), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 1)), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 2)), E_FAIL);

	for (int i = 0; i < 7; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OnUI", L"OnUI", &(iType = i)), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_RifleUI", L"GunUI", nullptr), E_FAIL);

	for (int i = 0; i < 6; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_EquipUI", L"EquipUI", &(iType = i)), E_FAIL);
	/*____________________________________________________________________
	GameObject 积己.
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
//	CFrustom::Get_Instance()->Destroy_Instance();

	Engine::CScene::Free();
}
