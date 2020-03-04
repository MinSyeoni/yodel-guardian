#include "NaviMesh.h"

CNaviMesh::CNaviMesh(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice,pCommandList)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh & rhs)
	:m_vecCell(rhs.m_vecCell)
	,m_dwIndex(rhs.m_dwIndex)
	,m_pShaderCom(rhs.m_pShaderCom)
{
	for (auto iter : m_vecCell)
		iter->AddRef();
	m_pShaderCom->AddRef();
}

CNaviMesh::~CNaviMesh()
{
}

HRESULT CNaviMesh::Ready_NaviMesh()
{
	for (auto iter : m_vecCell)
		Safe_Release(iter);

	m_vecCell.clear();
	m_vecCell.shrink_to_fit();

	_vec3 vecPointA, vecPointB, vecPointC;
	_int iOption=0;

	vecPointA = _vec3(0.f, 0.f, 0.f);
	vecPointB = _vec3(0.f, 0.f, 1.f);
	vecPointC = _vec3(1.f, 0.f, 1.f);

	CCell*pCell = nullptr;

	pCell = CCell::Create(m_pGraphicDevice, m_pCommandList, _ulong(m_vecCell.size()), &vecPointA, &vecPointB, &vecPointC, 0);
	m_vecCell.push_back(pCell);


	m_pShaderCom = CShader_ColorBuffer::Create(m_pGraphicDevice, m_pCommandList);
	return S_OK;
}

HRESULT CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
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

void CNaviMesh::Render_NaviMesh(void)
{
	m_pShaderCom->Begin_Shader();
	for (int i = 0; i < m_vecCell.size(); i++)
		m_vecCell[i]->Render_Cell(m_pShaderCom);

}

_vec3 CNaviMesh::Move_NaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{
	return _vec3();
}

CNaviMesh * CNaviMesh::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CNaviMesh* pInstance = new CNaviMesh(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent * CNaviMesh::Clone()
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Free()
{
}
