// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include "GraphicDevice.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "Management.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "Renderer.h"
#include "BaziorMgr.h"
#define MAX_LOADSTRING 100

// 전역 변수:
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HINSTANCE	g_hInst;							// 현재 인스턴스입니다.
HWND		g_hWnd;

_ulong Release_Singleton();

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;
	msg.message = WM_NULL;

	/*____________________________________________________________________
	MainApp 생성.
	______________________________________________________________________*/
	CMainApp* pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

	/*____________________________________________________________________
	Timer 생성.
	Timer_TimeDelta : GameObject들이 공통적으로 사용할 Timer
	Timer_FPS60		: FPS를 60으로 제한하기 위해 사용할 Timer
	______________________________________________________________________*/
	if (FAILED(Engine::CTimerMgr::Get_Instance()->Ready_Timer(L"Timer_TimeDelta")))
	{
		Engine::CTimerMgr::Get_Instance()->Destroy_Instance();
		return FALSE;
	}
	if (FAILED(Engine::CTimerMgr::Get_Instance()->Ready_Timer(L"Timer_FPS60")))
	{
		Engine::CTimerMgr::Get_Instance()->Destroy_Instance();
		return FALSE;
	}

	/*____________________________________________________________________
	Frame 생성.
	______________________________________________________________________*/
	if (FAILED(Engine::CFrameMgr::Get_Instance()->Ready_Frame(L"Frame60", 60.f)))
	{
		Engine::CFrameMgr::Get_Instance()->Destroy_Instance();
		return FALSE;
	}

    // 기본 메시지 루프입니다:
	while (true)
	{
		/*____________________________________________________________________
		종료 메시지가 발생할 때 까지 구동.
		______________________________________________________________________*/
		if (WM_QUIT == msg.message)
			break;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/*____________________________________________________________________
			프레임 제한에 사용할 Timer를 갱신. (Timer_FPS60)
			______________________________________________________________________*/
			Engine::CTimerMgr::Get_Instance()->SetUp_TimeDelta(L"Timer_FPS60");
			_float fTime_FPS60 = Engine::CTimerMgr::Get_Instance()->Get_TimeDelta(L"Timer_FPS60");

			/*____________________________________________________________________
			1초에 60번만 호출되도록 조건문 사용.
			______________________________________________________________________*/
			if (Engine::CFrameMgr::Get_Instance()->IsPermit_Call(L"Frame60", fTime_FPS60))
			{
				/*____________________________________________________________________
				GameObject들이 공통적으로 사용할 Timer 갱신. (Timer_TimeDelta)
				______________________________________________________________________*/
				Engine::CTimerMgr::Get_Instance()->SetUp_TimeDelta(L"Timer_TimeDelta");
				_float fTime_TimeDelta = Engine::CTimerMgr::Get_Instance()->Get_TimeDelta(L"Timer_TimeDelta");

				/*____________________________________________________________________
				MainApp - Update & Rendering.
				______________________________________________________________________*/
				pMainApp->Update_MainApp(fTime_TimeDelta);
				pMainApp->LateUpdate_MainApp(fTime_TimeDelta);
				pMainApp->Render_MainApp(fTime_TimeDelta);
			}
		}
	}

	/*____________________________________________________________________
	MainApp 제거.
	강제적으로 RefCnt를 0으로 만들어준다.
	______________________________________________________________________*/
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Engine::Safe_Release(pMainApp))
	{
		MSG_BOX(L"MainApp Release Failed");
		return FALSE;
	}

	/*____________________________________________________________________
	Singleton 객체 제거.
	______________________________________________________________________*/
	if (dwRefCnt = Release_Singleton())
	{
		MSG_BOX(L"Singleton Release Failed");
		return FALSE;
	}

	system("pause");

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	RECT rc{ 0, 0, WINCX, WINCY };

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							 WINPOS_X, WINPOS_Y,
							 rc.right - rc.left, 
							 rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(g_hWnd);
			break;

		default:
			break;
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

_ulong Release_Singleton()
{
#ifdef _DEBUG
	COUT_STR("-------------------------");
	COUT_STR("Destroy Singleton Object");
	COUT_STR("-------------------------");
#endif
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Engine::CFrameMgr::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CFrameMgr Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = Engine::CTimerMgr::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CTimerMgr Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = Engine::CMathMgr::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CMathMgr Release Failed");
		return dwRefCnt;
	}
	if (dwRefCnt = CManagement::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CManagement Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CRenderer::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CRenderer Release Failed");
		return dwRefCnt;
	}

	if (dwRefCnt = CObjectMgr::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CObjectMgr Release Failed");
		return dwRefCnt;
	}
	if (dwRefCnt = CComponentMgr::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CComponentMgr Release Failed");
		return dwRefCnt;
	}
	if (dwRefCnt = CMathMgr::Get_Instance()->Destroy_Instance())
	{
		return dwRefCnt;
	}
	if (dwRefCnt = CGraphicDevice::Get_Instance()->Destroy_Instance())
	{
		MSG_BOX(L"CDirectInput Release Failed");
		return dwRefCnt;
	}
	if (dwRefCnt = CColliderMgr::Get_Instance()->Destroy_Instance())
	{
		return dwRefCnt;


	}
	if (dwRefCnt = BaziorMgr::Get_Instance()->Destroy_Instance())
	{
			return dwRefCnt;
	}

	return 0;
}