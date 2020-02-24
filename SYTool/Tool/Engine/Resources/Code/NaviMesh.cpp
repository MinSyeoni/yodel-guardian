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
	,m_vecGraph(rhs.m_vecGraph)
{
	for (auto iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

void CNaviMesh::Set_DefaultIndex(_vec3 pPos)
{
	m_dwIndex = 0;
	int MinDist = 100;
	for (auto& pCell : m_vecCell)
	{
		float Dist = D3DXVec3Length(&_vec3(pPos - pCell->m_vPos));
		if (Dist < MinDist)
		{
			m_dwIndex = pCell->m_dwIndex;
			MinDist = Dist;
		}

	
	}


	return;
	
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	for (auto iter : m_vecCell)
		Safe_Release(iter);
	m_vecCell.clear();
	m_vecCell.shrink_to_fit();
	HANDLE hFile = CreateFile(L"../../Data/Navi.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;
	DWORD dwByte = 0;
	while (true)
	{
		_vec3 vecPointA, vecPointB, vecPointC;
		_int m_iOption;

		ReadFile(hFile, &vecPointA, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vecPointB, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &vecPointC, sizeof(D3DXVECTOR3), &dwByte, nullptr);
		ReadFile(hFile, &m_iOption, sizeof(int), &dwByte, nullptr);
		vecPointA.y = 0;
		vecPointB.y = 0;
		vecPointC.y = 0;



		if (dwByte == 0)
			break;

		CCell*pCell = nullptr;

		pCell = CCell::Create(m_pGraphicDev,
			m_vecCell.size(),
			&vecPointA,
			&vecPointB,
			&vecPointC,m_iOption);
		m_vecCell.push_back(pCell);


	}



	CloseHandle(hFile);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	MakeLoot();

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



void Engine::CNaviMesh::Render_NaviMesh(void)
{
	for (auto iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 Engine::CNaviMesh::Move_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir)
{
	CCell::LINE eLineID = CCell::LINE_END;
	_vec3	vEndPos = *pTargetPos + *pTargetDir;
	_ulong						PredwIndex = m_dwIndex;
	_ulong						CurdwIndex = m_dwIndex;
	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, eLineID, &m_dwIndex))
		return vEndPos;

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, eLineID, &m_dwIndex))
	{


		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		D3DXVec3Normalize(&vUp, &vUp);

		_vec2 vLine = m_vecCell[m_dwIndex]->Get_Line(eLineID)->Get_LineDir();

		_vec3 vNormal = _vec3(vLine.x, 0.f, vLine.y);
		D3DXVec3Cross(&vNormal, &vUp, &vNormal);
		D3DXVec3Normalize(&vNormal, &vNormal);

		_vec3 vSlide = *pTargetDir - (vNormal * D3DXVec3Dot(pTargetDir, &vNormal));

		_vec3 vEnd = *pTargetPos + vSlide;

		if (false == Check_Point(m_vecCell[m_dwIndex], &vEnd))
		{
			_ulong dwIndex = m_dwIndex;

			if (false == Find_Cell(&vEnd, &m_dwIndex))
			{
				return *pTargetPos + (*pTargetDir * -1.f) * 0.01f;
			}

			if (dwIndex != m_dwIndex)
			{
				if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEnd,
					eLineID,
					&m_dwIndex))
				{
					return vEnd;
				}
				else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEnd,
					eLineID,
					&m_dwIndex))
				{

					_vec3 vUp = _vec3(0.f, 1.f, 0.f);
					D3DXVec3Normalize(&vUp, &vUp);

					_vec2 vLine = m_vecCell[m_dwIndex]->Get_Line(eLineID)->Get_LineDir();

					_vec3 vNormal = _vec3(vLine.x, 0.f, vLine.y);
					D3DXVec3Cross(&vNormal, &vUp, &vNormal);
					D3DXVec3Normalize(&vNormal, &vNormal);

					_vec3 vSlide = *pTargetDir - (vNormal * D3DXVec3Dot(pTargetDir, &vNormal));

					return *pTargetPos + vSlide;
				}
			}

		}
		return *pTargetPos + vSlide;

	}
}
_vec3 CNaviMesh::Move_NaviMeshMonster(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _int* Option)
{
	CCell::LINE eLineID = CCell::LINE_END;
	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->CompareMonster(&vEndPos, eLineID,&m_dwIndex))
	{
		*Option = m_vecCell[m_dwIndex]->m_iOption;
		return vEndPos;

	}
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->CompareMonster(&vEndPos,eLineID, &m_dwIndex))
	{

		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		D3DXVec3Normalize(&vUp, &vUp);

		_vec2 vLine = m_vecCell[m_dwIndex]->Get_Line(eLineID)->Get_LineDir();

		_vec3 vNormal = _vec3(vLine.x, 0.f, vLine.y);
		D3DXVec3Cross(&vNormal, &vUp, &vNormal);
		D3DXVec3Normalize(&vNormal, &vNormal);

		_vec3 vSlide = *pTargetDir - (vNormal * D3DXVec3Dot(pTargetDir, &vNormal));

		_vec3 vEnd = *pTargetPos + vSlide;

		if (false == Check_Point(m_vecCell[m_dwIndex], &vEnd))
		{
			_ulong dwIndex = m_dwIndex;

			if (false == Find_Cell(&vEnd, &m_dwIndex))
			{
				return *pTargetPos + (*pTargetDir * -1.f) * 0.01f;
			}

			if (dwIndex != m_dwIndex)
			{
				if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEnd,
					eLineID,
					&m_dwIndex))
				{
					return vEnd;
				}
				else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEnd,
					eLineID,
					&m_dwIndex))
				{

					_vec3 vUp = _vec3(0.f, 1.f, 0.f);
					D3DXVec3Normalize(&vUp, &vUp);

					_vec2 vLine = m_vecCell[m_dwIndex]->Get_Line(eLineID)->Get_LineDir();

					_vec3 vNormal = _vec3(vLine.x, 0.f, vLine.y);
					D3DXVec3Cross(&vNormal, &vUp, &vNormal);
					D3DXVec3Normalize(&vNormal, &vNormal);

					_vec3 vSlide = *pTargetDir - (vNormal * D3DXVec3Dot(pTargetDir, &vNormal));

					return *pTargetPos + vSlide;
				}
			}

		}
		return *pTargetPos + vSlide;
	}
	return *pTargetPos;
}

