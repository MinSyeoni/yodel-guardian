#include "Base.h"

using namespace Engine;

CBase::CBase()
	: m_dwRefCnt(0)
{
}


CBase::~CBase()
{
}

unsigned long CBase::AddRef(void)
{
	return ++m_dwRefCnt;
}

unsigned long CBase::Release(void)
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}
