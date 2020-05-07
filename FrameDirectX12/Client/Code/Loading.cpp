#include "stdafx.h"
#include "Loading.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "GraphicDevice.h"
#include "ActionCamera.h"

CLoading::CLoading(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:m_pGraphicDev(pGraphicDevice)
	,m_bFinish(false)
	,m_pCommandList(pCommandList)
{

}

CLoading::~CLoading(void)
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	CFont* pFont = static_cast<CScene_Logo*>( CManagement::Get_Instance()->Get_CurScene())->Get_Font();
	pFont->Set_Text(L"Mesh_Loading");
	pFont->Set_Color(D2D1::ColorF::Red);
	Mesh_ForStage();

	pFont->Set_Text(L"Texture_Loading");
	pFont->Set_Color(D2D1::ColorF::Red);
	Texture_ForStage();

	pFont->Set_Text(L"Shader_Loading");
	pFont->Set_Color(D2D1::ColorF::Red);
	Load_Shader();

	pFont->Set_Text(L"Camera_Loading");
	pFont->Set_Color(D2D1::ColorF::Blue);
	Load_Camera();



	pFont->Set_Text(L"Loading_Finish");
	pFont->Set_Color(D2D1::ColorF::Red);
	m_bFinish = true;
	return S_OK;
}

HRESULT CLoading::Load_Shader()
{
	CComponent*	pComponent = nullptr;

	pComponent = CShader_Mesh::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Mesh", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Mesh::Create(DEVICE, m_pCommandList,CShader_Mesh::ALPHA);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_MeshAlpha", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Mesh::Create(DEVICE, m_pCommandList,CShader_Mesh::SKYDOME);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_SkyDome", ID_STATIC, pComponent), E_FAIL);

	// Shader_ColorBuffer


	return S_OK;
}

HRESULT CLoading::Load_Camera()
{
	wstring fileName = L"../../../SYTool/Tool/Data/Camera/CameraData.dat";
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK_RETURN(hFile, E_FAIL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;
	DWORD dwByte = 0;
	TCHAR strName[128] = L"";
	int iStrLength = 0;
	float PlayTime = 0;
	int iSpotEye = 0;
	int iSpotAt = 0;
	_vec3 vEye = _vec3{0.f,0.f,0.f};
	_vec3 vAt = _vec3{0.f,0.f,0.f};
	while (true)
	{
		ReadFile(hFile, &iStrLength, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &strName, sizeof(TCHAR) * iStrLength, &dwByte, nullptr);
		ReadFile(hFile, &PlayTime, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &iSpotEye, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iSpotAt, sizeof(int), &dwByte, nullptr);
		if (0 == dwByte)
		{
			break;
		}
		vector<_vec3> vecEyePosList;
		vector<_vec3> vecAtPosList;

		for (int i = 0; i < iSpotEye; i++)
		{
			ReadFile(hFile, &vEye, sizeof(_vec3), &dwByte, nullptr);
			vecEyePosList.push_back(vEye);
		}
		for (int i = 0; i < iSpotAt; i++)
		{
			ReadFile(hFile, &vAt, sizeof(_vec3), &dwByte, nullptr);
			vecAtPosList.push_back(vAt);
		}
		
		pGameObject = CActionCamera::Create(m_pGraphicDev, m_pCommandList, vecEyePosList, vecAtPosList, strName, PlayTime);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(CObjectMgr::Get_Instance()->Add_GameObjectPrototype(strName,pGameObject), E_FAIL);



		if (0 == dwByte)
		{
			break;
		}

	}

	CloseHandle(hFile);



	return S_OK;
}

HRESULT CLoading::Mesh_ForStage(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CNaviMesh::Create(m_pGraphicDev, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Navi", ID_STATIC, pComponent), E_FAIL);




	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Player/", L"Player.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Player", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Missile/", L"Missile.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Missile", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Temp/", L"Temp.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Console", ID_STATIC, pComponent), E_FAIL);



	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/SkyDome/", L"SkyDome.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_SkyDome", ID_STATIC, pComponent), E_FAIL);




	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Weapon/Rifle/", L"Rifle.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Rifle", ID_STATIC, pComponent), E_FAIL);
	////////////////NPC///////////////
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Salone/", L"Salone.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Salone", ID_STATIC, pComponent), E_FAIL);


	/////////////// 맵 옵젝 //////////////////////
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"ammocase.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"ammocase.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"board.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Board.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book1.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Book1.X", ID_STATIC, pComponent), E_FAIL);

	//pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"Book2.X");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Book2.X", ID_STATIC, pComponent), E_FAIL);

	//pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book3.X");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"book3.X", ID_STATIC, pComponent), E_FAIL);

	//pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book4.X");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"book4.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book5.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Book5.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book6.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Book6.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"book7.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Book7.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"booksSet.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"booksSet.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"bottle01.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"bottle01.X", ID_STATIC, pComponent), E_FAIL);

	//pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"chairoffice.X");
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"chairoffice.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"box.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Box.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"chairoffice.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"chairoffice.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"computer1.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Computer1.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"computer2.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Computer2.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"containerBlue.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"containerBlue.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"containerGray.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"containerGray.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"containerGreen.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"containerGreen.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"containerRed.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"containerRed.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"datapad.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"datapad.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"desk3.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"desk3.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"deskLamp.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"deskLamp.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"lathe.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Lathe.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"map1.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"map1.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"map1_roof.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"map1_roof.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"medcrate.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"medCrate.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"paper01.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"paper01.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"paper02.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"paper02.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"planter.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"planter.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"siren.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Siren.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"solar.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"solar.X", ID_STATIC, pComponent), E_FAIL);

	// 테스트용
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"test.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"test.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/temp/", L"a.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"a.X", ID_STATIC, pComponent), E_FAIL);

	/////////////// 몬스터 //////////////////////
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Monster/Stage1/Flamethrower/", L"Flamethrower.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Flamethrower", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Monster/Stage1/Zombi/", L"Zombi.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Zombi", ID_STATIC, pComponent), E_FAIL);

	return S_OK;
}

HRESULT CLoading::Texture_ForStage(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/Terrain/Terrain%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Terrain", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/TerrainN/TerrainN%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_TerrainN", ID_STATIC, pComponent), E_FAIL);


	// UI 이미지 넣을 것. 
	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/mission%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_UI", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/aim.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Aim", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/Player.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_PlayerUI", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/PlayerHp.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_PlayerHP", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/assaultRifle.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Rifle", ID_STATIC, pComponent), E_FAIL);

	/////////////////////////데메지
	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/DamageBlood/blood%d.dds", 3);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_DamageBlood", ID_STATIC, pComponent), E_FAIL);


	return S_OK;
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		pLoading->Loading_ForStage();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());


	return 0;
}

CLoading * CLoading::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDevice,pCommandList);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Safe_Release(pInstance);


	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}
