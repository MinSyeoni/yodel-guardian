#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

HRESULT Engine::CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void Engine::CGameObject::Render_Object()
{

}

void Engine::CGameObject::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

}

Engine::CComponent* Engine::CGameObject::Get_Component(const _tchar* pComponentTag, Engine::COMPONENTID eID)
{
	auto		pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

Engine::CComponent* Engine::CGameObject::Find_Component(const _tchar* pComponentTag, Engine::COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));

}

