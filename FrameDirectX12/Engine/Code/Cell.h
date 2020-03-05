#pragma once

#include "Component.h"
#include "Line.h"
#include "Shader_ColorBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CCell : public CComponent
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE { COMPARE_MOVE, COMPARE_STOP };

private:
	explicit CCell(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CCell();
public:
public:
	const _vec3*		Get_Point(POINT eType) const { return &m_vPoint[eType]; }
	CCell*				Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	const _ulong*		Get_Index(void) { return &m_dwIndex; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	CLine* Get_Line(LINE eId)const { return m_pLine[eId]; }
public:
	HRESULT				Ready_Cell(const _ulong& dwIndex,
		const _vec3* pPointA,
		const _vec3* pPointB,
		const _vec3* pPointC);

	COMPARE				Compare(const _vec3* pEndPos, LINE& eLine, _ulong* pCellIndex);
	_bool				Compare_Point(const _vec3* pPointA,
		const _vec3* pPointB,
		CCell* pCell);


	_bool               FindCell(_vec3* pPos, _ulong* iIndex);
	void				Render_Cell(CShader_ColorBuffer* pShader);

public:
	_vec3		m_vPoint[POINT_END];
	CCell*		m_pNeighbor[NEIGHBOR_END];
	CLine*		m_pLine[LINE_END];
	_vec3       m_vPos;
	_ulong		m_dwIndex;
	_ulong      m_dwParentIndex = 0;
	_uint       m_iSlidIndex;
	_uint       m_iOption = 0;
public:
	static CCell*		Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,
		const _ulong& dwIndex,
		const _vec3* pPointA,
		const _vec3* pPointB,
		const _vec3* pPointC,
		_int Option);
private:
	virtual void	Free(void);
};
END