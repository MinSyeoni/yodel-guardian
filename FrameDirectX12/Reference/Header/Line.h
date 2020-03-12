#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;

class ENGINE_DLL CLine : public CBase
{
public:
	enum POINT { POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CLine(void);
	virtual ~CLine(void)=default;

public:
	HRESULT	Ready_Line(const _vec2* pPointA, const _vec2* pPointB);
	COMPARE	Compare(const _vec2* pEndPos);


	_vec2 Point_Meet(_vec2 * OutPut, _vec2 * pDir);
	_vec2 Get_LineDir() { return m_vPoint[POINT_FINISH] - m_vPoint[POINT_START]; }
	const _vec2& Get_Normal() { return m_vNormal; }
	const _vec2& Get_Point(POINT ePoint) { return m_vPoint[ePoint]; }

private:
	_vec2			m_vPoint[POINT_END];
	_vec2			m_vDirection;
	_vec2			m_vNormal;

public:
	static CLine*		Create(const _vec2* pPointA, const _vec2* pPointB);
	virtual void		Free(void);
};

END