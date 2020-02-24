#include "TimerMgr.h"	
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{
}

_float CTimerMgr::Get_TimeDelta(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

HRESULT CTimerMgr::Add_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMERS::value_type(pTimerTag, pTimer));

	return NOERROR;
}

_float CTimerMgr::Compute_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Compute_TimeDelta();
}

CTimer* CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CFinder_Tag(pTimerTag));

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimerMgr::Free()
{
	for (auto& Pair : m_mapTimers)
	{
		Safe_Release(Pair.second);
	}

	m_mapTimers.clear();
}

