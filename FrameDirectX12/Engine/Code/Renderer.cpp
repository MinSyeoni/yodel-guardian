#include "Renderer.h"
#include "GameObject.h"
#include "ComponentMgr.h"
#include "GraphicDevice.h"
#include "LightMgr.h"
#include "DirectInput.h"
USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
	: m_pComponentMgr(CComponentMgr::Get_Instance())
{
}


CRenderer::~CRenderer()
{
}

HRESULT CRenderer::Ready_Renderer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	m_pGraphicDevice = pGraphicDevice;
	NULL_CHECK_RETURN(m_pGraphicDevice, E_FAIL);

	m_pCommandList = pCommandList;
	NULL_CHECK_RETURN(m_pCommandList, E_FAIL);

	FAILED_CHECK_RETURN(Ready_ShaderPrototype(), E_FAIL);

	m_DifferdTarget = CTarget::Create(m_pGraphicDevice, m_pCommandList);
	m_LightTarget = CLightTarget::Create(m_pGraphicDevice, m_pCommandList);
	m_ShadowDepthTarget = CShadowDepthTarget::Create(m_pGraphicDevice, m_pCommandList);


	m_pBlendBuffer = CRcTex::Create(m_pGraphicDevice, m_pCommandList);
	m_pBlendShader = CShader_Blend::Create(m_pGraphicDevice, m_pCommandList);

	m_pShadowShader = CShader_Shadow::Create(m_pGraphicDevice, m_pCommandList);

	m_pDownSampleBuffer = CRcTex::Create(m_pGraphicDevice, m_pCommandList);
	m_pDownSampleShader = CShader_DownSample::Create(m_pGraphicDevice, m_pCommandList);
	m_DownSampleTarget = CDownSampleTarget::Create(m_pGraphicDevice, m_pCommandList);
	return S_OK;
}

HRESULT CRenderer::Add_Renderer(const RENDERGROUP & eRenderID, CGameObject * pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_RenderList[eRenderID].push_back(pGameObject);

	return S_OK;
}

void CRenderer::Render_Renderer(const _float& fTimeDelta)
{

	CGraphicDevice::Get_Instance()->Render_Begin(_rgba(1.0f, 1.0f, 1.0f, 1.f));
	Render_ShadowDepth(); //그림자깊이 기록.

	Render_Priority(fTimeDelta);

	Render_NonAlpha(fTimeDelta);

	Render_LightAcc();

	Render_PostPoressing();

	Render_Blend();

	Render_Alpha(fTimeDelta);
	Render_UI(fTimeDelta);


	if (KEY_DOWN(8))
		m_blsShowTarget = !m_blsShowTarget;
	if (m_blsShowTarget == true)//디퍼드 타켓랜더
	{
		m_ShadowDepthTarget->Render_RenderTarget();
		m_DifferdTarget->Render_RenderTarget();
		m_LightTarget->Render_RenderTarget();
		m_DownSampleTarget->Render_RenderTarget();
	}
	CGraphicDevice::Get_Instance()->End_ResetCmdList();


	CGraphicDevice::Get_Instance()->Render_TextBegin();
	Render_Font(fTimeDelta);
	CGraphicDevice::Get_Instance()->Render_TextEnd();


	CGraphicDevice::Get_Instance()->Render_End();
	Clear_RenderGroup();
}

HRESULT CRenderer::Render_ShadowDepth()
{
	m_ShadowDepthTarget->SetUp_OnGraphicDev();
	
	m_pShadowShader->Begin_Shader();
	for (auto& pGameObject : m_RenderList[RENDER_SHADOWDEPTH])
	{
		pGameObject->Render_ShadowDepth(m_pShadowShader);
	}

	m_pShadowShader->End_Shader();
	m_ShadowDepthTarget->Release_OnGraphicDev();
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	CLight_Manager*		pLight_Manager = CLight_Manager::Get_Instance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	m_LightTarget->SetUp_OnGraphicDev();

	pLight_Manager->Render_Light(m_DifferdTarget->GetTargetTexture());

	m_LightTarget->Release_OnGraphicDev();
	return S_OK;
}

HRESULT CRenderer::Render_Priority(const _float& fTimeDelta)
{


	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}

	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha(const _float& fTimeDelta)
{

	m_DifferdTarget->SetUp_OnGraphicDev(0);

	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}

	m_DifferdTarget->Release_OnGraphicDev(0);

	return S_OK;
}

HRESULT CRenderer::Render_Alpha(const _float& fTimeDelta)
{
	m_RenderList[RENDER_ALPHA].sort([](CGameObject* pSour, CGameObject* pDest)->_bool { return pSour->Get_DepthOfView() > pDest->Get_DepthOfView(); });

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}

	return S_OK;
}

