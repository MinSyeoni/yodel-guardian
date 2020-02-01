#include "Scene.h"

USING(Engine)

CScene::CScene(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: m_pGraphicDevice(pGraphicDevice)
	, m_pCommandList(pCommandList)
	, m_pObjectMgr(CObjectMgr::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
{
}


CScene::~CScene()
{
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pObjectMgr, -1);

	return m_pObjectMgr->Update_ObjectMgr(fTimeDelta);
}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pObjectMgr, -1);

	return m_pObjectMgr->LateUpdate_ObjectMgr(fTimeDelta);
}

void CScene::Render_Scene(const _float & fTimeDelta)
{
	/*____________________________________________________________________
	Renderer를 통해서 렌더링.
	______________________________________________________________________*/
	if (nullptr == m_pRenderer)
		return;

	m_pRenderer->Render_Renderer(fTimeDelta);

}

void CScene::Free()
{
	m_pRenderer->Clear_RenderGroup();
}
