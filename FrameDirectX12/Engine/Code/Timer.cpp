#include "Timer.h"

USING(Engine)

CTimer::CTimer()
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}


CTimer::~CTimer()
{
}

void CTimer::SetUp_TimeDelta(void)
{
	/*____________________________________________________________________
	TimeDelta °»½Å.
	______________________________________________________________________*/

	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTimeDelta = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;
}

HRESULT CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

CTimer * CTimer::Create(void)
{
	CTimer*	pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTimer::Free(void)
{
}
