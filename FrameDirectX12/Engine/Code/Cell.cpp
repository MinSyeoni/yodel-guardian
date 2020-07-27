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

}

_float CCell::Get_Height(const _vec3* pPos)
{

	XMVECTOR vPlane;


	vPlane = XMPlaneFromPoints(m_vPoint[POINT_A].Get_XMVECTOR(), m_vPoint[POINT_B].Get_XMVECTOR(), m_vPoint[POINT_C].Get_XMVECTOR());

	_float fHeight = XMVectorGetY(vPlane);

	if (fHeight == 0.f)
		return pPos->y;

	float a = XMVectorGetX(vPlane);
	float b = XMVectorGetY(vPlane);
	float c = XMVectorGetZ(vPlane);
	float d = XMVectorGetW(vPlane);



	return -(( a * pPos->x + c * pPos->z + d)/b);
}

HRESULT CCell::Ready_Cell(const _ulong & dwIndex, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z),
		&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
		&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
		&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	
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
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
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

CCell::COMPARE CCell::Compare(const _vec3* pEndPos, _ulong * pIndex, const _float & fTargetSpeed, _vec3 * pTargetPos, _vec3 * pTargetDir, _vec3 * pSlidingDir)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
			{
				_vec3 vNormal = _vec3(m_pLine[i]->Get_Normal().x, 0.f, m_pLine[i]->Get_Normal().y);

				*pSlidingDir = *pTargetDir - vNormal * vNormal.Dot(*pTargetDir);
				_vec3 vNormal14 = vNormal * 1.4f;
				_vec3 vNormal20 = vNormal * 2.f;
				_vec3 doubleNormal = *pTargetDir - (vNormal14)*vNormal14.Dot(*pTargetDir);
				_vec3 TripleNormal = *pTargetDir - (vNormal20)*vNormal20.Dot(*pTargetDir);

				_vec3 vEndPos = *pTargetPos + *pSlidingDir * fTargetSpeed * 5.f;
				_vec3 vDoublePos = *pTargetPos + doubleNormal * fTargetSpeed * 5.f;
				_vec3 vTriplePos = *pTargetPos + TripleNormal * fTargetSpeed * 5.f;

				_float fMaxX = max((m_pLine[i]->Get_Point(CLine::POINT_START).x), (m_pLine[i]->Get_Point(CLine::POINT_FINISH).x));
				_float fMinX = min(m_pLine[i]->Get_Point(CLine::POINT_START).x, m_pLine[i]->Get_Point(CLine::POINT_FINISH).x);

				_float fMaxZ = max(m_pLine[i]->Get_Point(CLine::POINT_START).y, m_pLine[i]->Get_Point(CLine::POINT_FINISH).y);
				_float fMinZ = min(m_pLine[i]->Get_Point(CLine::POINT_START).y, m_pLine[i]->Get_Point(CLine::POINT_FINISH).y);

				if ((vEndPos.x > fMinX) && (vEndPos.x < fMaxX) &&
					(vEndPos.z > fMinZ) && (vEndPos.z < fMaxZ))
				{
	
					return COMPARE_SLIDING;
				}
				else
				{
					*pTargetDir = doubleNormal;
					*pSlidingDir = TripleNormal;
					return COMPARE_STOP;
				}

			}

			else
			{
				*pIndex = *m_pNeighbor[i]->Get_Index();

				return COMPARE_MOVE;
			}
		}


	}




	return COMPARE_MOVE;
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


CCell * CCell::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const _ulong & dwIndex, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, _uint Option)
{
	CCell*	pInstance = new CCell(pGraphicDevice,pCommandList);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);
	pInstance->m_iOption = Option;


	return pInstance;
}

void CCell::Free(void)
{
	for (auto&pSrc : m_pLine)
		Safe_Release(pSrc);
}
