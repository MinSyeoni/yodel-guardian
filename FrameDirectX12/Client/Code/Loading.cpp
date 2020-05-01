#include "stdafx.h"
#include "Loading.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"
#include "Scene_Logo.h"
#include "GraphicDevice.h"


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

HRESULT CLoading::Mesh_ForStage(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CNaviMesh::Create(m_pGraphicDev, m_pCommandList);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Navi", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Console/", L"Console.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Console", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Player/", L"Player.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Player", ID_STATIC, pComponent), E_FAIL);

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/PlayerLeg/", L"PlayerLeg.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_PlayerLeg", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Missile/", L"Missile.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Missile", ID_STATIC, pComponent), E_FAIL);



	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/SkyDome/", L"SkyDome.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_SkyDome", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Temp/", L"Temp.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Temp", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Weapon/Pistol/", L"Pistol.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Pistol", ID_STATIC, pComponent), E_FAIL);


<<<<<<< HEAD
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Salone/", L"Temp.X");
=======
	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Weapon/Rifle/", L"Rifle.X");
>>>>>>> 636870910ca616aa6afb8d245994959796ba7731
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Mesh_Rifle", ID_STATIC, pComponent), E_FAIL);



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
