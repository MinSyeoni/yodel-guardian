#include "Management.h"
#include "Renderer.h"

#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
	: m_pScene(nullptr)
{
}

Engine::CManagement::~CManagement(void)
{
	Free();
}

CLayer * CManagement::Get_Layer(const _tchar * pLayertag)
{
	if(m_pScene==nullptr)
	return nullptr;

	return m_pScene->Get_Layer(pLayertag);
}

HRESULT Engine::CManagement::Set_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);

	CRenderer::GetInstance()->Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_uint Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();

}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}

HRESULT Engine::CManagement::Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*		pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../Engine/Utility/Code/Shader_Texture.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderSample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Engine/Utility/Code/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderTerrain", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Engine/Utility/Code/Shader_ToolTerrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderToolTerrain", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../Engine/Utility/Code/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderMesh", pShader), E_FAIL);

	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	return S_OK;
}

