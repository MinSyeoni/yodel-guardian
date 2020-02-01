#pragma once

#include "Engine_Include.h"


// 모든 클래스의 부모
// 모든 객체에 참조 갯수를 체크한다. (객체의 주소를 몇개의 포인터형 변수가 가지고 있는지를 체크)
namespace Engine
{
	class ENGINE_DLL CBase abstract
	{
	protected:
		explicit CBase(void);
		virtual ~CBase(void);
	public:
		unsigned long AddRef(void);
		unsigned long Release(void);
	protected:
		unsigned long		m_dwRefCnt = 0; // 참조갯수
	protected:
		virtual void Free(void) = 0;
	};

}