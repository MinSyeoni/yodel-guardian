#include "Cell.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(m_vPoint, sizeof(_vec3) * POINT_END);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	ZeroMemory(m_pLine, sizeof(CLine*) * LINE_END);

	m_pGraphicDev->AddRef();
}

Engine::CCell::~CCell(void)
{
	for (auto&pSrc : m_pLine)
		Safe_Release(pSrc);
}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex,
	const _vec3* pPointA,
	const _vec3* pPointB,
	const _vec3* pPointC)
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

#ifdef _DEBUG
	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);
#endif

	m_vPos = (*pPointA + *pPointB + *pPointC) / 3.f;

	return S_OK;
}

void CCell::Sliding_Vector(_vec3 * pEndPos, const _vec3 * vDir)
{
	_vec3   vTagetPos = *pEndPos - *vDir;

	_vec2 vOut = { vDir->x, vDir->z };

	m_pLine[m_iSlidIndex]->Sliding_Vector(&_vec2(vTagetPos.x, vTagetPos.z), &vOut);

	vTagetPos.x += vOut.x;
	vTagetPos.z += vOut.y;

	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(vTagetPos.x, vTagetPos.z)))
		{
			*pEndPos -= *vDir;
			return;
		}
	}
	*pEndPos = vTagetPos;







}

_bool Engine::CCell::Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell)
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

void Engine::CCell::Render_Cell(void)
{
	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix			matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f); // 라인의 굵기를 세팅하는 함수
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix matTemp;
	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	
	m_pD3DXLine->End();

}

CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex,
	const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC,_int Option = 0)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);
	pInstance->m_iOption = Option;


	return pInstance;
}

void Engine::CCell::Free(void)
{
#ifdef _DEBUG
	Safe_Release(m_pD3DXLine);
#endif	

    




	Safe_Release(m_pGraphicDev);
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, LINE& eLine, _ulong* pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i]||m_pNeighbor[i]->m_iOption==1 || m_pNeighbor[i]->m_iOption == 2)
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

Engine::CCell::COMPARE CCell::CompareMonster(const _vec3 * pEndPos, LINE& eLine, _ulong * pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i] )
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
			pPoint = m_vPoint[i] - m_vPoint[i+1];
			pPlayer = *pPos - m_vPoint[i];

		}
		D3DXVec3Cross(&vecCross, &pPlayer, &pPoint);
		if(vecCross.y>0)
		return false;
}

	*iIndex = m_dwIndex;
	return true;

}

