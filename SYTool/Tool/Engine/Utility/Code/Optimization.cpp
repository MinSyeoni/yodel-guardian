#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFrustum(nullptr)
	, m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::COptimization(const COptimization& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pFrustum(rhs.m_pFrustum)
	, m_pQuadTree(rhs.m_pQuadTree)
	, m_bClone(true)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::~COptimization(void)
{

}

HRESULT Engine::COptimization::Ready_Optimization(USINGQUADTREE eType,
													const _ulong& dwCntX,
													const _ulong& dwCntZ)
{
	m_pFrustum = Engine::CFrustum::Create(m_pGraphicDev);
	if (nullptr == m_pFrustum)
		return E_FAIL;

	if (NUSE == eType)
		return S_OK;

	m_pQuadTree = CQuadTree::Create(dwCntX, dwCntZ);
	if (nullptr == m_pQuadTree)
		return E_FAIL;

	return S_OK;
}

_bool Engine::COptimization::Is_InFrustum_ForObject(const _vec3* pWorldPos, const _float fRadius)
{
	return m_pFrustum->Is_InFrustum_ForObject(pWorldPos, fRadius);
}


COptimization* Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												USINGQUADTREE eType,
												const _ulong& dwCntX,
												const _ulong& dwCntZ)
{
	COptimization*	pInstance = new COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization(eType, dwCntX, dwCntZ)))
		Safe_Release(pInstance);
	
	return pInstance;
}

CComponent* Engine::COptimization::Clone(void)
{
	return new COptimization(*this);
}

void Engine::COptimization::Free(void)
{
	if (false == m_bClone)
	{
		Safe_Release(m_pFrustum);
		Safe_Release(m_pQuadTree);
	}
	
	Safe_Release(m_pGraphicDev);
}

void Engine::COptimization::Is_InFrustum_ForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, Engine::INDEX32* pIndex, _ulong* pTriCnt)
{
	m_pFrustum->Is_InFrustum_ForTerrain(pVtxPos, dwCntX, dwCntZ, pIndex, pTriCnt, m_pQuadTree);
}



