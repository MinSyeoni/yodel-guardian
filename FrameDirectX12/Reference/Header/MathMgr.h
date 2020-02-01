#pragma once
#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CMathMgr : public CBase
{
	DECLARE_SINGLETON(CMathMgr)

private:
	explicit CMathMgr();
	virtual ~CMathMgr();

private:
	virtual void Free();
};

END
