#ifndef KeyMgr_h__
#define KeyMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr(void);
	~CKeyMgr(void);
public:
	void KeyCheck(void);
public:
	bool KeyUp(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);

	private:
		DWORD m_dwCurKey;
		DWORD m_dwKeyPressed;
		DWORD m_dwKeyDown;

public:
	virtual void	Free(void);
};

END




#endif // KeyMgr_h__
