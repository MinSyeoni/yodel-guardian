#include "Renderer.h"
#include "GameObject.h"
#include "ComponentMgr.h"
#include "GraphicDevice.h"
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

	CGraphicDevice::Get_Instance()->Render_Begin(_rgba(0.0f, 0.0f, 1.0f, 1.f));

	Render_Priority(fTimeDelta);
	Render_NonAlpha(fTimeDelta);
	Render_Alpha(fTimeDelta);
	Render_UI(fTimeDelta);

	CGraphicDevice::Get_Instance()->Render_End();
	Clear_RenderGroup();
}

void CRenderer::Render_Priority(const _float& fTimeDelta)
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}
}

void CRenderer::Render_NonAlpha(const _float& fTimeDelta)
{
	for (auto& pGameObject : m_RenderList[RENDER_NONALPHA])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}
}

void CRenderer::Render_Alpha(const _float& fTimeDelta)
{
	m_RenderList[RENDER_ALPHA].sort([](CGameObject* pSour, CGameObject* pDest)->_bool { return pSour->Get_DepthOfView() > pDest->Get_DepthOfView(); });

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}
}

void CRenderer::Render_UI(const _float& fTimeDelta)
{
	m_RenderList[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool {return pSour->Get_UIDepth() < pDest->Get_UIDepth(); });

	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		pGameObject->Render_GameObject(fTimeDelta);
	}
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

	return S_OK;
}


void CRenderer::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy Renderer");
#endif

	Clear_RenderGroup();
}
