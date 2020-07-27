#include "stdafx.h"
#include "Scene_Stage.h"
#include "GraphicDevice.h"
#include "RectObject.h"
#include "CubeObject.h"
#include "Terrain.h"
#include "StaticObject.h"
#include "GlassObject.h"
#include "Dynamic_Object.h"
#include "Player.h"
#include "LightMgr.h"
#include "SkyDome.h"
#include "Frustom.h"
#include "Pistol.h"
#include "Rifle.h"

#include "UI.h"	// 나중에 ui로 묶을것
#include "Aim.h"
#include "HPBar.h"
#include "IconUI.h"
#include "InvenUI.h"
#include "GunUI.h"
#include "Trigger.h"
#include "NpcWords.h"
#include "OptionUI.h"
#include "OnUI.h"
#include "EquipUI.h"

#include "MapObject.h"
#include "HPKit.h"
#include "Monster.h"
#include "Salone.h"
#include "Trigger.h"
#include "LobbyDoor.h"
#include "PassageDoor.h"
#include "CardKey.h"

#include "LightObject.h"
#include "DamageBlood.h"
#include "Normandy.h"
#include "DistortionEffect.h"
#include "Shepard.h"
#include "Sniper.h"
#include "FadeOut.h"

#include "StaticCamera.h"
#include "PlayerPoint.h"


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
	tagLight.m_vDiffuse = _vec4{ 1.0f,1.0f,1.0f,1.0f };
	tagLight.m_vAmbient = _vec4{ 0.2f,0.2f,0.2f,1.0f };
	tagLight.m_vSpecular = _vec4{ 0.5f,0.5f,0.5f,1.0f };
	tagLight.m_vDirection= _vec4{ -1.0f,-1.0f,1.f,1.0f };
	if(FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &tagLight)))
	   return E_FAIL;


	return S_OK;
}

HRESULT CScene_Stage::Ready_Scene()
{
#ifdef _DEBUG
	COUT_STR("Ready Scene_Stage");
#endif

	srand((unsigned int)time(0));

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
	ShowCursor(true);
	CFrustom::Get_Instance()->Update_Frustom_Manager();
	return Engine::CScene::Update_Scene(fTimeDelta);
}

