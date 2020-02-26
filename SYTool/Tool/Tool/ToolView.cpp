
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView
HINSTANCE g_hInst;
HWND	  g_hWnd;

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	Engine::Safe_Delete(m_pTerrain);
	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pDevice);
	Engine::DestroyUtility();
	Engine::DestroyResources();
	Engine::DestroySystem();

	Engine::CGraphicDev::GetInstance()->DestroyInstance();
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
	Render_MainApp();
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

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

HRESULT CToolView::Render_MainApp()
{
	_int			iExitCode = 0;

	if (nullptr == m_pGraphicDev ||
		nullptr == m_pDevice ||
		nullptr == m_pManagement )
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));
	
	if (m_pTerrain != nullptr)
		m_pTerrain->Render_Object();

	m_pGraphicDev->Render_End();

	return iExitCode;
}

_int CToolView::Update_MainApp(const _float& fTimeDelta)
{
	_int			iExitCode = 0;

	if (nullptr == m_pManagement)
		return -1;

	m_pManagement->Update_Scene(fTimeDelta);

	if(m_pTerrain != nullptr)
		m_pTerrain->Update_Object(fTimeDelta);

	return iExitCode;
}

HRESULT CToolView::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting(CGraphicDev::MODE_WIN, g_iWinCX, g_iWinCY)))
		return E_FAIL;
}

HRESULT  CToolView::Ready_Default_Setting(CGraphicDev::WINMODE eMode, 
										const _uint& iWinCX,
										const _uint& iWinCY)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd, eMode, iWinCX, iWinCY, &m_pGraphicDev)))
		return E_FAIL;

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);
	m_pDevice->AddRef();

	if (FAILED(Engine::Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;

	return NOERROR;
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	Ready_MainApp();
	Ready_Buffer_Setting();

	Engine::Create_Management(m_pDevice, &m_pManagement);
	m_pManagement->AddRef();

	//////////////테스트/////////////////
	m_pTerrain = CTerrain::Create(m_pDevice);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), float(g_iWinCX) / float(g_iWinCY), 1.f, 1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CToolView::Ready_Buffer_Setting()
{
	if (FAILED(Engine::Reserve_ContainerSize(RESOURCE_END), E_FAIL));

	if (FAILED(Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX)))
		return;

	if (FAILED(Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_Terrain",
		Engine::BUFFER_TERRAINTEX,
		129,
		129,
		1)))
		return;

	//if (FAILED(Engine::Ready_Mesh(m_pDevice,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navigation",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL)));

	if (FAILED((Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_CubeTex",
		Engine::BUFFER_CUBETEX))))
		return;

	if (FAILED(Engine::Ready_Texture(m_pDevice,
		RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../Resources/Texture/Terrain/Terrain%d.png", 1)))
		return;

	//if (FAILED(Engine::Ready_Texture(m_pDevice,
	//	RESOURCE_STAGE,
	//	L"Texture_Brush",
	//	Engine::TEX_NORMAL,
	//	L"../../../Resource/Texture/Brush/Brush%d.tga", 2)))
	//	return;
}
