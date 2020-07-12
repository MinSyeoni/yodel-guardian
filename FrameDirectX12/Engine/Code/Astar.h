#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;
class ENGINE_DLL CAstar : public CComponent
{
private:
	explicit CAstar(ID3D12Device* pGraphicDevice);
	explicit CAstar(const CAstar& rhs);
	virtual ~CAstar();
private:
	HRESULT Ready_Astar();
public:
	HRESULT Init_AstarCell(vector<CCell*> pCellList);


	void Start_Aster(const _vec3& vStartPos, const _vec3& vGoal,_ulong StartIndex, _ulong GoalIndex);

private:
	bool SearthRoute(int iStartIdx, int iGoalIdx);
	void MakeRoute(int iStartIdx, int iGoalIdx);
	bool CheckOpen(int iIndex);
	bool CheckClose(int iIndex);
public:
	list<CCell*>& GetBestLst() { return m_BestLst; };
private:
	_ulong m_iStartIndex;


	list<int> m_OpenLst;
	list<int> m_ClostLst;
	list<CCell*>  m_BestLst;
	vector<list<CCell*>>m_vecGraph;

	vector<CCell*> m_vecCell;
public:
	static CAstar* Create(ID3D12Device* pGraphicDevice);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();



};
END