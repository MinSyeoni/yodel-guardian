#pragma once



// CMyform 폼 보기
#include "MapTab.h"
#include "NaviTab.h"

class CMapTab;
class CNaviTab;
class CMyform : public CFormView
{
	DECLARE_DYNCREATE(CMyform)

protected:
	CMyform();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyform();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Myform };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
	//CMapTab		 Get_MapTab() { return m_pMapTab; };

	CMapTab*	m_pMapTab = nullptr;
	CNaviTab*	m_pNaviTab = nullptr;

	_int		m_iCurTab = 0;	// 0-MAP, 1-NAVI
private:
	void		Free();

private:
	CTabCtrl	m_MainTab;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};


