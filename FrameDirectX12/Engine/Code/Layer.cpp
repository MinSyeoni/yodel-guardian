#include "Layer.h"



CLayer::CLayer()
{
}


CLayer::~CLayer()
{
}

OBJLIST * CLayer::Get_OBJLIST(wstring wstrObjTag)
{
	/*____________________________________________________________________
	1. map에서 찾고자 하는 OBJLIST의 Key값을 탐색한다.
	______________________________________________________________________*/
	auto iter_find = m_mapObjLst.find(wstrObjTag);

	/*____________________________________________________________________
	2. 찾고자 하는 OBJLIST가 없다면 nullptr을 반환.
	______________________________________________________________________*/
	if (iter_find == m_mapObjLst.end())
		return nullptr;

	return &(iter_find->second);
}

CGameObject * CLayer::Get_GameObject(wstring wstrObjTag, _int iIdx)
{
	/*____________________________________________________________________
	1. map에서 찾고자 하는 OBJLIST의 Key값을 탐색한다.
	______________________________________________________________________*/
	OBJLIST* pObjLst = Get_OBJLIST(wstrObjTag);

	/*____________________________________________________________________
	2. 찾고자 하는 OBJLIST가 없다면 nullptr을 반환.
	______________________________________________________________________*/
	NULL_CHECK_RETURN(pObjLst, nullptr);

	/*____________________________________________________________________
	3. 찾고자하는 GameObject를 OBJLIST에서 탐색.
	______________________________________________________________________*/
	if (pObjLst->size() <= (_ulong)iIdx)
		return nullptr;

	OBJITER iter_begin = pObjLst->begin();

	for (_int i = 0; i < iIdx; ++i)
		++iter_begin;

	return (*iter_begin);
}


HRESULT CLayer::Add_GameObject(wstring wstrObjTag, CGameObject * pGameObject)
{
	if (pGameObject != nullptr)
	{
		/*____________________________________________________________________
		1. map에 ObjTag값이 있는지 탐색한다.
		______________________________________________________________________*/
		auto iter = m_mapObjLst.find(wstrObjTag);

		/*____________________________________________________________________
		2. 없을 경우, 임시 OBJLIST객체를 생성해준 후에 GameObject를 넣어준다.
		______________________________________________________________________*/
		if (iter == m_mapObjLst.end())
			m_mapObjLst[wstrObjTag] = OBJLIST();


		m_mapObjLst[wstrObjTag].push_back(pGameObject);
	}

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(wstring wstrObjTag, _int iIdx)
{
	/*____________________________________________________________________
	삭제할 OBJLIST의 Key값을 탐색한다.
	______________________________________________________________________*/
	auto iter_find = m_mapObjLst.find(wstrObjTag);

	/*____________________________________________________________________
	탐색에 실패할 경우 종료
	______________________________________________________________________*/
	if (iter_find == m_mapObjLst.end())
		return E_FAIL;

	/*____________________________________________________________________
	탐색 성공시, 해당 Key값의 OBJLIST에서 해당 index삭제.
	______________________________________________________________________*/
	CGameObject* pGameObject = Get_GameObject(wstrObjTag, iIdx);
	if (nullptr != pGameObject)
	{
		pGameObject->Dead_GameObject();

		return S_OK;
	}

	return E_FAIL;
}

HRESULT CLayer::Clear_OBJLIST(wstring wstrObjTag)
{
	/*____________________________________________________________________
	삭제할 OBJLIST의 Key값을 탐색한다.
	______________________________________________________________________*/
	auto iter_find = m_mapObjLst.find(wstrObjTag);

	/*____________________________________________________________________
	탐색에 실패할 경우 종료
	______________________________________________________________________*/
	if (iter_find == m_mapObjLst.end())
		return E_FAIL;

	/*____________________________________________________________________
	탐색 성공시, 해당 Key값의 OBJLIST 삭제.
	______________________________________________________________________*/
	for (auto& pGameObj : iter_find->second)
		pGameObj->Dead_GameObject();

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	for (auto& MyPair : m_mapObjLst)
	{
		OBJITER iter_begin = MyPair.second.begin();
		OBJITER iter_end = MyPair.second.end();

		for (; iter_begin != iter_end;)
		{
			if (MyPair.second.empty())
				break;

			int iEvent = (*iter_begin)->Update_GameObject(fTimeDelta);

			if (DEADOBJ == iEvent)
			{
				Engine::Safe_Release(*iter_begin);
				iter_begin = MyPair.second.erase(iter_begin);
			}
			else
				++iter_begin;
		}

	}

	return NOEVENT;
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	for (auto& MyPair : m_mapObjLst)
	{
		for (auto& pGameObject : MyPair.second)
			pGameObject->LateUpdate_GameObject(fTimeDelta);
	}

	return NOEVENT;
}

void CLayer::Render_Layer(const _float & fTimeDelta)
{
	for (auto& MyPair : m_mapObjLst)
	{
		for (auto& pGameObject : MyPair.second)
			pGameObject->Render_GameObject(fTimeDelta);
	}
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free()
{
	for (auto& MyPair : m_mapObjLst)
	{
		for (auto& pGameObject : MyPair.second)
			Engine::Safe_Release(pGameObject);

		MyPair.second.clear();
	}

	m_mapObjLst.clear();
}