_int CScene_Stage::LateUpdate_Scene(const _float & fTimeDelta)
{
	// 대화창 enum 값 바꾸는 것. 나중에 NPC 상호작용으로 바꿔주세요.

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
	원형 객체를 생산하는 작업 수행.
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

	pGameObject = CGlassObject::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_GlassObject", pGameObject), E_FAIL);

	pGameObject = CRifle::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Rifle", pGameObject), E_FAIL);

	pGameObject = CSniper::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Sniper", pGameObject), E_FAIL);

	////////////////////NPC/////////////////////////
	pGameObject = CSalone::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Salone", pGameObject), E_FAIL);

	////////////////////////////////// MAP /////////////////////////////////////////////
	pGameObject = CMapObject::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_MapObject", pGameObject), E_FAIL);
	
	pGameObject = CHPKit::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_ItemObject", pGameObject), E_FAIL);

	pGameObject = CMonster::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Monster", pGameObject), E_FAIL);

	pGameObject = CLobbyDoor::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_LobbyDoor", pGameObject), E_FAIL);

	pGameObject = CPassageDoor::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_PassageDoor", pGameObject), E_FAIL);

	pGameObject = CCardKey::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_CardKey", pGameObject), E_FAIL);

	////////////////////////////////// UI /////////////////////////////////////////////
	pGameObject = CUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_UI", pGameObject), E_FAIL);

	pGameObject = CAim::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Aim", pGameObject), E_FAIL);

	pGameObject = COptionUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_OptionUI", pGameObject), E_FAIL);

	pGameObject = CIconUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_IconUI", pGameObject), E_FAIL);

	pGameObject = CHPBar::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_HPBarUI", pGameObject), E_FAIL);

	pGameObject = CGunUI::Create(m_pGraphicDevice, m_pCommandList, L"Prototype_Texture_Rifle");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_RifleUI", pGameObject), E_FAIL);

	pGameObject = CNpcWords::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_NpcBoard", pGameObject), E_FAIL);

	pGameObject = CInvenUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_InvenUI", pGameObject), E_FAIL);

	pGameObject = COnUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_OnUI", pGameObject), E_FAIL);

	pGameObject = CEquipUI::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_EquipUI", pGameObject), E_FAIL);

	pGameObject = CDamageBlood::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_DamageBlood", pGameObject), E_FAIL);

	////////////////////////// 트리거 /////////////////////////////////////
	pGameObject = CTrigger::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Trigger", pGameObject), E_FAIL);

	/////////////////////////조명쓰////////////////////////////////////////

	pGameObject = CLightObject::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_LightObject", pGameObject), E_FAIL);

	pGameObject = CNormandy::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Normandy", pGameObject), E_FAIL);

	pGameObject = CDistortionEffect::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_DistortEffect", pGameObject), E_FAIL);

	pGameObject = CShepard::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_Shepard", pGameObject), E_FAIL);

	pGameObject = CFadeOut::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_FadeOut", pGameObject), E_FAIL);
	 
	pGameObject = CStaticCamera::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_StaticCamera", pGameObject), E_FAIL);

	pGameObject = CPlayerPoint::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_PlayerPoint", pGameObject), E_FAIL);


	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerEnvironment(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	Environment Layer 생성.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 생성.
	m_pObjectMgr->Add_GameObject(wstrLayerTag, wstrObjTag);
	______________________________________________________________________*/

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_SkyDome", L"SkyDome", nullptr), E_FAIL);
	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerCamera(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	Environment Layer 생성.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	/*____________________________________________________________________
	GameObject 생성.
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
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Normandy", L"Normandy", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Terrain", L"Terrain", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Player", L"Player", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Sniper", L"Weapon", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Salone", L"Salone", nullptr), E_FAIL);//For.Salone
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Shepard", L"Shepard", nullptr), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_PlayerPoint", L"PlayerPoint", nullptr), E_FAIL);

	//C:\Users\user\Documents\GitHub\yodel-guardian\FrameDirectX12\Data\StaticObj\mapAddoutside_1_test																	 //Prototype_MapObject
	Load_StageObject(L"../../Data/StaticObj/SY_Kit_Test.dat");
	
	// Monster
	//Load_MonsterPos(L"../../Data/Collider/Flame.dat");
	Load_MonsterPos(L"../../Data/Collider/Zombi.dat");
	Load_MonsterPos(L"../../Data/Collider/Zombi2.dat");
	Load_MonsterPos(L"../../Data/Collider/Zombi3.dat");
	Load_MonsterPos(L"../../Data/Collider/Zombi4.dat");

	// Trigger
	Load_TriggerPos(L"../../Data/Collider/Trigger.dat");
	Load_TriggerPos(L"../../Data/Collider/Trigger2.dat");
	Load_TriggerPos(L"../../Data/Collider/Trigger3.dat");

	return S_OK;
}

HRESULT CScene_Stage::Ready_LayerUI(wstring wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObjectMgr, E_FAIL);

	/*____________________________________________________________________
	UI Layer 생성.
	______________________________________________________________________*/
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_pObjectMgr->Add_Layer(wstrLayerTag, pLayer);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_UI", L"Quest", nullptr), E_FAIL);

	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_Aim", L"Aim", nullptr), E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObject(wstrLayerTag, L"Prototype_OptionUI", L"OptionUI", nullptr), E_FAIL);

	//////// 아이콘 //////
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



	return S_OK;
}

void CScene_Stage::Load_MonsterPos(const wstring& wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK(hFile);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	COLLIDER tColData = {};

	//if (wstrFilePath == L"../../Data/Collider/Flame.dat")
	////	m_tMeshInfo.MeshTag = L"Flamethrower";
	//	m_tMeshInfo.MeshTag = L"Dron";
	if (wstrFilePath == L"../../Data/Collider/Zombi.dat")
	{
		m_tMeshInfo.MeshTag = L"Zombi";
		m_tMeshInfo.iDrawID = 1;
		m_tMeshInfo.iMeshID = 4;	// 달리는거
	}
	else if (wstrFilePath == L"../../Data/Collider/Zombi2.dat")
	{
		m_tMeshInfo.MeshTag = L"Zombi";
		m_tMeshInfo.iDrawID = 2;
		m_tMeshInfo.iMeshID = 3;	// 밑에서 올라오기
	}
	else if (wstrFilePath == L"../../Data/Collider/Zombi3.dat")
	{
		m_tMeshInfo.MeshTag = L"Zombi";
		m_tMeshInfo.iDrawID = 3;
		m_tMeshInfo.iMeshID = 2;	// 위에서 떨어지기
	}
	else if (wstrFilePath == L"../../Data/Collider/Zombi4.dat")
	{
		m_tMeshInfo.MeshTag = L"Zombi";
		m_tMeshInfo.iDrawID = 4;
		m_tMeshInfo.iMeshID = rand() % 2;	// 누워있다가 or 엎드려있다가 일어나기
	}

	while (true)
	{
		ReadFile(hFile, &tColData, sizeof(COLLIDER), &dwByte, nullptr);

		if (dwByte == 0)
			break;

	//	m_tMeshInfo.iMeshID = rand() % 4;
		if(m_tMeshInfo.MeshTag == L"Dron")
			m_tMeshInfo.iMeshID = 0;
		else
		{
			m_tMeshInfo.Rotation.y = rand() % 360 - 180;
		}

		m_tMeshInfo.Pos = tColData.vCenter;
		m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Monster", m_tMeshInfo.MeshTag, &m_tMeshInfo);
	}
	CloseHandle(hFile);
}

void CScene_Stage::Load_TriggerPos(const wstring& wstrFilePath)
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

		if (wstrFilePath == L"../../Data/Collider/Trigger.dat")
			tColData.iColID = 1;
		else if (wstrFilePath == L"../../Data/Collider/Trigger2.dat")
			tColData.iColID = 2;
		else if (wstrFilePath == L"../../Data/Collider/Trigger3.dat")
			tColData.iColID = 3;

		if(3 == tColData.iOptionID)
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Trigger", L"Trigger", &tColData);
	}
	CloseHandle(hFile);
}