HRESULT CNaviMesh::Change_NaviMesh()
{

	return S_OK;
}

void CNaviMesh::MakeLoot(void)
{
	m_vecGraph.clear();
	m_vecGraph.resize(m_vecCell.size());

	for (int iIndex = 0; iIndex < m_vecCell.size(); iIndex++)
	{


		for (int i = 0; i < CCell::NEIGHBOR_END; i++)
		{
			if (m_vecCell[iIndex]->m_pNeighbor[i] != nullptr)
			{
				m_vecGraph[iIndex].push_back(m_vecCell[iIndex]->m_pNeighbor[i]);
			}

		}

	}



}


void CNaviMesh::StartAster(const _vec3 & vStartPos, const _vec3 & vGoalPos,_ulong goalIndex)
{
	m_OpenLst.clear();
	m_ClostLst.clear();
	m_BestLst.clear();

	m_iStartIndex = m_dwIndex;
	_ulong iGoalIndex = goalIndex;



	if (true == SearthRoute(m_iStartIndex, iGoalIndex))
		MakeRoute(m_iStartIndex, iGoalIndex);



}

bool CNaviMesh::SearthRoute(int iStartIdx, int iGoalIdx)
{
	const vector<CCell*>&vecCell = m_vecCell;


	if (!m_OpenLst.empty())
		m_OpenLst.pop_front();

	m_ClostLst.push_back(iStartIdx);

	for (auto&pCell : m_vecGraph[iStartIdx])
	{

		if (iGoalIdx == pCell->m_dwIndex)
		{
			pCell->m_dwParentIndex = iStartIdx;
			return true;
		}

		if(CheckOpen(pCell->m_dwIndex)||CheckClose(pCell->m_dwIndex))
			continue;
		pCell->m_dwParentIndex = iStartIdx;
		m_OpenLst.push_back(pCell->m_dwIndex);

	}

	if (m_OpenLst.empty())
		return false;


	int iTempStartIdx = m_iStartIndex;

	m_OpenLst.sort([&iTempStartIdx, &iGoalIdx, &vecCell](int iFirst, int iSecond)
	{
		_vec3 v1 = vecCell[iTempStartIdx]->m_vPos - vecCell[iFirst]->m_vPos;
		float fFromStart1 = D3DXVec3Length(&v1);

		_vec3 v2 = vecCell[iGoalIdx]->m_vPos - vecCell[iFirst]->m_vPos;
		float fToGoal1 = D3DXVec3Length(&v2);

		float fCost1 = fFromStart1 + fToGoal1;

		_vec3 v3 = vecCell[iTempStartIdx]->m_vPos - vecCell[iSecond]->m_vPos;
		float fFromStart2 = D3DXVec3Length(&v3);

		_vec3 v4 = vecCell[iGoalIdx]->m_vPos - vecCell[iSecond]->m_vPos;
		float fToGoal2 = D3DXVec3Length(&v4);

		float fCost2 = fFromStart2 + fToGoal2;
		return fCost1 < fCost2;

	});


	return SearthRoute(m_OpenLst.front(),iGoalIdx);
}

