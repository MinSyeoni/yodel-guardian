#include "TimeMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr(void)
{
}

CTimerMgr::~CTimerMgr(void)
{
}

_float CTimerMgr::Get_TimeDelta(wstring wstrTimerTag)
{
	/*____________________________________________________________________
	해당 Tag값에 해당하는 Timer의 TimeDelta값을 얻어온다.
	______________________________________________________________________*/
	CTimer* pInstance = Find_Timer(wstrTimerTag);

	if (nullptr == pInstance)
		return 0.f;


	return pInstance->Get_TimeDelta();
}

void CTimerMgr::SetUp_TimeDelta(wstring wstrTimerTag)
{
	/*____________________________________________________________________
	해당 Tag값에 해당하는 Timer 갱신.
	______________________________________________________________________*/
	CTimer* pInstance = Find_Timer(wstrTimerTag);
	NULL_CHECK(pInstance);

	pInstance->SetUp_TimeDelta();
}

HRESULT CTimerMgr::Ready_Timer(wstring wstrTimerTag)
{
	/*____________________________________________________________________
	Timer를 생성하기 전, 해당 Tag값이 map에 들어있는지 탐색한다.
	이미 존재하는 값이면, 새로 생성할 필요 없으므로 종료한다.
	______________________________________________________________________*/
	CTimer* pInstance = Find_Timer(wstrTimerTag);

	if (nullptr != pInstance)
		return E_FAIL;

	/*____________________________________________________________________
	없는 Tag값이면 새로 생성한 후, map컨테이너에 넣어준다.
	______________________________________________________________________*/
	pInstance = CTimer::Create();
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_mapTimer.emplace(wstrTimerTag, pInstance);


	return S_OK;
}

CTimer * CTimerMgr::Find_Timer(wstring wstrTimerTag)
{
	/*____________________________________________________________________
	해당 Timer Tag값을 탐색한다.
	찾는 Tag값이 없다면 nullptr을 반환한다.
	______________________________________________________________________*/
	auto iter_find = m_mapTimer.find(wstrTimerTag);

	if (iter_find == m_mapTimer.end())
		return nullptr;


	return iter_find->second;
}

void CTimerMgr::Free(void)
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());

	m_mapTimer.clear();
}
