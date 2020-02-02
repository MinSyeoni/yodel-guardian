
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyForm.h"

#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Management.h"
#include "Renderer.h"
#include "DirectInput.h"
#include "TimeMgr.h"
#include "FrameMgr.h"

HINSTANCE g_hInst;
HWND g_hWnd;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
	: m_pDeviceClass(CGraphicDevice::Get_Instance())
	, m_pComponentMgr(CComponentMgr::Get_Instance())
	, m_pObjectMgr(CObjectMgr::Get_Instance())
	, m_pManagement(CManagement::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Engine::Safe_Release(m_pComponentMgr);
	Engine::Safe_Release(m_pObjectMgr);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pGraphicDevice);
	Engine::Safe_Release(m_pDeviceClass);

	Engine::CFrameMgr::Get_Instance()->Destroy_Instance();
	Engine::CTimerMgr::Get_Instance()->Destroy_Instance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

HRESULT CToolView::Ready_MainApp()
{
	if(FAILED(SetUp_DefaultSetting(CGraphicDevice::MODE_WIN, g_iWinCX, g_iWinCY)))
		return E_FAIL;

	if (FAILED(Engine::CTimerMgr::Get_Instance()->Ready_Timer(L"Timer_TimeDelta")))
	{
		Engine::CTimerMgr::Get_Instance()->Destroy_Instance();
		return E_FAIL;
	}
	if (FAILED(Engine::CTimerMgr::Get_Instance()->Ready_Timer(L"Timer_FPS60")))
	{
		Engine::CTimerMgr::Get_Instance()->Destroy_Instance();
		return E_FAIL;
	}
	if (FAILED(Engine::CFrameMgr::Get_Instance()->Ready_Frame(L"Frame60", 60.f)))
	{
		Engine::CFrameMgr::Get_Instance()->Destroy_Instance();
		return E_FAIL;
	}
	return NOERROR;
}

_int CToolView::Update_MainApp(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pManagement, -1);

	///*____________________________________________________________________
	//Direct Input
	//______________________________________________________________________*/
	Engine::CDirectInput::Get_Instance()->SetUp_InputState();

	_int			iExitCode = 0;

	iExitCode = m_pManagement->Update_Management(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	return iExitCode;
}

_int CToolView::LateUpdate_MainApp(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pManagement, -1);

	_int			iExitCode = 0;

	iExitCode = m_pManagement->LateUpdate_Management(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	return iExitCode;
}

HRESULT CToolView::Render_MainApp(const _float& fTimeDelta)
{
	if (nullptr == m_pDeviceClass ||
		nullptr == m_pManagement ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pManagement->Render_Management(fTimeDelta);
	m_pRenderer->Render_Renderer(fTimeDelta);

	return NOERROR;
}

HRESULT CToolView::SetUp_DefaultSetting(CGraphicDevice::WINMODE eMode, const _uint& uiWidth, const _uint& uiHeight)
{
	NULL_CHECK_RETURN(m_pDeviceClass, E_FAIL);

	if (FAILED(m_pDeviceClass->Ready_GraphicDevice(g_hWnd, g_hInst, eMode, uiWidth, uiHeight)))
		return E_FAIL;

	m_pGraphicDevice = m_pDeviceClass->Get_GraphicDevice();
	NULL_CHECK_RETURN(m_pGraphicDevice, E_FAIL);

	m_pCommandList = m_pDeviceClass->Get_CommandList();
	NULL_CHECK_RETURN(m_pCommandList, E_FAIL);

	if (FAILED(m_pRenderer->Ready_Renderer(m_pGraphicDevice, m_pCommandList)))
		return E_FAIL;

	if (FAILED(Engine::CDirectInput::Get_Instance()->Ready_InputDevice(g_hInst, g_hWnd)))
	{
		CDirectInput::Get_Instance()->Destroy_Instance();
		return E_FAIL;
	}

	return S_OK;
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();
	
	Ready_MainApp();

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pMyForm = dynamic_cast<CMyForm*>(m_pMainFrm->m_MainSplitWnd.GetPane(0, 0));
	m_pToolView = dynamic_cast<CToolView*>(m_pMainFrm->m_MainSplitWnd.GetPane(0, 1));
}
