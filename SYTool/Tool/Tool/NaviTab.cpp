// NaviTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "NaviTab.h"
#include "afxdialogex.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

// CNaviTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviTab, CDialogEx)

CNaviTab::CNaviTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

CNaviTab::~CNaviTab()
{
	for (auto& pPoint : m_pPointLstTmp)
		Engine::Safe_Release(pPoint);
	for (auto& pCell : m_pCellLstTmp)
		Engine::Safe_Release(pCell);
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_NaviList);
	DDX_Control(pDX, IDC_EDIT1, m_EditPosX[0]);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosY[0]);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosZ[0]);

	DDX_Control(pDX, IDC_EDIT13, m_EditPosX[1]);
	DDX_Control(pDX, IDC_EDIT14, m_EditPosY[1]);
	DDX_Control(pDX, IDC_EDIT4, m_EditPosZ[1]);

	DDX_Control(pDX, IDC_EDIT5, m_EditPosX[2]);
	DDX_Control(pDX, IDC_EDIT6, m_EditPosY[2]);
	DDX_Control(pDX, IDC_EDIT15, m_EditPosZ[2]);
	DDX_Text(pDX, IDC_EDIT1, m_fPosX[0]);
	DDX_Text(pDX, IDC_EDIT2, m_fPosY[0]);
	DDX_Text(pDX, IDC_EDIT3, m_fPosZ[0]);

	DDX_Text(pDX, IDC_EDIT13, m_fPosX[1]);
	DDX_Text(pDX, IDC_EDIT14, m_fPosY[1]);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ[1]);

	DDX_Text(pDX, IDC_EDIT5, m_fPosX[2]);
	DDX_Text(pDX, IDC_EDIT6, m_fPosY[2]);
	DDX_Text(pDX, IDC_EDIT15, m_fPosZ[2]);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CNaviTab::OnBnClickedNaviCreateMode)
	ON_BN_CLICKED(IDC_RADIO4, &CNaviTab::OnBnClickedNaviModifyMode)
	ON_BN_CLICKED(IDC_RADIO3, &CNaviTab::OnBnClickedNaviDeleteMode)
	ON_BN_CLICKED(IDC_RADIO5, &CNaviTab::OnBnClickedNaviNoneMode)
	ON_WM_MOUSEWHEEL()
	ON_LBN_SELCHANGE(IDC_LIST1, &CNaviTab::OnLbnSelchangeNaviList)
	ON_BN_CLICKED(IDC_BUTTON11, &CNaviTab::OnBnClickedNaviDelete)
END_MESSAGE_MAP()


// CNaviTab 메시지 처리기
void CNaviTab::OnBnClickedNaviCreateMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurNaviMode = 0;
}
void CNaviTab::OnBnClickedNaviModifyMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurNaviMode = 1;
}
void CNaviTab::OnBnClickedNaviDeleteMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurNaviMode = 2;
}
void CNaviTab::OnBnClickedNaviNoneMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurNaviMode = 3;
}


BOOL CNaviTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	RECT rc[9] = {};
	m_EditPosX[0].GetWindowRect(&rc[0]);
	m_EditPosY[0].GetWindowRect(&rc[1]);
	m_EditPosZ[0].GetWindowRect(&rc[2]);
	m_EditPosX[1].GetWindowRect(&rc[3]);
	m_EditPosY[1].GetWindowRect(&rc[4]);
	m_EditPosZ[1].GetWindowRect(&rc[5]);
	m_EditPosX[2].GetWindowRect(&rc[6]);
	m_EditPosY[2].GetWindowRect(&rc[7]);
	m_EditPosZ[2].GetWindowRect(&rc[8]);

	Modify_PointPosition(rc, pt, zDelta);

	UpdateData(FALSE);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CNaviTab::Modify_PointPosition(RECT  rc[9], CPoint& pt, short zDelta)
{
	if (PtInRect(&rc[0], pt))	// pt1 - PosX
	{
		if (zDelta > 0)
			m_fPosX[0] += 0.1f;
		else
			m_fPosX[0] -= 0.1f;
		m_vNaviPos[0].x = m_fPosX[0];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[1], pt))	// pt1 - PosY
	{
		if (zDelta > 0)
			m_fPosY[0] += 0.1f;
		else
			m_fPosY[0] -= 0.1f;
		m_vNaviPos[0].y = m_fPosY[0];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[2], pt))	// pt1 - PosZ
	{
		if (zDelta > 0)
			m_fPosZ[0] += 0.1f;
		else
			m_fPosZ[0] -= 0.1f;
		m_vNaviPos[0].z = m_fPosZ[0];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}

	if (PtInRect(&rc[3], pt))	// pt2 - PosX
	{
		if (zDelta > 0)
			m_fPosX[1] += 0.1f;
		else
			m_fPosX[1] -= 0.1f;
		m_vNaviPos[1].x = m_fPosX[1];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[4], pt))	// pt2 - PosY
	{
		if (zDelta > 0)
			m_fPosY[1] += 0.1f;
		else
			m_fPosY[1] -= 0.1f;
		m_vNaviPos[1].y = m_fPosY[1];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[5], pt))	// pt2 - PosZ
	{
		if (zDelta > 0)
			m_fPosZ[1] += 0.1f;
		else
			m_fPosZ[1] -= 0.1f;
		m_vNaviPos[1].z = m_fPosZ[1];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}

	if (PtInRect(&rc[6], pt))	// pt3 - PosX
	{
		if (zDelta > 0)
			m_fPosX[2] += 0.1f;
		else
			m_fPosX[2] -= 0.1f;
		m_vNaviPos[2].x = m_fPosX[2];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[7], pt))	// pt3 - PosY
	{
		if (zDelta > 0)
			m_fPosY[2] += 0.1f;
		else
			m_fPosY[2] -= 0.1f;
		m_vNaviPos[2].y = m_fPosY[2];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[8], pt))	// pt3 - PosZ
	{
		if (zDelta > 0)
			m_fPosZ[2] += 0.1f;
		else
			m_fPosZ[2] -= 0.1f;
		m_vNaviPos[2].z = m_fPosZ[2];

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
}

