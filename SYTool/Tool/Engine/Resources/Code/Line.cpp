#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{
	ZeroMemory(m_vPoint, sizeof(_vec2) * POINT_END);
}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)
{
	_vec2 vDest = *pEndPos - m_vPoint[POINT_START];

	_float fResult = D3DXVec2Dot(D3DXVec2Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fResult)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

void CLine::Sliding_Vector(_vec2 * OutPut, _vec2 * pDir)
{
	_vec2 vMeetPoint = Point_Meet(OutPut, pDir);

	// ¹æÇâ º¤ÅÍ
	_vec2 vEndDir = m_vPoint[POINT_FINISH] - vMeetPoint;
	_vec2 vStartDir = m_vPoint[POINT_START] - vMeetPoint;

	// ½½¶óÀÌµù º¤ÅÍ 
	_vec2   vSlide, Temp1, Temp2;

	vSlide = *pDir - m_vNormal * (D3DXVec2Dot(pDir, &m_vNormal));

	_float fResult = D3DXVec2Dot(D3DXVec2Normalize(&Temp1, &vSlide), D3DXVec2Normalize(&Temp2, &vEndDir));


	if (0.f <= fResult)
	{
		if (D3DXVec2Length(&(vSlide)) < D3DXVec2Length(&vEndDir))
		{
			*pDir = vSlide;
			return;
		}
	}
	else
	{
		if (D3DXVec2Length(&(vSlide)) < D3DXVec2Length(&vStartDir))
		{
			*pDir = vSlide;
			return;
		}
	}

	*pDir = { 0.f, 0.f };
	return;

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

	return{ (b2 - b1) / (m1 - m2)  ,m1 * ((b2 - b1) / (m1 - m2)) + b1 };




}

CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*	pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

