#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer(void)
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

Engine::CTimer::~CTimer(void)
{

}

HRESULT Engine::CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);	
	QueryPerformanceCounter(&m_FixTime);	
	QueryPerformanceCounter(&m_LastTime);	

	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

Engine::CTimer* Engine::CTimer::Create(void)
{
	CTimer*		pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("Timer Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTimer::Free(void)
{

}

void Engine::CTimer::Set_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);	

	if (m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;
}

_float Engine::CTimer::Get_TimeDelta(void) const
{
	 return m_fTimeDelta;
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