void CNaviTab::Modify_NaviPointPos()
{
	for (auto& pCell : m_pCellLstTmp)
	{
		if (pCell->m_pPointA == m_pPointTmp)
		{
			pCell->m_pPointA->m_pTransCom->Set_Pos(&m_vNaviPos[0]);
			m_pPointTmp->m_pTransCom->Set_Pos(&m_vNaviPos[0]);
			m_pPointTmp->Set_NaviPos(m_vNaviPos[0]);
			break;
		}
		else if (pCell->m_pPointB == m_pPointTmp)
		{
			pCell->m_pPointB->m_pTransCom->Set_Pos(&m_vNaviPos[1]);
			m_pPointTmp->m_pTransCom->Set_Pos(&m_vNaviPos[1]);
			m_pPointTmp->Set_NaviPos(m_vNaviPos[1]);
			break;
		}
		else if (pCell->m_pPointC == m_pPointTmp)
		{
			pCell->m_pPointC->m_pTransCom->Set_Pos(&m_vNaviPos[2]);
			m_pPointTmp->m_pTransCom->Set_Pos(&m_vNaviPos[2]);
			m_pPointTmp->Set_NaviPos(m_vNaviPos[2]);
			break;
		}
	}
}


BOOL CNaviTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO5);
	pButton->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNaviTab::OnLbnSelchangeNaviList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iCurSel = m_NaviList.GetCurSel();

	if (m_pPointLstTmp.empty() || m_pCellLstTmp.empty()) 
		return;

	int iTmp = 0;
	for (auto& pPoint : m_pPointLstTmp)
	{	
		if (iCurSel == iTmp)
		{
			pPoint->Set_CheckPoint(true);
			m_pPointTmp = pPoint;
		}
		if (iCurSel != iTmp)
			pPoint->Set_CheckPoint(false);		
		iTmp++;
	}

	for (auto& pCell : m_pCellLstTmp)
	{
		if (nullptr == m_pPointTmp)
			return;

		if (pCell->m_pPointA == m_pPointTmp)
		{
			Get_NaviPointPos(0);
			break;
		}
		else if (pCell->m_pPointB == m_pPointTmp)
		{
			Get_NaviPointPos(1);
			break;
		}
		else if (pCell->m_pPointC == m_pPointTmp)
		{
			Get_NaviPointPos(2);
			break;
		}
	}

	UpdateData(FALSE);
}

void CNaviTab::Get_NaviPointPos(_int iIdx)
{
	m_fPosX[iIdx] = m_pPointTmp->m_pSphereCol->Get_PareOriWorld()._41;
	m_fPosY[iIdx] = m_pPointTmp->m_pSphereCol->Get_PareOriWorld()._42;
	m_fPosZ[iIdx] = m_pPointTmp->m_pSphereCol->Get_PareOriWorld()._43;
	m_vNaviPos[iIdx] = { m_fPosX[iIdx], m_fPosY[iIdx], m_fPosZ[iIdx] };
}


void CNaviTab::OnBnClickedNaviDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (2 != m_iCurNaviMode)
	{
		MessageBox(L"DeleteMode 선택할 것");
		return;
	}
	if (m_pPointLstTmp.empty() || m_pCellLstTmp.empty())
		return;
	int i = m_NaviList.GetCount();

	CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_CELL].pop_back();
	m_pCellLstTmp.pop_back();

	//CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_POINT].pop_back();
	//CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_POINT].pop_back();
	//CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_POINT].pop_back();
	//m_pPointLstTmp.pop_back();
	//m_pPointLstTmp.pop_back();
	//m_pPointLstTmp.pop_back();
	// 뒤에서부터 삭제해야하는 대신, 포인트마다 인덱스를 보이게 할 것. 
	// 포인트도 삭제할 것.

	m_NaviList.DeleteString(i - 1);
}
