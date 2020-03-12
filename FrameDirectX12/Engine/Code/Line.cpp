#include "Line.h"
#include "GraphicDevice.h"
USING(Engine)
CLine::CLine()
{
	ZeroMemory(m_vPoint, sizeof(_vec2)*POINT_END);
}





HRESULT CLine::Ready_Line(const _vec2 * pPointA, const _vec2 * pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	m_vNormal.Normalize();

	return S_OK;
}

CLine::COMPARE CLine::Compare(const _vec2* pEndPos)
{
	_vec2 vEndPos = *pEndPos;
	_vec2 vDest = vEndPos - m_vPoint[POINT_START];
	vDest.Normalize();
	_float fResult = m_vNormal.Dot(vDest);

	if (0.f <= fResult)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

_vec2 CLine::Point_Meet(_vec2 * OutPut, _vec2 * pDir)
{
	_float x1 = OutPut->x;
	_float y1 = OutPut->y;
	_float m1;

	if (pDir->x == 0.f)
		m1 = 0.f;
	else
		m1 = pDir->y / pDir->x;

	_float b1 = -(m1 * x1) + y1;

	_float x2 = m_vPoint[POINT_FINISH].x;
	_float y2 = m_vPoint[POINT_FINISH].y;

	_float m2;

	if (m_vDirection.x == 0.f)
		m2 = 0.f;
	else
		m2 = m_vDirection.y / m_vDirection.x;


	_float b2 = -(m2 * x2) + y2;

	return _vec2{ (b2 - b1) / (m1 - m2)  ,m1 * ((b2 - b1) / (m1 - m2)) + b1 };
}

CLine * CLine::Create(const _vec2 * pPointA, const _vec2 * pPointB)
{
	CLine*	pInstance = new CLine();

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLine::Free(void)
{

}
