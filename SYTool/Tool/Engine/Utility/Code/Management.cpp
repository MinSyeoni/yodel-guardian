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

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Texture.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderSample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderTerrain", pShader), E_FAIL);



	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_ToolTerrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderToolTerrain", pShader), E_FAIL);





	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderMesh", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderShade", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderBlend", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Monster.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_ShaderMonster", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Engine/Utility/Code/Shader_Trail.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Trail", pShader), E_FAIL);


	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	// f가 붙어있는 이유는 실수 값에 해당하는 픽셀의 값을 저장하겠다는 것을 의미, 
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Albedo", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 100.f, 100.f), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Normal", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Normal", 0.f, 100.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Shade", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Shade", 100.f, 0.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_GodRay", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_GodRay", 200.f, 0.f, 100.f, 100.f), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Depth", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Depth", 0.f, 200.f, 100.f, 100.f), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Lim", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Lim", 100.f, 100.f, 100.f, 100.f), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_light", pGraphicDev, ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_light", 100.f, 200.f, 100.f, 100.f), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Defferd", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Defferd", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Defferd", L"Target_Depth"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Defferd", L"Target_Lim"), E_FAIL);
	

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_ConeLight", L"Target_light"), E_FAIL);





	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_GodRay", L"Target_GodRay"), E_FAIL);
	return S_OK;
}

