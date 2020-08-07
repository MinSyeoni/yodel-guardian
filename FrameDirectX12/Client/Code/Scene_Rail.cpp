#include "stdafx.h"
#include "Scene_Rail.h"
#include "DirectInput.h"
#include "Scene_Stage.h"
#include "Management.h"
#include "NaviMesh.h"

#include "Monster.h"
#include "FlameThrower.h"
#include "Dron.h"
#include "DronBullet.h"

#include "LightMgr.h"
#include "FadeOut.h"
#include "Weapon.h"

#include "DronBullet.h"
#include "Scene_Boss.h"

#include "Trigger.h"
#include "QuestUI.h"

CScene_Rail::CScene_Rail(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CScene(pGraphicDevice, pCommandList)
{
	CComponentMgr::Get_Instance()->Delete_Component(L"Mesh_Navi", ID_STATIC);
	Engine::CComponent* pComponent = Engine::CNaviMesh::Create(m_pGraphicDevice, m_pCommandList, L"../../Data/Navi/passage.dat");
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
	FAILED_CHECK_RETURN(Ready_Light(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObjectPrototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerCamera(L"Layer_Camera"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerEnvironment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerGameObject(L"Layer_GameObject"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LayerUI(L"Layer_UI"), E_FAIL);

	// 决企窍绰 固记栏肺 官厕.
	list<CGameObject*>* pQuestUIList = m_pObjectMgr->Get_OBJLIST(L"Layer_UI", L"QuestUI");
	if (pQuestUIList != nullptr)
	{
		for (auto& pSrc : *pQuestUIList)
			dynamic_cast<CQuestUI*>(pSrc)->Set_CurQUEST_TYPE(CQuestUI::QUEST_TYPE6);
	}

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

	// 飘府芭 光栏搁 焊胶 胶抛捞瘤肺 
	list<CGameObject*>* pTriggerList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Trigger");
	if (pTriggerList != nullptr)
	{
		for (auto& pSrc : *pTriggerList)
		{
			if (CTrigger::TRIGGER_BOX == dynamic_cast<CTrigger*>(pSrc)->Get_ColType()
				&& dynamic_cast<CTrigger*>(pSrc)->Get_IsGoBoss())
			{
				m_pObjectMgr->Clear_Layer();
				Engine::CScene* pNewScene = CScene_Boss::Create(m_pGraphicDevice, m_pCommandList);
				Engine::CManagement::Get_Instance()->SetUp_CurrentScene(pNewScene);
				break;
			}
		}
	}

	if (KEY_DOWN(DIK_PGDN))//胶白侩
	{
		m_pObjectMgr->Clear_Layer();
		Engine::CScene* pNewScene = CScene_Boss::Create(m_pGraphicDevice, m_pCommandList);
		Engine::CManagement::Get_Instance()->SetUp_CurrentScene(pNewScene);
	}

}

HRESULT CScene_Rail::Ready_GameObjectPrototype()
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDronBullet::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_DronBullet", pGameObject), E_FAIL);

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

	_vec3 vPos = _vec3(370.f, 0.f, 300.f);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Player", L"Player", &vPos), E_FAIL);

	

	CWeapon::WEAPONSTATE eState = CWeapon::BAG;
	m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Rifle", L"Weapon", &eState);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Sniper", L"Weapon", &eState), E_FAIL);


	CFadeOut::FADETYPE eType = CFadeOut::FADEIN;
	m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_FadeOut", L"FadeOut", &eType);


	Load_StageObject(L"../../Data/StaticObj/passage2.dat", wstrLayerTag);
	Load_MonsterPos(L"../../Data/Collider/Flame.dat", wstrLayerTag);

	Load_MonsterPos(L"../../Data/Collider/DronStart.dat", wstrLayerTag);
	Load_MonsterPos(L"../../Data/Collider/DronStart2.dat", wstrLayerTag);
	Load_MonsterPos(L"../../Data/Collider/DronStart3.dat", wstrLayerTag);
	Load_TriggerPos(L"../../Data/Collider/DronEnd.dat", wstrLayerTag);
	Load_TriggerPos(L"../../Data/Collider/DronEnd2.dat", wstrLayerTag);
	Load_TriggerPos(L"../../Data/Collider/DronEnd3.dat", wstrLayerTag);

	Load_TriggerPos(L"../../Data/Collider/NextBossStage.dat", wstrLayerTag);

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

		if (wstrFilePath == L"../../Data/Collider/DronEnd.dat")
			tColData.iColID = 1;
		else if (wstrFilePath == L"../../Data/Collider/DronEnd2.dat")
			tColData.iColID = 2;
		else if (wstrFilePath == L"../../Data/Collider/DronEnd3.dat")
			tColData.iColID = 3;
		else if (wstrFilePath == L"../../Data/Collider/NextBossStage.dat")
			tColData.iColID = 10;

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

	m_tMeshInfo.MeshTag = L"";
	m_tMeshInfo.iDrawID = 0;

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
	{
		m_tMeshInfo.iDrawID++;
		m_tMeshInfo.MeshTag = L"Flamethrower";
	}
	else if (wstrFilePath == L"../../Data/Collider/DronStart.dat")
	{
		m_tMeshInfo.MeshTag = L"Dron";
		m_tMeshInfo.iDrawID = 1;
		m_tMeshInfo.Rotation = _vec3{ 0.f, -100.f, 0.f };
	}
	else if (wstrFilePath == L"../../Data/Collider/DronStart2.dat")
	{
		m_tMeshInfo.MeshTag = L"Dron";
		m_tMeshInfo.iDrawID = 2;
		m_tMeshInfo.Rotation = _vec3{ 0.f, 100.f, 0.f };
	}
	else if (wstrFilePath == L"../../Data/Collider/DronStart3.dat")
	{
		m_tMeshInfo.MeshTag = L"Dron";
		m_tMeshInfo.iDrawID = 3;
		m_tMeshInfo.Rotation = _vec3{ 0.f, 80.f, 0.f };
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

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Aim", L"Aim", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OptionUI", L"OptionUI", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_InvenUI", L"InvenUI", nullptr), E_FAIL);

	_uint iType = 0;
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_HPBarUI", L"HPBarUI", &(iType = 0)), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_IconUI", L"IconUI", &(iType = 0)), E_FAIL);

	for (int i = 0; i < 7; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OnUI", L"OnUI", &(iType = i)), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_RifleUI", L"GunUI", nullptr), E_FAIL);

	for (int i = 0; i < 9; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_EquipUI", L"EquipUI", &(iType = i)), E_FAIL);

	for (int i = 0; i < 9; ++i)
		FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_QuestUI", L"QuestUI", &(iType = i)), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_MPBarUI", L"MPBarUI", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_MouseUI", L"MouseUI", nullptr), E_FAIL);

	return S_OK;
}

HRESULT CScene_Rail::Ready_Light()
{
	CLight_Manager::Get_Instance()->ClearLigth();

	D3DLIGHT tagLight;
	tagLight.m_eType = LIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	tagLight.m_vDiffuse = _vec4{ 0.3f,0.3f,0.3f,1.0f };
	tagLight.m_vAmbient = _vec4{ 0.2f,0.2f,0.2f,1.0f };
	tagLight.m_vSpecular = _vec4{ 0.5f,0.5f,0.5f,1.0f };
	tagLight.m_vDirection = _vec4{ -1.0f,-1.0f,1.f,1.0f };
	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &tagLight)))
		return E_FAIL;


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
