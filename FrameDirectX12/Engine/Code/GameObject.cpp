#include "GameObject.h"
#include "ObjectMgr.h"
#include "Renderer.h"

USING(Engine)

CGameObject::CGameObject(ID3D12Device* pGraphicDevice,
						 ID3D12GraphicsCommandList*	pCommandList)
	: m_pGraphicDevice(pGraphicDevice)
	, m_pCommandList(pCommandList)
	, m_pObjectMgr(CObjectMgr::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
	, m_pComponentMgr(CComponentMgr::Get_Instance())
{
	
	
	
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice)
	, m_pCommandList(rhs.m_pCommandList)
	, m_pObjectMgr(rhs.m_pObjectMgr)
	, m_pComponentMgr(rhs.m_pComponentMgr)
	, m_pRenderer(rhs.m_pRenderer)
	, m_bIsDead(rhs.m_bIsDead)
	, m_bIsLateInit(rhs.m_bIsLateInit)
	, m_fViewZ(rhs.m_fViewZ)
	, m_UIDepth(rhs.m_UIDepth)
{


}


CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(wstring wstrComponentTag, COMPONENTID eID)
{
	CComponent* pComponent = Find_Component(wstrComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

HRESULT CGameObject::Ready_GameObject()
{

	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);



	m_pInfoCom = static_cast<CInfo*>(m_pComponentMgr->Clone_Component(L"Prototype_Info", COMPONENTID::ID_STATIC));
	if (nullptr != m_pInfoCom)
		m_mapComponent[ID_STATIC].emplace(L"Com_Info", m_pInfoCom);

	return S_OK;
}

HRESULT CGameObject::LateInit_GameObject()
{
	if (!m_bIsLateInit)
	{
		FAILED_CHECK_RETURN(this->LateInit_GameObject(), E_FAIL);
		m_bIsLateInit = true;
	}

	return S_OK;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);


	return 0;
}

_int CGameObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void CGameObject::Render_GameObject(const _float & fTimeDelta)
{
}

HRESULT CGameObject::Add_Component()
{
	return S_OK;
}

void CGameObject::Compute_ViewZ(const _vec4 * pPosInWorld)
{
	//_matrix		matView = m_pGraphic_Interface->Get_Transform(CGraphic_Interface::MATRIX_VIEW);

	//matView = XMMatrixInverse(nullptr, matView);

	//_vec4		vCamPositionInWorld;

	//memcpy(&vCamPositionInWorld, &matView.r[3], sizeof(_vec4));

	//_vec4		vDir = *pPosInWorld - vCamPositionInWorld;

	//m_fViewZ = XMVectorGetX(XMVector3Length(vDir));
}


CComponent * CGameObject::Find_Component(wstring wstrComponentTag, const COMPONENTID & eID)
{
	auto iter_find = m_mapComponent[eID].find(wstrComponentTag);

	if (iter_find == m_mapComponent[eID].end())
		return nullptr;

	return iter_find->second;
}


void CGameObject::Free()
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}
}
