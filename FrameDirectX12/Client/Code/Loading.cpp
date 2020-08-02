#include "stdafx.h"
#include "Loading.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "GraphicDevice.h"
#include "ActionCamera.h"
#include "Effect.h"
#include "LoadingBar.h"

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
	pFont->Set_Color(D2D1::ColorF::DarkGray);
	Mesh_ForStage();

	pFont->Set_Text(L"Effect_Loading");
	pFont->Set_Color(D2D1::ColorF::DarkGray);
	Load_Effect();


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.7f);
	pFont->Set_Text(L"Texture_Loading");
	pFont->Set_Color(D2D1::ColorF::DarkGray);
	Texture_ForStage();


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.8f);
	pFont->Set_Text(L"Shader_Loading");
	pFont->Set_Color(D2D1::ColorF::DarkGray);
	Load_Shader();

	pFont->Set_Text(L"Camera_Loading");
	pFont->Set_Color(D2D1::ColorF::DarkGray);
	Load_Camera();



	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(1.0f);
	pFont->Set_Text(L"Loading_Finish");
	pFont->Set_Color(D2D1::ColorF::DarkRed);
	
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

	pComponent = CShader_Dissolve::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Dissolve", ID_STATIC, pComponent), E_FAIL);




	pComponent = CShader_UI::Create(DEVICE, m_pCommandList, CShader_UI::HPBAR);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_HPBAR", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_UI::Create(DEVICE, m_pCommandList, CShader_UI::HPBAR1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_HPBAR1", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_UI::Create(DEVICE, m_pCommandList, CShader_UI::HPBAR2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_HPBAR2", ID_STATIC, pComponent), E_FAIL);





	pComponent = CShader_LightAcc::Create(DEVICE, m_pCommandList, LIGHTTYPE::D3DLIGHT_DIRECTIONAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_LightDirect", ID_STATIC, pComponent), E_FAIL);


	pComponent = CShader_LightAcc::Create(DEVICE, m_pCommandList, LIGHTTYPE::D3DLIGHT_POINT);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_LightPoint", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_LightAcc::Create(DEVICE, m_pCommandList, LIGHTTYPE::D3DLIGHT_SPOT);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_LightSpot", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Destortion::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Destortion", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Terrain::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Terrain", ID_STATIC, pComponent), E_FAIL);


	pComponent = CShader_Effect::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Effect", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Effect::Create(DEVICE, m_pCommandList, CShader_Effect::ALPHABLENDTEX);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_EffectAlphaBlend", ID_STATIC, pComponent), E_FAIL);

	pComponent = CShader_Effect::Create(DEVICE, m_pCommandList, CShader_Effect::FADEOUT);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_FadeOut", ID_STATIC, pComponent), E_FAIL);



	//pComponent = CShader_Mesh::Create(DEVICE, m_pCommandList, CShader_Mesh::SHADOW);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_MeshShadow", ID_STATIC, pComponent), E_FAIL);


	return S_OK;
}

HRESULT CLoading::Load_Effect()
{

	CGameObject* pObj = CEffect::Create(DEVICE, m_pCommandList, L"../../Data/Effect/GunFire.dat");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	FAILED_CHECK_RETURN(CObjectMgr::Get_Instance()->Add_GameObjectPrototype(L"Prototype_Effect_GunFire", pObj),E_FAIL);


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

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Player/", L"Player.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Player", ID_STATIC, pComponent), E_FAIL);

	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.3f);

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


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.32f);

	/////////////// 맵 옵젝 //////////////////////
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"ammocase.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"ammocase.X", ID_STATIC, pComponent), E_FAIL);
	
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"barricade.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"barricade.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"board.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Board.X", ID_STATIC, pComponent), E_FAIL);


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.4f);

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

	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.43f);
	
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
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Desk3.X", ID_STATIC, pComponent), E_FAIL);

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
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Solar.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"Missile.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Missile.X", ID_STATIC, pComponent), E_FAIL);
	
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"card.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"card.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"apollo.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"apollo.X", ID_STATIC, pComponent), E_FAIL);


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.47f);



	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"passage_top.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"passage_top.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"passage_side.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"passage_side.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"passage_bottom.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"passage_bottom.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"passage_test.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"passage_test.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"minibase.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"minibase.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"medikit_syringe.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"medikit_syringe.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"medikit_bandage.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"medikit_bandage.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"antenna.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"antenna.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"medikit_vaccine.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"medikit_vaccine.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"medikit.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"medikit.X", ID_STATIC, pComponent), E_FAIL);


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.52f);



	// 절벽
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"RoundCliffL.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"RoundCliffL.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"RoundCliffS.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"RoundCliffS.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"CornerCliffL.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"CornerCliffL.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"CornerCliffS.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"CornerCliffS.X", ID_STATIC, pComponent), E_FAIL);
	
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Stage1_2/", L"SquareCliffL.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"SquareCliffL.X", ID_STATIC, pComponent), E_FAIL);

	// 다이나믹 오브젝트
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Stage1/", L"door1.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"door1.X", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Stage1/", L"door2.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"door2.X", ID_STATIC, pComponent), E_FAIL);

	/////////////// 몬스터 //////////////////////
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Monster/Stage1/Flamethrower/", L"Flamethrower.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Flamethrower", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Monster/Stage1/Zombi/", L"Zombi.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Zombi", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Normandy/", L"Normandy.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Normandy", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Monster/Stage2/Dron/", L"Dron.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Dron", ID_STATIC, pComponent), E_FAIL);
	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.58f);
	//왜곡용
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/DistortDisk/", L"DistortDisk.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"DistortDisk", ID_STATIC, pComponent), E_FAIL);

	//SHepard
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Shepard/", L"Shepard.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Shepard", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Ken/", L"Ken.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Ken", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Weapon/Sniper/", L"Sniper.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Sniper", ID_STATIC, pComponent), E_FAIL);

	//MESH_POINT
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/PlayerPoint/", L"point_top.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Point", ID_STATIC, pComponent), E_FAIL);


	static_cast<CLoadingBar*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingBar"))->SetGauge(0.65f);

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

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/aim%d.dds", 2);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Aim", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_player_background.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_PlayerIcon", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_player_blue.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_PlayerMP", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_player_white.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_PlayerHP", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_colleague1_white.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_HP_colleague1", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_colleague2_white.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_HP_colleague2", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_colleague1_background.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_colleague1Icon", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/blood_colleague2_background.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_colleague2Icon", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/assaultRifle.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Rifle", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/talk.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_NpcTalkBoard", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/option.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_OptionUI", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/on.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_OnUI", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/off.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_OffUI", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/medi_icon.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_MedicineIcon", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/Grass_Blade_L.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_AttackDamageL", ID_STATIC, pComponent), E_FAIL);

	// e 버튼
	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/e1.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_E_Conversation", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/e2.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_E_Equip", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/e3.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_E_Hiding", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/UI/e4.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_E_DoorOpen", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/DamageBlood/blood%d.dds", 3);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_DamageBlood", ID_STATIC, pComponent), E_FAIL);

	/////////////////////////데메지
	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Effect/Alphablend/Alphablend%d.dds", 8);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Effect_AlphaBlend", ID_STATIC, pComponent), E_FAIL);

	/////// r디졸브 
	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/Dissolve/Dissolve%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture_Dissolve", ID_STATIC, pComponent), E_FAIL);

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
