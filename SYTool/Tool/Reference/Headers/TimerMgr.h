#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr(void);
	virtual ~CTimerMgr(void);
public:
	_float		Get_TimeDelta(const _tchar* pTimerTag)const;
	void		Set_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);

private:
	CTimer*		Find_Timer(const _tchar* pTimerTag) const;
	
private:
	map<const _tchar*, CTimer*>			m_mapTimers;

public:
	virtual void Free(void);
};

END
#endif // TimerMgr_h__
