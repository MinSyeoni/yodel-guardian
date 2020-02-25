#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Timer.h"
// CTimer들을 여러개 모아서 보관한다.

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimerMgr final : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr() = default;

public:
	_float	Get_TimeDelta(const _tchar * pTimerTag);

public:
	HRESULT	Add_Timer(const _tchar * pTimerTag);
	_float	Compute_Timer(const _tchar * pTimerTag);

private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMERS;

private:
	CTimer* Find_Timer(const _tchar * pTimerTag);

protected:
	virtual void Free();
};

END

#endif // TimerMgr_h__