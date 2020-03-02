#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#pragma once

#include "Base.h"

// 시간관리를 위한 클래스다.
// m_fTimeDelta을 보유하고 있다.
// m_fTimeDelta : 한 프레임이 지나갈때까지 거리는 시간.

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer() = default;
public:
	_float Get_TimeDelta() const {
		return m_fTimeDelta;
	}
public:
	HRESULT Ready_Timer();
	_float Compute_TimeDelta();
private:
	_float						m_fTimeDelta = 0.f;
private:
	LARGE_INTEGER				m_FrameTime;
	LARGE_INTEGER				m_FixTime;
	LARGE_INTEGER				m_LastTime;
	LARGE_INTEGER				m_CpuTick;
public:
	static CTimer* Create();
protected:
	virtual void Free();
};

END
#endif // Timer_h__
