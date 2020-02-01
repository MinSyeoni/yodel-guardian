#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T> 
	void Safe_Delete(T& pointer)
	{
		if(NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T> 
	void Safe_Delete_Array(T& pointer)
	{
		if(NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}

	template <typename T> 
	DWORD Safe_Release(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if(NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if(dwRefCnt == 0)
				pointer = NULL;
		}
		return dwRefCnt;
	}

	template <typename T> 
	DWORD Safe_Release_Delete(T& pointer)
	{
		DWORD	dwRefCnt = 0;
		if(NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if(dwRefCnt == 0)
			{
				delete pointer;
				pointer = NULL;
			}
		}
		return dwRefCnt;
	}

	template <typename T> 
	void Safe_Single_Destory(T& pointer)
	{
		if(NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

}
#endif // Engine_Function_h__
