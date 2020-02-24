// MyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MyForm.h"


// MyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MyForm)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
END_MESSAGE_MAP()


// MyForm 진단

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm 메시지 처리기
