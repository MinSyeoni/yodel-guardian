// CameraTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CameraTool.h"
#include "afxdialogex.h"


// CameraTool 대화 상자

IMPLEMENT_DYNAMIC(CCameraTool, CDialogEx)

CCameraTool::CCameraTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CCameraTool::~CCameraTool()
{
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialogEx)
END_MESSAGE_MAP()


// CameraTool 메시지 처리기
