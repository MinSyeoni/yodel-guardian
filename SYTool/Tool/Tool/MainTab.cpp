// MainTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MainTab.h"
#include "afxdialogex.h"


// CMainTab 대화 상자

IMPLEMENT_DYNAMIC(CMainTab, CDialogEx)

CMainTab::CMainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMainTab, pParent)
{

}

CMainTab::~CMainTab()
{
}

void CMainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MainTab);
}


BEGIN_MESSAGE_MAP(CMainTab, CDialogEx)
END_MESSAGE_MAP()


// CMainTab 메시지 처리기


BOOL CMainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
