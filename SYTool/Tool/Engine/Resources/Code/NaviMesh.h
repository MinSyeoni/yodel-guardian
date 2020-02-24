#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void		Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }
	void        Set_DefaultIndex(_vec3 pPos);
public:
	HRESULT		Ready_NaviMesh(void);
	HRESULT		Link_Cell(void);
	void		Render_NaviMesh(void);
	_vec3		Move_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_vec3       Move_NaviMeshMonster(const _vec3 *pTargetPos, const _vec3* pTargetDir,_int* Option);

	HRESULT    Change_NaviMesh(void);
	void       MakeLoot(void);
	_ulong  GetIndex(void) { return m_dwIndex; };

	CCell* GetCurCell(void) { return m_vecCell[m_dwIndex]; };


private:

	vector<CCell*>				m_vecCell;
	_ulong						m_dwIndex;
	
//Astar ฐüทร

public:
	void StartAster(const _vec3&vStartPos, const _vec3& vGoalPos,_ulong GoalIndex);
private:
	bool SearthRoute(int iStartIdx, int iGoalIdx);
	void MakeRoute(int iStartIdx, int iGoalIdx);
	bool CheckOpen(int iIndex);
	bool CheckClose(int iIndex);
	_bool Check_Point(const CCell* pCell, const _vec3* pPos);
	_bool Find_Cell(_vec3* pPos, _ulong* dwIndex);
public:
	 list<CCell*>& GetBestLst() { return m_BestLst; };
public:
	_ulong m_iStartIndex;




public:
	list<int> m_OpenLst;
	list<int> m_ClostLst;
	list<CCell*>  m_BestLst;
	vector<list<CCell*>>m_vecGraph;



public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*	Clone(void);
	virtual void		Free(void);

};

END
#endif // NaviMesh_h__