HRESULT CRenderer::Render_UI(const _float& fTimeDelta)
{
	m_RenderList[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool {return pSour->Get_UIDepth() < pDest->Get_UIDepth(); });

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}

	return S_OK;
}
HRESULT CRenderer::Render_Font(const _float & fTimeDelta)
{
	for (auto& pGameObject : m_RenderList[RENDER_FONT])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (!m_blsBlendInit)
	{
		vector<ComPtr<ID3D12Resource>> pDiffedTarget = m_DifferdTarget->GetTargetTexture();
		vector<ComPtr<ID3D12Resource>> pShadeTarget = m_LightTarget->GetTargetTexture();

		vector<ComPtr<ID3D12Resource>> pBlendTarget;
		pBlendTarget.push_back(pDiffedTarget[0]);
		pBlendTarget.push_back(pShadeTarget[0]);
		pBlendTarget.push_back(pShadeTarget[1]);

		m_pBlendShader->Set_Shader_Texture(pBlendTarget);

		m_blsBlendInit = true;
	}

	m_pBlendShader->Begin_Shader();
	m_pBlendBuffer->Begin_Buffer();

	m_pBlendShader->End_Shader();
	m_pBlendBuffer->Render_Buffer();



	return S_OK;
}

HRESULT CRenderer::Render_PostPoressing()
{

	Render_DownSampleing();//블러 다운샘플링.

	return S_OK;
}

HRESULT CRenderer::Render_DownSampleing()
{
	if (!m_bIsDownSampleInit)
	{
		vector<ComPtr<ID3D12Resource>> pDiffedTarget = m_DifferdTarget->GetTargetTexture();
		vector<ComPtr<ID3D12Resource>> pShadeTarget = m_LightTarget->GetTargetTexture();
		vector<ComPtr<ID3D12Resource>> pDownSampleTarget;
		pDownSampleTarget.push_back(pDiffedTarget[4]);
		m_pDownSampleShader->Set_Shader_Texture(pDownSampleTarget);
		m_bIsDownSampleInit = true;
	}
	m_DownSampleTarget->SetUp_OnGraphicDev();

	m_pDownSampleShader->Begin_Shader();
	m_pDownSampleBuffer->Begin_Buffer();

	m_pDownSampleShader->End_Shader();
	m_pDownSampleBuffer->Render_Buffer();


	m_DownSampleTarget->Release_OnGraphicDev();

	return S_OK;
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
		m_RenderList[i].clear();
}


HRESULT CRenderer::Ready_ShaderPrototype()
{
	CComponent*	pShader	= nullptr;

	// Shader_ColorBuffer
	pShader = CShader_ColorBuffer::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Shader_ColorBuffer", ID_STATIC, pShader), E_FAIL);


	pShader = CShader_Default::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Shader_Default", ID_STATIC, pShader), E_FAIL);


	pShader = CShader_DefaultTex::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Shader_DefaultTex", ID_STATIC, pShader), E_FAIL);


	
	pShader = CShader_LightAcc::Create(m_pGraphicDevice, m_pCommandList, LIGHTTYPE::D3DLIGHT_DIRECTIONAL);
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Shader_LightDirect", ID_STATIC, pShader), E_FAIL);


	pShader = CShader_LightAcc::Create(m_pGraphicDevice, m_pCommandList, LIGHTTYPE::D3DLIGHT_POINT);
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(m_pComponentMgr->Add_ComponentPrototype(L"Prototype_Shader_LightPoint", ID_STATIC, pShader), E_FAIL);


	pShader = CShader_Terrain::Create(DEVICE, m_pCommandList);
	NULL_CHECK_RETURN(pShader, E_FAIL);

	FAILED_CHECK_RETURN(CComponentMgr::Get_Instance()->Add_ComponentPrototype(L"Prototype_Shader_Terrain", ID_STATIC, pShader), E_FAIL);
	return S_OK;
}


void CRenderer::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy Renderer");
#endif
	Safe_Release(m_LightTarget);
	Safe_Release(m_DifferdTarget);
	Safe_Release(m_ShadowDepthTarget);
	Safe_Release(m_DownSampleTarget);
	//블랜드
	Safe_Release(m_pBlendBuffer);
	Safe_Release(m_pBlendShader);
	Safe_Release(m_pShadowShader);

	//다운샘플
	Safe_Release(m_pDownSampleBuffer);
	Safe_Release(m_pDownSampleShader);

	CLight_Manager::Get_Instance()->Destroy_Instance();
	Clear_RenderGroup();
}
