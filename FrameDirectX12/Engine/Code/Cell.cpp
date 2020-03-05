#include "Cell.h"

USING(Engine)

CCell::CCell(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice,pCommandList)
{
	ZeroMemory(m_vPoint, sizeof(_vec3) * POINT_END);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	ZeroMemory(m_pLine, sizeof(CLine*) * LINE_END);
}

CCell::~CCell()
{
	for (auto&pSrc : m_pLine)
		Safe_Release(pSrc);
}

HRESULT CCell::Ready_Cell(const _ulong & dwIndex, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&m_vPoint[POINT_A],
		&m_vPoint[POINT_B],m_pGraphicDevice,m_pCommandList);

	m_pLine[LINE_BC] = CLine::Create(&m_vPoint[POINT_B],
		&m_vPoint[POINT_C],m_pGraphicDevice, m_pCommandList);

	m_pLine[LINE_CA] = CLine::Create(&m_vPoint[POINT_C],
		&m_vPoint[POINT_A], m_pGraphicDevice, m_pCommandList);


	m_vPos = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]);
	m_vPos.x = m_vPos.x / 3.f;
	m_vPos.y = m_vPos.y / 3.f;
	m_vPos.z = m_vPos.z / 3.f;

	return S_OK;
}

CCell::COMPARE CCell::Compare(const _vec3 * pEndPos, LINE & eLine, _ulong * pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(pEndPos))
		{
			if (nullptr == m_pNeighbor[i] || m_pNeighbor[i]->m_iOption == 1 || m_pNeighbor[i]->m_iOption == 2)
			{
				eLine = CCell::LINE(i);
				return CCell::COMPARE_STOP;
			}
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::COMPARE_MOVE;
			}
		}
	}

	return CCell::COMPARE_MOVE;
}

_bool CCell::Compare_Point(const _vec3 * pPointA, const _vec3 * pPointB, CCell * pCell)
{
	if (m_vPoint[POINT_A] == *pPointA)
	{
		if (m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;

			return true;
		}
		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointA)
	{
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointA)
	{
		if (m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
		if (m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	return false;
}



_bool CCell::FindCell(_vec3 * pPos, _ulong * iIndex)
{
	for (int i = 0; i < POINT_END; i++)
	{
		_vec3 pPoint;
		_vec3 pPlayer;
		_vec3 vecCross;
		if (i == 2)
		{
			pPoint = m_vPoint[2] - m_vPoint[0];
			pPlayer = *pPos - m_vPoint[2];
		}
		else
		{
			pPoint = m_vPoint[i] - m_vPoint[i + 1];
			pPlayer = *pPos - m_vPoint[i];

		}
		 vecCross = pPoint.Cross_InputDst(pPlayer);
		if (vecCross.y > 0)
			return false;
	}

	*iIndex = m_dwIndex;
	return true;
}

void CCell::Render_Cell(CShader_ColorBuffer* pShader)
{
	for (int i = 0; i < LINE_END; i++)
	{
		m_pLine[i]->RenderLine(pShader);
	}
	
}

CCell * CCell::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const _ulong & dwIndex, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, _int Option)
{
	CCell*	pInstance = new CCell(pGraphicDevice,pCommandList);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);
	pInstance->m_iOption = Option;


	return pInstance;
}

void CCell::Free(void)
{
}
