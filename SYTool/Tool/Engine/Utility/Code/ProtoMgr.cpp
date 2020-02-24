#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

Engine::CProtoMgr::CProtoMgr(void)
{

}

Engine::CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT Engine::CProtoMgr::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*	pPrototype = Find_Prototype(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.emplace(pProtoTag, pInstance);

	return S_OK;
}

CComponent* Engine::CProtoMgr::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent*		pProtoType = Find_Prototype(pProtoTag);

	if (nullptr == pProtoType)
		return nullptr;

	return pProtoType->Clone();
}

CComponent* Engine::CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void Engine::CProtoMgr::Free(void)
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

