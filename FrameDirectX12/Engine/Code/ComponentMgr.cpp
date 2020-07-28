#include "ComponentMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CComponentMgr)

CComponentMgr::CComponentMgr()
{
}


CComponentMgr::~CComponentMgr()
{
}

CComponent * CComponentMgr::Get_Component(wstring wstrPrototypeTag, COMPONENTID eID)
{
	CComponent* pComponent = Find_Component(wstrPrototypeTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

HRESULT CComponentMgr::Add_ComponentPrototype(wstring wstrPrototypeTag, const COMPONENTID & eID, CComponent* pComponent)
{
	if (eID > COMPONENTID::ID_END)
		return E_FAIL;

	/*____________________________________________________________________
	Tag값이 존재하면 map에 넣지 않는다.
	______________________________________________________________________*/
	auto iter = m_mapComponent[eID].find(wstrPrototypeTag);
	if (iter != m_mapComponent[eID].end())
		return E_FAIL;

	m_mapComponent[eID].emplace(wstrPrototypeTag, pComponent);

	return S_OK;
}

CComponent * CComponentMgr::Clone_Component(wstring wstrPrototypeTag, const COMPONENTID & eID)
{
	if (eID > COMPONENTID::ID_END)
		return nullptr;

	CComponent* pComponent = Find_Component(wstrPrototypeTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent->Clone();
}

CComponent * CComponentMgr::Clone_Collider(wstring wstrPrototypeTag, const COMPONENTID & eID, const CCollider::COLLSHAPE & eColShape, const _bool & bIsAttatch,  CMesh * pMesh, const _vec3 & vAddPos, const _vec3 & vAngle, const _float & fRadius, const _vec3 & vScale, CGameObject * pOwner)
{
	if (eID > COMPONENTID::ID_END)
		return nullptr;

	CComponent* pComponent = Find_Component(wstrPrototypeTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	CComponent* pClone = static_cast<CCollider*>(pComponent)->Clone_Collider(bIsAttatch, pMesh, vAddPos, vAngle, fRadius, vScale, pOwner);
	if (nullptr == pClone)
		return nullptr;

	return pClone;
}

HRESULT CComponentMgr::Delete_Component(wstring wstrPrototypeTag, COMPONENTID eID)
{

	auto iter_find = m_mapComponent[eID].find(wstrPrototypeTag);

	if (iter_find == m_mapComponent[eID].end())
		return E_FAIL;

	m_mapComponent[eID].erase(iter_find);

	return S_OK;
}

CComponent * CComponentMgr::Find_Component(wstring wstrPrototypeTag, const COMPONENTID & eID)
{
	auto iter_find = m_mapComponent[eID].find(wstrPrototypeTag);

	if (iter_find == m_mapComponent[eID].end())
		return nullptr;

	return iter_find->second;
}

void CComponentMgr::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy ComponentMgr");
#endif

	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}
}
