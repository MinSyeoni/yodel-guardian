#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	isEnd;

	for (auto& iter : m_mapOBjList)
	{
		auto iter_begin = iter.second.begin();
		for (; iter_begin != iter.second.end();)
		{
			int iEvent = (*iter_begin)->Update_Object(fTimeDelta);
			if (iEvent == 1)
			{
				Safe_Release(*iter_begin);
				iter_begin = iter.second.erase(iter_begin);
			}
			else
				++iter_begin;
		}
		
	}

	return 0;
}

void Engine::CLayer::Render_Layer(void)
{
	
	for (auto&iter : m_mapOBjList)
	{
		for (auto& List : iter.second)
			List->Render_Object();
	}
}

Engine::CLayer* Engine::CLayer::Create(void)
{
	CLayer*	pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
	{
		MSG_BOX("Layer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto&iter : m_mapOBjList)
	{
		for (auto& List : iter.second)
			Safe_Release(List);


	}
}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr != pGameObject)
	{

		auto iter = m_mapOBjList.find(pObjTag);
		if (iter == m_mapOBjList.end())
		{
			m_mapOBjList[pObjTag] = OBJLIST();

		}
		m_mapOBjList[pObjTag].push_back(pGameObject);
	}

	return S_OK;
}



CGameObject * CLayer::Get_GameObj(const _tchar * pObjTag)
{

	auto iter = m_mapOBjList.find(pObjTag);
	if (iter == m_mapOBjList.end())
		return nullptr;

	return m_mapOBjList[pObjTag].front();

}

list<CGameObject*>* CLayer::Get_GameObjLst(const _tchar * pObjTag)
{
	auto iter = m_mapOBjList.find(pObjTag);
	if (iter == m_mapOBjList.end())
		return nullptr;

	return &m_mapOBjList[pObjTag];



}

