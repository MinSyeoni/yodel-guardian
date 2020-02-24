#include "QuadTree.h"
#include "Frustum.h"

USING(Engine)

Engine::CQuadTree::CQuadTree(void)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
	ZeroMemory(m_dwCorner, sizeof(_ulong) * CORNER_END);
}

Engine::CQuadTree::~CQuadTree(void)
{

}

HRESULT Engine::CQuadTree::Ready_QuadTree(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCorner[CORNER_LT] = dwCntX * dwCntZ - dwCntX;
	m_dwCorner[CORNER_RT] = dwCntX * dwCntZ - 1;
	m_dwCorner[CORNER_LB] = 0;
	m_dwCorner[CORNER_RB] = dwCntX - 1;

	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;

	Make_ChildQuadTree();
		
	return S_OK;
}

void Engine::CQuadTree::CullingForTerrain(CFrustum* pFrustum, 
										const _vec3* pVtxPos, 
										INDEX32* pIndex, 
										_ulong* pTriCnt)
{
	m_fRadius = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_LT]] - pVtxPos[m_dwCenter]));

	_bool bIsIn = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCenter], m_fRadius);

	if (true == bIsIn)
	{
		//if (nullptr == m_pChild[0])
		if(LevelOfDetail(pFrustum->Get_GraphicDev(), pVtxPos))
		{
			_bool		bIsNeighborDraw[NEIGHBOR_END] = { true, true, true, true };

			for (_ulong i = 0; i < NEIGHBOR_END; ++i)
			{
				if (nullptr != m_pNeighbor[i])
					bIsNeighborDraw[i] = m_pNeighbor[i]-> LevelOfDetail(pFrustum->Get_GraphicDev(), pVtxPos);
			}
			_bool	bVtxInFrustum[3] = { false };

			bVtxInFrustum[0] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_RT]]);
			bVtxInFrustum[2] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_RB]]);

			if (true == bVtxInFrustum[0] ||
				true == bVtxInFrustum[1] ||
				true == bVtxInFrustum[2])
			{
				if (bIsNeighborDraw[NEIGHBOR_TOP] && bIsNeighborDraw[NEIGHBOR_RIGHT])
				{
					if (nullptr == m_pChild[CHILD_LT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];

						++(*pTriCnt);
					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);
					}
				
				}
				else
				{
					if (false == bIsNeighborDraw[NEIGHBOR_TOP])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 1;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);
					}

					if (false == bIsNeighborDraw[NEIGHBOR_RIGHT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) >> 1;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_RT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCenter;

						++(*pTriCnt);
					}
				}			
			}

			bVtxInFrustum[0] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_RB]]);
			bVtxInFrustum[2] = pFrustum->Is_IsFrustum(&pVtxPos[m_dwCorner[CORNER_LB]]);

			if (true == bVtxInFrustum[0] ||
				true == bVtxInFrustum[1] ||
				true == bVtxInFrustum[2])
			{
				if (bIsNeighborDraw[NEIGHBOR_BOTTOM] && bIsNeighborDraw[NEIGHBOR_LEFT])
				{
					if (nullptr == m_pChild[CHILD_LT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];

						++(*pTriCnt);
					}
					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];

						++(*pTriCnt);
					}
				}

				else
				{
					if (false == bIsNeighborDraw[NEIGHBOR_BOTTOM])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 1;

						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];

						++(*pTriCnt);
					}

					if (false == bIsNeighborDraw[NEIGHBOR_LEFT])
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;

						++(*pTriCnt);

						pIndex[*pTriCnt]._0 = m_dwCenter;
						pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
						pIndex[*pTriCnt]._2 = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) >> 1;

						++(*pTriCnt);
					}

					else
					{
						pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
						pIndex[*pTriCnt]._1 = m_dwCenter;
						pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];

						++(*pTriCnt);
					}
				}
			}

			return;
		}

		for (_ulong i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->CullingForTerrain(pFrustum, pVtxPos, pIndex, pTriCnt);
		}
	}
}

