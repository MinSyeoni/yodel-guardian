#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr(void)
	: m_pmatResource(nullptr)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pmatResource)
		return E_FAIL;

	m_pmatResource = new MAPRESOURCES[wSize];

	m_wCount = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID,
	const _tchar* pPath,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	if (nullptr == m_pmatResource)
	{
		MSG_BOX("Resources Container not Reserved");
		return E_FAIL;
	}

	CResources* pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;

	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;

	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;

	case BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_TERRAINTEXPATH:
		pResources = CTerrainTex::Create(pGraphicDev, pPath, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
	}

	if (nullptr == pResources)
		return E_FAIL;

	m_pmatResource[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx,
	const _tchar* pResourcesTag)
{
	auto		iter = find_if(m_pmatResource[wContainerIdx].begin(),
		m_pmatResource[wContainerIdx].end(), CTag_Finder(pResourcesTag));

	if (iter == m_pmatResource[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free(void)
{

	for (_ushort i = 0; i < m_wCount; ++i)
	{
		for_each(m_pmatResource[i].begin(), m_pmatResource[i].end(), CDeleteMap());
		m_pmatResource[i].clear();
	}

	Safe_Delete_Array(m_pmatResource);
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx,
	const _tchar* pBufferTag)
{
	CResources* pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<CVIBuffer*>(pResources)->Render();
}

Engine::CResources* Engine::CResourcesMgr::Clone_Resources(const _ushort& wContainerIdx,
	const _tchar* pResourceTag)
{
	if (nullptr == m_pmatResource)
		return nullptr;

	CResources* pPrototype = Find_Resources(wContainerIdx, pResourceTag);

	return pPrototype->Clone();

}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pTextureTag,
	TEXTURETYPE eType,
	const _tchar* pPath,
	const _uint& iCnt /*= 1*/)
{
	if (nullptr == m_pmatResource)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources* pResource = Find_Resources(wContainerIdx, pTextureTag);
	if (nullptr != pResource)
		return E_FAIL;

	pResource = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	if (nullptr == pResource)
		return E_FAIL;

	m_pmatResource[wContainerIdx].emplace(pTextureTag, pResource);

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	if (nullptr == m_pmatResource)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}
	CResources* pResource = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResource)
		return E_FAIL;

	switch (eType)
	{
	case TYPE_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case TYPE_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case TYPE_NAVI:
		pResource = CNaviMesh::Create(pGraphicDev);
		break;
	}

	if (nullptr == pResource)
		return E_FAIL;

	m_pmatResource[wContainerIdx].emplace(pMeshTag, pResource);

	return S_OK;
}