void CNaviMesh::MakeRoute(int iStartIdx, int iGoalIdx)
{
	m_BestLst.push_front(m_vecCell[iGoalIdx]);
    _ulong iParentIdx = m_vecCell[iGoalIdx]->m_dwParentIndex;

	while (true)
	{
		if(m_iStartIndex == iParentIdx)
			break;

		m_BestLst.push_front(m_vecCell[iParentIdx]);
		iParentIdx = m_vecCell[iParentIdx]->m_dwParentIndex;


	}


}

bool CNaviMesh::CheckOpen(int iIndex)
{
	for (int iOpen : m_OpenLst)
	{
		if (iOpen == iIndex)
			return true;

	}
	return false;


}

bool CNaviMesh::CheckClose(int iIndex)
{
	for (int iClose : m_ClostLst)
	{
		if (iClose == iIndex)
			return true;
	}

	return false;
}

_bool CNaviMesh::Check_Point(const CCell* pCell,const _vec3* pPos)
{
	_vec3 vPoint[CCell::POINT_END];

	for (_uint i = 0; i < CCell::POINT_END; ++i)
	{
		vPoint[i] = *pCell->Get_Point(CCell::POINT(i));
	}

	_vec3 vLineDir[CCell::LINE_END];

	vLineDir[CCell::LINE_AB] = _vec3
	(
		pCell->Get_Line(CCell::LINE_AB)->Get_LineDir().x,
		0.f,
		pCell->Get_Line(CCell::LINE_AB)->Get_LineDir().y
	);

	vLineDir[CCell::LINE_BC] = _vec3
	(
		pCell->Get_Line(CCell::LINE_BC)->Get_LineDir().x,
		0.f,
		pCell->Get_Line(CCell::LINE_BC)->Get_LineDir().y
	);

	vLineDir[CCell::LINE_CA] = _vec3
	(
		pCell->Get_Line(CCell::LINE_CA)->Get_LineDir().x,
		0.f,
		pCell->Get_Line(CCell::LINE_CA)->Get_LineDir().y
	);

	for (_uint i = 0; i < CCell::LINE_END; ++i)
	{
		D3DXVec3Normalize(&vLineDir[i], &vLineDir[i]);
	}

	_vec3 vPointDir[CCell::POINT_END];

	vPointDir[CCell::POINT_A] = *pPos - vPoint[CCell::POINT_A];
	vPointDir[CCell::POINT_B] = *pPos - vPoint[CCell::POINT_B];
	vPointDir[CCell::POINT_C] = *pPos - vPoint[CCell::POINT_C];

	for (_uint i = 0; i < CCell::LINE_END; ++i)
	{
		D3DXVec3Normalize(&vPointDir[i], &vPointDir[i]);
	}

	_vec3 vCross;

	D3DXVec3Cross(&vCross, &vPointDir[CCell::POINT_A], &vLineDir[CCell::LINE_AB]);

	if (0.f < vCross.y)
	{
		return false;
	}

	D3DXVec3Cross(&vCross, &vPointDir[CCell::POINT_B], &vLineDir[CCell::LINE_BC]);

	if (0.f < vCross.y)
	{
		return false;
	}

	D3DXVec3Cross(&vCross, &vPointDir[CCell::POINT_C], &vLineDir[CCell::LINE_CA]);

	if (0.f < vCross.y)
	{
		return false;
	}

	return true;
}
_bool CNaviMesh::Find_Cell(_vec3* pPos,_ulong* dwIndex)
{
	_vec3 vNearCell;
	_ulong _dwIndex = 0;
	_float fLength = 100.f;

	for (_uint i = 0; i < m_vecCell.size(); ++i)
	{
		if (Check_Point(m_vecCell[i], pPos))
		{
			*dwIndex = i;
			return true;
		}
		else
		{
			for (_uint j = 0; j < CCell::POINT_END; ++j)
			{
				_vec3 vDiff = *pPos - *m_vecCell[i]->Get_Point(CCell::POINT(j));

				if (fLength >= D3DXVec3Length(&vDiff))
				{
					fLength = D3DXVec3Length(&vDiff);
					vNearCell = *m_vecCell[i]->Get_Point(CCell::POINT(j));

					_dwIndex = i;
				}
			}
		}
	}

	if (0.1f > fLength)
	{
		*dwIndex = _dwIndex;
		*pPos = vNearCell;

		return true;
	}

	return false;
}