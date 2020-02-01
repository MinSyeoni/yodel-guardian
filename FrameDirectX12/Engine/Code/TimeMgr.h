#pragma once
#include "Timer.h"

BEGIN(Engine)

/*____________________________________________________________________
여러 개의 Timer 클래스들을 map 컨테이너로 관리할 TimerMgr.
______________________________________________________________________*/

class ENGINE_DLL CTimerMgr: public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr(void);
	virtual ~CTimerMgr(void);
	
public:
	_float	Get_TimeDelta(wstring wstrTimerTag);
	void	SetUp_TimeDelta(wstring wstrTimerTag);
public:
	HRESULT	Ready_Timer(wstring wstrTimerTag);
private:
	CTimer*	Find_Timer(wstring wstrTimerTag);

private:
	unordered_map<wstring, CTimer*>	m_mapTimer;

private:
	virtual void Free(void);
};


END