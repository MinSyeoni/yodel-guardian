#include "ColliderMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CColliderMgr)

CColliderMgr::CColliderMgr()
{
}

const list<CCollider*>& CColliderMgr::Get_ColliderList(const SHAPE eShape, const COLLIDER_TAG eTag)
{
	if (eShape == BOX)
		return m_listBoxCol[eTag];
	else if (eShape == SPHERE)
		return m_listSphereCol[eTag];
	return m_listBoxCol[eTag];
}

void CColliderMgr::Clear_Collider(void)
{
	for (_int i = 0; i < TAG_END; i++)
	{
		for (auto& pCol : m_listBoxCol[i])
			Safe_Release(pCol);
		m_listBoxCol[i].clear();
		for (auto& pCol : m_listSphereCol[i])
			Safe_Release(pCol);
		m_listSphereCol[i].clear();
	}
}

HRESULT CColliderMgr::Add_Collider(const COLLIDER_TAG eTag, CCollider * pCollider)
{
	if (pCollider == nullptr)
		return E_FAIL;

	if (pCollider->Get_CollShape() == CCollider::COL_SPHERE)
		m_listSphereCol[eTag].push_back(pCollider);
	else if (pCollider->Get_CollShape() == CCollider::COL_BOX)
		m_listBoxCol[eTag].push_back(pCollider);
	else
		return E_FAIL;

	pCollider->AddRef();

	return NOERROR;
}

void CColliderMgr::Free(void)
{
	Clear_Collider();
}