void Engine::CQuadTree::Make_ChildCorner(CHILD eType, const _ulong& dwPLT, const _ulong& dwPRT, const _ulong& dwPLB, const _ulong& dwPRB, const _ulong& dwPCenter, _ulong* pLT, _ulong* pRT, _ulong* pLB, _ulong* pRB)
{
	_ulong	dwLT, dwRT, dwLB, dwRB;
	_ulong	dwLC, dwTC, dwRC, dwBC;


	dwLC = (dwPLT + dwPLB) >> 1;
	dwTC = (dwPLT + dwPRT) >> 1;
	dwRC = (dwPRT + dwPRB) >> 1;
	dwBC = (dwPLB + dwPRB) >> 1;

	switch (eType)
	{
	case CHILD_LT:
		dwLT = dwPLT;
		dwRT = dwTC;
		dwLB = dwLC;
		dwRB = dwPCenter;
		break;

	case CHILD_RT:
		dwLT = dwTC;
		dwRT = dwPRT;
		dwLB = dwPCenter;
		dwRB = dwRC;
		break;

	case CHILD_LB:
		dwLT = dwLC;
		dwRT = dwPCenter;
		dwLB = dwPLB;
		dwRB = dwBC;
		break;

	case CHILD_RB:
		dwLT = dwPCenter;
		dwRT = dwRC;
		dwLB = dwBC;
		dwRB = dwPRB;
		break;
	}

	*pLT = dwLT;
	*pRT = dwRT;
	*pLB = dwLB;
	*pRB = dwRB;

}

void Engine::CQuadTree::Make_ChildQuadTree()
{
	if (1 == m_dwCorner[CORNER_RT] - m_dwCorner[CORNER_LT])
		return;

	for (_ulong i = 0; i < CHILD_END; ++i)
	{
		_ulong	dwLT, dwRT, dwLB, dwRB;
		Make_ChildCorner(CHILD(i),
						m_dwCorner[CORNER_LT],
						m_dwCorner[CORNER_RT],
						m_dwCorner[CORNER_LB],
						m_dwCorner[CORNER_RB],
						m_dwCenter,
						&dwLT,
						&dwRT,
						&dwLB,
						&dwRB);
		m_pChild[i] = CQuadTree::Create(dwLT, dwRT, dwLB, dwRB);
		
		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_ChildQuadTree();
	}
}

void Engine::CQuadTree::Set_Corner(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	m_dwCorner[CORNER_LT] = dwLT;
	m_dwCorner[CORNER_RT] = dwRT;
	m_dwCorner[CORNER_LB] = dwLB;
	m_dwCorner[CORNER_RB] = dwRB;

	m_dwCenter = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) >> 2;
}

CQuadTree* Engine::CQuadTree::Create(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	CQuadTree*	pInstance = new CQuadTree;

	if (FAILED(pInstance->Ready_QuadTree(dwCntX, dwCntZ)))
		Safe_Release(pInstance);

	if (FAILED(pInstance->Ready_Neighbor()))
		Safe_Release(pInstance);

	return pInstance;
}

CQuadTree* Engine::CQuadTree::Create(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	CQuadTree*	pInstance = new CQuadTree;

	pInstance->Set_Corner(dwLT, dwRT, dwLB, dwRB);

	return pInstance;
}

void Engine::CQuadTree::Free(void)
{
	for (_ulong i = 0; i < CHILD_END; ++i)
		Safe_Release(m_pChild[i]);
}

_bool Engine::CQuadTree::LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos)
{
	_matrix		matView;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	_float	fDistance = D3DXVec3Length(&(vCamPos - pVtxPos[m_dwCenter]));

	_float	fWidth = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_RT]] - pVtxPos[m_dwCorner[CORNER_LT]]));

	return fDistance * 0.1f > fWidth;
}

HRESULT Engine::CQuadTree::Ready_Neighbor(void)
{
	Make_Neighbor();

	return S_OK;
}

void Engine::CQuadTree::Make_Neighbor()
{
	if (nullptr == m_pChild[CHILD_LT])
		return;

	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RT];
	m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_LB];

	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LT];
	m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pChild[CHILD_RB];

	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pChild[CHILD_RB];
	m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_LT];

	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pChild[CHILD_LB];
	m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_TOP] = m_pChild[CHILD_RT];


	if (nullptr != m_pNeighbor[NEIGHBOR_LEFT])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RT];
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_LEFT] = m_pNeighbor[NEIGHBOR_LEFT]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_TOP])
	{
		m_pChild[CHILD_LT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_LB];
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_TOP] = m_pNeighbor[NEIGHBOR_TOP]->m_pChild[CHILD_RB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_RIGHT])
	{
		m_pChild[CHILD_RT]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_RIGHT] = m_pNeighbor[NEIGHBOR_RIGHT]->m_pChild[CHILD_LB];
	}

	if (nullptr != m_pNeighbor[NEIGHBOR_BOTTOM])
	{
		m_pChild[CHILD_LB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_LT];
		m_pChild[CHILD_RB]->m_pNeighbor[NEIGHBOR_BOTTOM] = m_pNeighbor[NEIGHBOR_BOTTOM]->m_pChild[CHILD_RT];
	}

	for (_ulong i = 0; i < CHILD_END; ++i)
		m_pChild[i]->Make_Neighbor();
}

