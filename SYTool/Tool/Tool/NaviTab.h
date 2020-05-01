#pragma once


// CNaviTab 대화 상자
#include "Terrain.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "StaticObject.h"
#include "ToolPoint.h"
#include "ToolCell.h"

BEGIN(Engine)
class CComponent;
class CToolCollider;
class CSphereCollider;
END

class CNaviTab : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviTab)

public:
	CNaviTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaviTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnBnClickedNaviCreateMode();
	afx_msg void	OnBnClickedNaviModifyMode();
	afx_msg void	OnBnClickedNaviDeleteMode();
	afx_msg BOOL	OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnBnClickedNaviNoneMode();
	afx_msg void	OnLbnSelchangeNaviList();
	afx_msg void	OnBnClickedNaviDelete();
	afx_msg void	OnBnClickedShowNavi();
	afx_msg void	OnBnClickedShowNaviPoint();
	afx_msg void	OnBnClickedSaveNavi();
	afx_msg void	OnBnClickedLoadNavi();


	virtual BOOL	OnInitDialog();

	void			Get_NaviPointPos();
	
private:
	void			Modify_PointPosition(RECT  rc[9], CPoint& pt, short zDelta);
	void			Modify_NaviPointPos();

public:
	_int			m_iCurNaviMode = 3;		// 0-CREATE, 1-MODIFY, 2-DELETE, 3-NONE
	_int			m_iCellCnt = 0;
	_int			m_iCurCell = 0;
	CListBox		m_NaviList;

	CEdit			m_EditPosX;
	CEdit			m_EditPosY;
	CEdit			m_EditPosZ;

	_vec3			m_vNaviPos = { 0.f, 0.f,0.f };
	float			m_fPosX = 0.f;
	float			m_fPosY = 0.f;
	float			m_fPosZ = 0.f;

	CToolPoint*		m_pToolPoint[3];
	CToolPoint*		m_pPointTmp = nullptr;
	CToolCell*		m_pToolCell = nullptr;

	list<CToolPoint*>		m_pPointLst;
	list<CToolCell*>		m_pCellLst;

private:
	CMainFrame*		m_pMainFrm = nullptr;
	CToolView*		m_pToolView = nullptr;
	CButton			m_BnShowNavi;
	CButton			m_BnShowPoint;
};
