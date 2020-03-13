﻿// Myform.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Myform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyform::OnTcnSelchangeTab)
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
	m_MainTab.InsertItem(1, L"NaviTool", 1);

	m_MainTab.SetCurSel(0);

	CRect rc;
	m_MainTab.GetWindowRect(&rc);

	m_pMapTab = new CMapTab;
	m_pMapTab->Create(IDD_DIALOG1, &m_MainTab);
	m_pMapTab->MoveWindow(0, 25, rc.Width(), rc.Height());
	m_pMapTab->ShowWindow(SW_SHOW);

	m_pNaviTab = new CNaviTab;
	m_pNaviTab->Create(IDD_DIALOG2, &m_MainTab);
	m_pNaviTab->MoveWindow(0, 25, rc.Width(), rc.Height());
	m_pNaviTab->ShowWindow(SW_HIDE);
}

void CMyform::Free()
{
	Engine::Safe_Delete(m_pMapTab);
	Engine::Safe_Delete(m_pNaviTab);
}


void CMyform::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCurTab = m_MainTab.GetCurSel();

	switch (iCurTab)
	{
	case 0:
		m_iCurTab = 0;
		m_pMapTab->ShowWindow(SW_SHOW);
		m_pNaviTab->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_iCurTab = 1;
		m_pMapTab->ShowWindow(SW_HIDE);
		m_pNaviTab->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;
}
