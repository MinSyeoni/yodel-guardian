#include "ObjectMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
}

CLayer * CObjectMgr::Get_Layer(wstring wstrLayerTag)
{
	/*____________________________________________________________________
	1. 찾고자 하는 Layer Tag를 탐색한다.
	______________________________________________________________________*/
	auto iter_find = m_mapLayer.find(wstrLayerTag);

	/*____________________________________________________________________
	2. 없을 경우 nullptr을 반환한다.
	______________________________________________________________________*/
	if (iter_find == m_mapLayer.end())
		return nullptr;


	return iter_find->second;
}

OBJLIST * CObjectMgr::Get_OBJLIST(wstring wstrLayerTag, wstring wstrObjTag)
{
	/*____________________________________________________________________
	1. 찾고자 하는 Layer Tag값을 탐색한다.
	______________________________________________________________________*/
	CLayer* pInstance = Get_Layer(wstrLayerTag);

	/*____________________________________________________________________
	2. 없을 경우, nullptr을 반환한다.
	______________________________________________________________________*/
	if (pInstance == nullptr)
		return nullptr;


	return pInstance->Get_OBJLIST(wstrObjTag);
}

CGameObject * CObjectMgr::Get_GameObject(wstring wstrLayerTag, wstring wstrObjTag, _int iIdx)
{
	/*____________________________________________________________________
	1. 찾고자 하는 Layer Tag값을 탐색한다.
	______________________________________________________________________*/
	CLayer* pInstance = Get_Layer(wstrLayerTag);

	/*____________________________________________________________________
	2. 없을 경우, nullptr을 반환한다.
	______________________________________________________________________*/
	NULL_CHECK_RETURN(pInstance, nullptr);


	return pInstance->Get_GameObject(wstrObjTag, iIdx);
}

HRESULT CObjectMgr::Add_GameObjectPrototype(wstring wstrPrototypeTag, CGameObject * pGameObject)
{
	if (pGameObject != nullptr)
	{
		/*____________________________________________________________________
		1. map에 ObjTag값이 있는지 탐색한다.
		______________________________________________________________________*/
		auto iter = m_mapObjectPrototype.find(wstrPrototypeTag);

		/*____________________________________________________________________
		2. Tag값이 있을 경우 E_FAIL.
		______________________________________________________________________*/
		if (iter != m_mapObjectPrototype.end())
			return E_FAIL;


		m_mapObjectPrototype.emplace(wstrPrototypeTag, pGameObject);
	}

	return S_OK;
}

CGameObject * CObjectMgr::Find_GameObjectPrototype(wstring wstrObjTag)
{
	/*____________________________________________________________________
	1. map에 ObjTag값이 있는지 탐색한다.
	______________________________________________________________________*/
	auto iter = m_mapObjectPrototype.find(wstrObjTag);

	/*____________________________________________________________________
	2. Tag값이 있을 경우 GameObject Prototpye 반환.
	______________________________________________________________________*/
	if (iter == m_mapObjectPrototype.end())
		return nullptr;

	return iter->second->Clone_GameObject(nullptr);
}

HRESULT CObjectMgr::Add_GameObject(wstring wstrLayerTag, wstring wstrPrototypeTag, wstring wstrObjTag, void* pArg)
{
	/*____________________________________________________________________
	1. Layer를 탐색한다.
	______________________________________________________________________*/
	auto iter_find_layer = m_mapLayer.find(wstrLayerTag);

	/*____________________________________________________________________
	2. 해당 Layer가 없을 경우 종료.
	______________________________________________________________________*/
	if (iter_find_layer == m_mapLayer.end())
		return E_FAIL;

	/*____________________________________________________________________
	3. GambObject의 Prototype을 찾는다. 없다면 E_FAIL.
	______________________________________________________________________*/
	auto iter_Prototype = m_mapObjectPrototype.find(wstrPrototypeTag);
	if (iter_Prototype == m_mapObjectPrototype.end())
		return E_FAIL;

	CGameObject* pGameObject = iter_Prototype->second->Clone_GameObject(pArg);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	/*____________________________________________________________________
	4. Layer에 GameObject 추가.
	______________________________________________________________________*/
	return iter_find_layer->second->Add_GameObject(wstrObjTag, pGameObject);
}

