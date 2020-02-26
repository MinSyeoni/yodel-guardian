// Myform.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Myform.h"


// CMyform

IMPLEMENT_DYNCREATE(CMyform, CFormView)

CMyform::CMyform()
	: CFormView(IDD_Myform)
{

}

CMyform::~CMyform()
{
	Free();
}

void CMyform::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MainTab);
}

BEGIN_MESSAGE_MAP(CMyform, CFormView)
END_MESSAGE_MAP()


// CMyform 진단

#ifdef _DEBUG
void CMyform::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyform::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyform 메시지 처리기


void CMyform::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_MainTab.InsertItem(0, L"MapTool", 0);

	m_MainTab.SetCurSel(0);

	CRect rc;
	m_MainTab.GetWindowRect(&rc);

	m_pMapTab = new CMapTab;
	m_pMapTab->Create(IDD_DIALOG1, &m_MainTab);
	m_pMapTab->MoveWindow(0, 25, rc.Width(), rc.Height());
	m_pMapTab->ShowWindow(SW_SHOW);



}

void CMyform::Free()
{
	Engine::Safe_Delete(m_pMapTab);
}
