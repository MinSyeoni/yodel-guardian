#include "Astar.h"
#include "Cell.h"
CAstar::CAstar(ID3D12Device* pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CAstar::CAstar(const CAstar& rhs)
{
}

CAstar::~CAstar()
{
}

HRESULT CAstar::Ready_Astar()
{
	return S_OK;
}

HRESULT CAstar::Init_AstarCell(vector<CCell*> pCellList)
{



	m_vecGraph.clear();
	m_vecGraph.resize(pCellList.size());
	for (_uint iIndex = 0; iIndex < pCellList.size(); ++iIndex)
	{
		for (int i = 0; i < CCell::NEIGHBOR_END; ++i)
		{
			if (pCellList[iIndex]->m_pNeighbor[i] != nullptr)
			{

				m_vecGraph[iIndex].push_back(pCellList[iIndex]->m_pNeighbor[i]);

			}

		}



	}


	m_vecCell = pCellList;
	return S_OK;
}

void CAstar::Start_Aster(const _vec3& vStartPos, const _vec3& vGoal, _ulong StartIndex, _ulong GoalIndex)
{
	m_OpenLst.clear();
	m_ClostLst.clear();
	m_BestLst.clear();

	m_iStartIndex = StartIndex;

	_ulong iGoalIndex = GoalIndex;



	if (true == SearthRoute(m_iStartIndex, iGoalIndex))
		MakeRoute(m_iStartIndex, iGoalIndex);



}

bool CAstar::SearthRoute(int iStartIdx, int iGoalIdx)
{

	const vector<CCell*>& vecCell = m_vecCell;

	if (!m_OpenLst.empty())
		m_OpenLst.pop_front();


	m_ClostLst.push_back(iStartIdx);


	for (auto& pCell : m_vecGraph[iStartIdx])
	{

		if (iGoalIdx == pCell->m_dwIndex)
		{
			pCell->m_dwParentIndex = iStartIdx;
			return true;
		}

		if (CheckOpen(pCell->m_dwIndex) || CheckClose(pCell->m_dwIndex))
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
		float fFromStart1 = v1.Get_Length();

		_vec3 v2 = vecCell[iGoalIdx]->m_vPos - vecCell[iFirst]->m_vPos;
		float fToGoal1 = v2.Get_Length();

		float fCost1 = fFromStart1 + fToGoal1;

		_vec3 v3 = vecCell[iTempStartIdx]->m_vPos - vecCell[iSecond]->m_vPos;
		float fFromStart2 = v3.Get_Length();

		_vec3 v4 = vecCell[iGoalIdx]->m_vPos - vecCell[iSecond]->m_vPos;
		float fToGoal2 = v4.Get_Length();

		float fCost2 = fFromStart2 + fToGoal2;
		return fCost1 < fCost2;

	});


	return SearthRoute(m_OpenLst.front(), iGoalIdx);

	return false;
}

void CAstar::MakeRoute(int iStartIdx, int iGoalIdx)
{
	m_BestLst.push_front(m_vecCell[iGoalIdx]);
	_ulong iParentIdx = m_vecCell[iGoalIdx]->m_dwParentIndex;

	while (true)
	{
		if (m_iStartIndex == iParentIdx)
			break;

		m_BestLst.push_front(m_vecCell[iParentIdx]);
		iParentIdx = m_vecCell[iParentIdx]->m_dwParentIndex;


	}


}

bool CAstar::CheckOpen(int iIndex)
{
	for (int iOpen : m_OpenLst)
	{
		if (iOpen == iIndex)
			return true;

	}
	return false;

}

bool CAstar::CheckClose(int iIndex)
{
	for (int iClose : m_ClostLst)
	{
		if (iClose == iIndex)
			return true;
	}

	return false;
}



CAstar* CAstar::Create(ID3D12Device* pGraphicDevice)
{
	CAstar* pInstance = new CAstar(pGraphicDevice);

	if (FAILED(pInstance->Ready_Astar()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent* CAstar::Clone()
{
	return new CAstar(*this);
}

void CAstar::Free()
{
}
