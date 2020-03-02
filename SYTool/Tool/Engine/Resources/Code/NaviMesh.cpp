#include "NaviMesh.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_dwIndex(rhs.m_dwIndex)
{
	for (auto iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	m_vecCell.reserve(4);

	CCell*		pCell = NULL;

	pCell = CCell::Create(m_pGraphicDev,
						m_vecCell.size(),
						&_vec3(0.f, 0.f, 2.f),
						&_vec3(2.f, 0.f, 0.f),
						&_vec3(0.f, 0.f, 0.f));

	m_vecCell.push_back(pCell);

	pCell = CCell::Create(m_pGraphicDev,
						m_vecCell.size(),
						&_vec3(0.f, 0.f, 2.f),
						&_vec3(2.f, 0.f, 2.f),
						&_vec3(2.f, 0.f, 0.f));

	m_vecCell.push_back(pCell);

	pCell = CCell::Create(m_pGraphicDev,
						m_vecCell.size(),
						&_vec3(0.f, 0.f, 4.f),
						&_vec3(2.f, 0.f, 2.f),
						&_vec3(0.f, 0.f, 2.f));

	m_vecCell.push_back(pCell);

	pCell = CCell::Create(m_pGraphicDev,
						m_vecCell.size(),
						&_vec3(2.f, 0.f, 2.f),
						&_vec3(4.f, 0.f, 0.f),
						&_vec3(2.f, 0.f, 0.f));

	m_vecCell.push_back(pCell);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);


	return S_OK;
}

CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	CMesh::Free();

	for (auto iter : m_vecCell)
		Safe_Release(iter);

	m_vecCell.clear();

}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]->Get_Point(CCell::POINT_B), 
													m_vecCell[i]))
			
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}				
	}

	return S_OK;
}

void Engine::CNaviMesh::Render_NaviMesh(void)
{
	for (auto iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 Engine::CNaviMesh::Move_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return vEndPos;

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return *pTargetPos;
}