void CScene_Stage::Load_StageObject(const wstring& wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK(hFile);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	MESHDATA tObjData = {};

	// 나중에 옵젝 로드할 떄 모든 옵젝 삭제하기 위해서 
	//if (nullptr != Engine::Get_Layer(SCENE_STATIC, LAYER_STATICOBJECT))
	//{
	//	m_GameObjLstTemp = Engine::Get_Layer(SCENE_STATIC, LAYER_STATICOBJECT)->Get_list();
	//	for (auto& pGameObj : m_GameObjLstTemp)
	//	{
	//		dynamic_cast<CStaticObj*>(pGameObj)->IsStaticObjDead(true);
	//	}
	//	m_GameObjLstTemp.clear();
	//}

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

		if(m_tMeshInfo.MeshTag ==L"Siren.X" )
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_LightObject", L"LightObject", &m_tMeshInfo);//조명처리
		else if (m_tMeshInfo.MeshTag == L"medikit.X")
		{
			m_tMeshInfo.iMeshID++;
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_ItemObject", L"ItemObject", &m_tMeshInfo);
		}
		else if (m_tMeshInfo.MeshTag == L"door1.X")
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_LobbyDoor", L"LobbyDoor", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"door2.X")
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_PassageDoor", L"PassageDoor", &m_tMeshInfo);
		else if (m_tMeshInfo.MeshTag == L"card.X")
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_CardKey", L"CardKey", &m_tMeshInfo);
		else
			m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_MapObject", L"MapObject", &m_tMeshInfo);
	}
	CloseHandle(hFile);
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
