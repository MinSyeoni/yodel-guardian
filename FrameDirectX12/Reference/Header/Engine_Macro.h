#ifndef Engine_Macro_h__
#define Engine_Macro_h__


#define DEVICE  CGraphicDevice::Get_Instance()->Get_GraphicDevice()
namespace Engine
{
    #define         TEXTUREPATH  L"../../Resource/Texture/"

	#define			DEADOBJ 0
	#define			NOEVENT 1

	#define			WINSIZEX 1000
	#define			WINSIZEY 800

#define       SHADOWDEPTHSIZEX 8000
#define       SHADOWDEPTHSIZEY 8000


	#define			MIN_STR	64
	#define			MAX_STR	256

	#define			USING(NAMESPACE)	using namespace NAMESPACE;

	#define			MSGERR_BOX(_message)			MessageBoxW(NULL, _message, L"System Message", MB_OK)
	#define			TAGMSGERR_BOX(_tag,_message)	MessageBoxW(NULL, TEXT(_message), _tag, MB_OK)

	#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
	#define			END						}

	#ifdef ENGINE_EXPORTS
	#define ENGINE_DLL _declspec(dllexport)
	#else
	#define ENGINE_DLL _declspec(dllimport)
	#endif // ENGINE_EXPORTS

	#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){__debugbreak();return;}}
	
	#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){__debugbreak();return _return;}}
	
	#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__debugbreak();}}
	
	#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__debugbreak();return _return;}}
	
	
	#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __debugbreak(); return E_FAIL;}
	
	#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __debugbreak(); return _return;}
	
	#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __debugbreak();return E_FAIL;}
	
	#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __debugbreak();return _return;}
}

#define NO_COPY(CLASSNAME)								\
private:												\
CLASSNAME(const CLASSNAME&);							\
CLASSNAME& operator = (const CLASSNAME&);	

#define DECLARE_SINGLETON(CLASSNAME)					\
NO_COPY(CLASSNAME)										\
private:												\
	static CLASSNAME* m_pInstance;						\
public:													\
	static CLASSNAME* Get_Instance(void);				\
public:													\
	unsigned long Destroy_Instance(void);

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
CLASSNAME* CLASSNAME::m_pInstance = nullptr;			\
CLASSNAME* CLASSNAME::Get_Instance(void)				\
{														\
	if(nullptr == m_pInstance)							\
	{													\
		m_pInstance = new CLASSNAME;					\
	}													\
	return m_pInstance;									\
}														\
unsigned long CLASSNAME::Destroy_Instance(void)			\
{														\
	unsigned long dwRefCnt = 0;							\
	if (nullptr != m_pInstance)							\
	{													\
		dwRefCnt = m_pInstance->Release();				\
		if (0 == dwRefCnt)								\
			m_pInstance = nullptr;						\
	}													\
	return dwRefCnt;									\
}

/*____________________________________________________________________
[ Matrix / Vector ]
______________________________________________________________________*/
#define INIT_MATRIX				XMMatrixIdentity()
#define INIT_VEC2(input)		_vec2(input, input)
#define INIT_VEC3(input)		_vec3(input, input, input)
#define INIT_VEC4(input)		_vec4(input, input, input, input)

#define MatrixInverse(matrix)	XMMatrixInverse(&XMMatrixDeterminant(matrix), matrix)

/*____________________________________________________________________
[ Random RGB ]
______________________________________________________________________*/
#define RANDOM_COLOR	_rgba(rand() / _float(RAND_MAX), rand() / _float(RAND_MAX), rand() / _float(RAND_MAX), 1.f)

/*____________________________________________________________________
[ Shader ConstBuffer ]
______________________________________________________________________*/
#define INIT_CB_256(input)	((sizeof(input) + 255) & ~255)
#define ROOT_PARAMETER_OBJECT 0
#define ROOT_PARAMETER_CAMERA 1

/*____________________________________________________________________
[ DirectInput ]
______________________________________________________________________*/
#define KEY_PRESSING(bykey)							CDirectInput::Get_Instance()->Key_Pressing(bykey)
#define KEY_UP(byKey)								CDirectInput::Get_Instance()->Key_Up(byKey)
#define KEY_DOWN(byKey)								CDirectInput::Get_Instance()->Key_Down(byKey)
#define KEY_COMBINE(byFirstKey, bySecondKey)		CDirectInput::Get_Instance()->Key_Combine(byFirstKey, bySecondKey)
#define KEY_COMBINE_UP(byFirstKey, bySecondKey)		CDirectInput::Get_Instance()->Key_CombineUp(byFirstKey, bySecondKey)
#define KEY_COMBINE_DOWN(byFirstKey, bySecondKey)	CDirectInput::Get_Instance()->Key_CombineDown(byFirstKey, bySecondKey)

#define MOUSE_PRESSING(eMouseKey)					CDirectInput::Get_Instance()->Mouse_KeyPressing(eMouseKey)
#define MOUSE_KEYUP(eMouseKey)						CDirectInput::Get_Instance()->Mouse_KeyUp(eMouseKey)
#define MOUSE_KEYDOWN(eMouseKey)					CDirectInput::Get_Instance()->Mouse_KeyDown(eMouseKey)

/*____________________________________________________________________
[ cout Debug ]
______________________________________________________________________*/
#define COUT_STR(STR)	cout << STR << endl;



/*____________________________________________________________________
[ Ã¥ ¿¹Á¦ ]
______________________________________________________________________*/
inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
		: ErrorCode(hr)
		, FunctionName(functionName)
		, Filename(filename)
		, LineNumber(lineNumber)
	{
	}

	std::wstring ToString() const
	{
		// Get the string description of the error code.
		_com_error err(ErrorCode);
		std::wstring msg = err.ErrorMessage();

		return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
	}

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int LineNumber = -1;
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#endif // Engine_Macro_h__