CGameObject * CObjectMgr::Get_NewGameObject(wstring wstrPrototypeTag, wstring wstrObjTag, void * pArg)
{

	/*____________________________________________________________________
	1. GambObject의 Prototype을 찾는다. 없다면 E_FAIL.
	______________________________________________________________________*/
	auto iter_Prototype = m_mapObjectPrototype.find(wstrPrototypeTag);
	if (iter_Prototype == m_mapObjectPrototype.end())
		return nullptr;

	CGameObject* pGameObject = iter_Prototype->second->Clone_GameObject(pArg);
	if (pGameObject == nullptr) return nullptr;
	return pGameObject;

}



_int CObjectMgr::Update_ObjectMgr(const _float & fTimeDelta)
{
	_int	iEnd = 0;
	_float fTime = fTimeDelta;
	fTime = m_bIsTime * fTime;

	for (auto& iter : m_mapLayer)
	{
		iEnd = iter.second->Update_Layer(fTime);

		if (iEnd & 0x80000000)
			return -1;
	}

	return NOEVENT;
}

_int CObjectMgr::LateUpdate_ObjectMgr(const _float & fTimeDelta)
{
	_int	iEnd = 0;
	_float fTime = fTimeDelta;
	fTime = m_bIsTime * fTime;
	for (auto& iter : m_mapLayer)
	{
		iEnd = iter.second->LateUpdate_Layer(fTime);

		if (iEnd & 0x80000000)
			return -1;
	}

	return NOEVENT;
}

void CObjectMgr::Render_ObjectMgr(const _float & fTimeDelta)
{
	for (auto& iter : m_mapLayer)
		iter.second->Render_Layer(fTimeDelta);
}

HRESULT CObjectMgr::Delete_GameObject(wstring wstrLayerTag, wstring wstrObjTag, _int iIdx)
{
	/*____________________________________________________________________
	Layer Tag값을 탐색한다.
	_____________________________________________________________________*/
	auto iter_find = m_mapLayer.find(wstrLayerTag);

	if (iter_find == m_mapLayer.end())
		return E_FAIL;

	/*____________________________________________________________________
	탐색에 성공할 경우 해당 Key값의 OBJLIST에서 해당 index삭제.
	______________________________________________________________________*/
	return iter_find->second->Delete_GameObject(wstrObjTag, iIdx);
}

HRESULT CObjectMgr::Clear_OBJLIST(wstring wstrLayerTag, wstring wstrObjTag)
{
	/*____________________________________________________________________
	Layer Tag값을 탐색한다.
	_____________________________________________________________________*/
	auto iter_find = m_mapLayer.find(wstrLayerTag);

	if (iter_find == m_mapLayer.end())
		return E_FAIL;

	/*____________________________________________________________________
	탐색에 성공할 경우 해당 OBJLIST 삭제.
	______________________________________________________________________*/
	return iter_find->second->Clear_OBJLIST(wstrObjTag);
}

void CObjectMgr::Clear_Layer()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());

	m_mapLayer.clear();
}

void CObjectMgr::Clear_Prototype()
{
	for_each(m_mapObjectPrototype.begin(), m_mapObjectPrototype.end(), CDeleteMap());

	m_mapObjectPrototype.clear();
}

void CObjectMgr::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy ObjectMgr");
#endif

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	/*____________________________________________________________________
	Release 모드에서 종료시 ERROR 발생.
	주석하면 원활하게 종료됨. 원인을 잘 모르겠음.
	______________________________________________________________________*/
	for_each(m_mapObjectPrototype.begin(), m_mapObjectPrototype.end(), CDeleteMap());
	m_mapObjectPrototype.clear();
}
