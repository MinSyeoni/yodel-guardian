#include "stdafx.h"
#include "Loading.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"


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

	Mesh_ForStage();
	Texture_ForStage();

	m_bFinish = true;
	return S_OK;
}

HRESULT CLoading::Mesh_ForStage(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/StaticMesh/Missile/", L"B.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Missile", ID_STATIC, pComponent), E_FAIL);


	pComponent = Engine::CMesh::Create(m_pGraphicDev, m_pCommandList, L"../../Resource/DynamicMesh/Weapon/", L"Weapon.X");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Pig", ID_STATIC, pComponent), E_FAIL);


	return S_OK;
}

HRESULT CLoading::Texture_ForStage(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, m_pCommandList, TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/StonesArray%d.dds", 1);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Texture", ID_STATIC, pComponent), E_FAIL);

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
