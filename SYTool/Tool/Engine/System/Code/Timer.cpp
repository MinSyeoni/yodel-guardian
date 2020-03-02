#include "Timer.h"

CTimer::CTimer()
{

}

HRESULT CTimer::Ready_Timer()
{
	// QueryPerformanceCounter : 현재 cpu가 카운팅한 숫자를 얻어온다.
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	// QueryPerformanceFrequency : cpu가 1초에 카운팅할 수 있는 최대 숫자를 얻어온다.
	QueryPerformanceFrequency(&m_CpuTick);

	return NOERROR;
}

// 매 프레임마다 한번씩 무한으로 호출된다.
_float CTimer::Compute_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / _float(m_CpuTick.QuadPart);

	m_FixTime.QuadPart = m_FrameTime.QuadPart;

	// 1초에 한번씩 
	if ((m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime = m_FrameTime;
	}

	return _float(m_fTimeDelta);
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer();

	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("CTimer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTimer::Free()
{

}