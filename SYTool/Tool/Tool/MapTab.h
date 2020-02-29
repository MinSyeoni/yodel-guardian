#pragma once


// CMapTab 대화 상자
#include "Terrain.h"
#include "MainFrm.h"
#include "ToolView.h"

BEGIN(Engine)
class CComponent;
END

class CMapTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTab)

public:
	CMapTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedWireMode();
	afx_msg void OnBnClickedSolidMode();
	afx_msg void OnBnClickedCreateTerrain();
	afx_msg void OnBnClickedSetTexture();
	virtual BOOL OnInitDialog();

private:
	void					Load_TerrainTexture(CString strPath, CListBox* pListBox, CString pDir);
	void					Change_TerrainTexture();
	
	void					Change_HeightMapTexture();

private:
	_bool					m_bIsMode;
	Engine::CComponent*		m_pComponent = nullptr;

private:
	int						m_iCntX;
	int						m_iCntZ;
	int						m_iInterval;

	_int					m_iIdx = 0;

	CListBox				m_TexListBox;
	CListBox				m_HeightTexLst;
	CString					m_strTexList;

	CMainFrame*				m_pMainFrm = nullptr;
	CToolView*				m_pToolView = nullptr;


public:

};
