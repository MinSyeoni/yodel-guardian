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
	for (auto& pPoint : m_pPointLst)
		Engine::Safe_Release(pPoint);
	m_pPointLst.clear();
	
	for (auto& pCell : m_pCellLst)
		Engine::Safe_Release(pCell);
	m_pCellLst.clear();
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_NaviList);
	DDX_Control(pDX, IDC_EDIT1, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosY);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosZ);

	DDX_Text(pDX, IDC_EDIT1, m_fPosX);
	DDX_Text(pDX, IDC_EDIT2, m_fPosY);
	DDX_Text(pDX, IDC_EDIT3, m_fPosZ);
	DDX_Control(pDX, IDC_CHECK1, m_BnShowNavi);
	DDX_Control(pDX, IDC_CHECK2, m_BnShowPoint);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CNaviTab::OnBnClickedNaviCreateMode)
	ON_BN_CLICKED(IDC_RADIO4, &CNaviTab::OnBnClickedNaviModifyMode)
	ON_BN_CLICKED(IDC_RADIO3, &CNaviTab::OnBnClickedNaviDeleteMode)
	ON_BN_CLICKED(IDC_RADIO5, &CNaviTab::OnBnClickedNaviNoneMode)
	ON_WM_MOUSEWHEEL()
	ON_LBN_SELCHANGE(IDC_LIST1, &CNaviTab::OnLbnSelchangeNaviList)
	ON_BN_CLICKED(IDC_BUTTON11, &CNaviTab::OnBnClickedNaviDelete)
	ON_BN_CLICKED(IDC_CHECK1, &CNaviTab::OnBnClickedShowNavi)
	ON_BN_CLICKED(IDC_CHECK2, &CNaviTab::OnBnClickedShowNaviPoint)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviTab::OnBnClickedSaveNavi)
	ON_BN_CLICKED(IDC_BUTTON10, &CNaviTab::OnBnClickedLoadNavi)
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
	m_EditPosX.GetWindowRect(&rc[0]);
	m_EditPosY.GetWindowRect(&rc[1]);
	m_EditPosZ.GetWindowRect(&rc[2]);

	Modify_PointPosition(rc, pt, zDelta);

	UpdateData(FALSE);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CNaviTab::Modify_PointPosition(RECT  rc[9], CPoint& pt, short zDelta)
{
	if (PtInRect(&rc[0], pt))	// pt1 - PosX
	{
		if (zDelta > 0)
			m_fPosX += 0.1f;
		else
			m_fPosX -= 0.1f;
		m_vNaviPos.x = m_fPosX;

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[1], pt))	// pt1 - PosY
	{
		if (zDelta > 0)
			m_fPosY += 0.1f;
		else
			m_fPosY -= 0.1f;
		m_vNaviPos.y = m_fPosY;

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
	if (PtInRect(&rc[2], pt))	// pt1 - PosZ
	{
		if (zDelta > 0)
			m_fPosZ += 0.1f;
		else
			m_fPosZ -= 0.1f;
		m_vNaviPos.z = m_fPosZ;

		if (nullptr != m_pPointTmp && 1 == m_iCurNaviMode)	// 생성
			Modify_NaviPointPos();
	}
}

void CNaviTab::Modify_NaviPointPos()
{
	for (auto& pPoint : m_pPointLst)
	{
		if (pPoint->m_pTransCom->m_vInfo[Engine::INFO_POS] ==
			m_pPointTmp->m_pTransCom->m_vInfo[Engine::INFO_POS])
		{
			pPoint->m_pTransCom->Set_Pos(&m_vNaviPos);
			pPoint->Set_NaviPos(m_vNaviPos);
		}
	}	
}


BOOL CNaviTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO5);
	pButton->SetCheck(TRUE);

	m_BnShowNavi.SetCheck(true);
	m_BnShowPoint.SetCheck(true);

	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CToolView*>(m_pMainFrm->m_MainSplitWnd.GetPane(0, 1));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNaviTab::OnLbnSelchangeNaviList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iCurSel = m_NaviList.GetCurSel();

	if (m_pPointLst.empty() || m_pCellLst.empty()) 
		return;

	int iCntTmp = 0;
	for (auto& pCell : m_pCellLst)
	{
		if (iCntTmp == iCurSel)
			pCell->Set_IsCheckCell(true);
		else
			pCell->Set_IsCheckCell(false);
		iCntTmp++;
	}

	UpdateData(FALSE);
}
void CNaviTab::Get_NaviPointPos()
{
	m_vNaviPos = m_pPointTmp->m_pSphereCol->Get_Pos();
	m_fPosX = m_vNaviPos.x;
	m_fPosY = m_vNaviPos.y;
	m_fPosZ = m_vNaviPos.z;
}

void CNaviTab::OnBnClickedNaviDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (2 != m_iCurNaviMode)
	{
		MessageBox(L"DeleteMode 선택할 것");
		return;
	}

	if (m_pPointLst.empty() || m_pCellLst.empty() 
		|| CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_CELL].empty())
	{
		MessageBox(L"삭제할 NaviMesh가 없습니다.");
		return;
	}
	
	CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_CELL].pop_back();

	int iCurCnt = m_NaviList.GetCount();
	m_NaviList.DeleteString(iCurCnt - 1);
}

void CNaviTab::OnBnClickedShowNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iCheck = m_BnShowNavi.GetCheck();

	if(m_pCellLst.empty())
		return;

	if (1 == iCheck)
	{
		for (auto& pCell : m_pCellLst)
			pCell->Set_IsShowCell(true);
	}
	else
	{
		for (auto& pCell : m_pCellLst)
			pCell->Set_IsShowCell(false);
	}

	UpdateData(FALSE);
}
void CNaviTab::OnBnClickedShowNaviPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iCheck = m_BnShowPoint.GetCheck();

	if (m_pPointLst.empty())
		return;

	if (1 == iCheck)
	{
		for (auto& pPoint : m_pPointLst)
			pPoint->Set_IsShowPoint(true);
	}
	else
	{
		for (auto& pPoint : m_pPointLst)
			pPoint->Set_IsShowPoint(false);
	}

	UpdateData(FALSE);
}

void CNaviTab::OnBnClickedSaveNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[256] = L"";

	// GetCurrentDirectory: 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(256, szCurPath);

	// PathRemoveFileSpec: 현재 경로 상에서 파일명을 제거하는 함수.
	// 제거해야할 파일명이 없을 경우에는 가장 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data\\Navi");

	// 대화상자를 열 때 초기 경로를 설정한다.
	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		NAVICELL tNaviData;
		list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_CELL);

		// 일단 태그만 저장
		int		  iTagLength = 0;
		for (auto& pCell : pLst)
		{
			if (nullptr == dynamic_cast<CToolCell*>(pCell))
				return;

			tNaviData.PointA = dynamic_cast<CToolCell*>(pCell)->m_pPointA->m_pTransCom->m_vInfo[Engine::INFO_POS];
			tNaviData.PointB = dynamic_cast<CToolCell*>(pCell)->m_pPointB->m_pTransCom->m_vInfo[Engine::INFO_POS];
			tNaviData.PointC = dynamic_cast<CToolCell*>(pCell)->m_pPointC->m_pTransCom->m_vInfo[Engine::INFO_POS];

			WriteFile(hFile, &tNaviData, sizeof(NAVICELL), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}
void CNaviTab::OnBnClickedLoadNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// CFileDialog: 파일 저장 및 열기를 수행하는 대화상자에 해당하는 MFC 클래스.
	CFileDialog Dlg(TRUE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[256] = L"";

	// GetCurrentDirectory: 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(256, szCurPath);

	// PathRemoveFileSpec: 현재 경로 상에서 파일명을 제거하는 함수.
	// 제거해야할 파일명이 없을 경우에는 가장 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data\\Navi");

	// 대화상자를 열 때 초기 경로를 설정한다.
	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		NAVICELL tNaviData = {};
		CString	strIndex = L"";
		m_iCellCnt = 0;

		if (!CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_CELL).empty())
			CObjMgr::GetInstance()->ReleaseGroup(CObjMgr::OBJ_CELL);
		if (!CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_POINT).empty())
			CObjMgr::GetInstance()->ReleaseGroup(CObjMgr::OBJ_POINT);

		m_NaviList.ResetContent();

		for (auto& pPoint : m_pPointLst)
			Engine::Safe_Release(pPoint);
		m_pPointLst.clear();

		for (auto& pCell : m_pCellLst)
			Engine::Safe_Release(pCell);
		m_pCellLst.clear();

		while (1)
		{
			ReadFile(hFile, &tNaviData, sizeof(NAVICELL), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			int iPointCnt = 0;

			m_vNaviPos = tNaviData.PointA;
			m_pToolPoint[iPointCnt] = CToolPoint::Create(m_pToolView->Get_ToolViewDevice(), m_vNaviPos);
			CObjMgr::GetInstance()->AddObject(m_pToolPoint[iPointCnt], CObjMgr::OBJ_POINT);
			m_pPointLst.push_back(m_pToolPoint[iPointCnt]);
			iPointCnt++;

			m_vNaviPos = tNaviData.PointB;
			m_pToolPoint[iPointCnt] = CToolPoint::Create(m_pToolView->Get_ToolViewDevice(), m_vNaviPos);
			CObjMgr::GetInstance()->AddObject(m_pToolPoint[iPointCnt], CObjMgr::OBJ_POINT);
			m_pPointLst.push_back(m_pToolPoint[iPointCnt]);
			iPointCnt++;

			m_vNaviPos = tNaviData.PointC;
			m_pToolPoint[iPointCnt] = CToolPoint::Create(m_pToolView->Get_ToolViewDevice(), m_vNaviPos);
			CObjMgr::GetInstance()->AddObject(m_pToolPoint[iPointCnt], CObjMgr::OBJ_POINT);
			m_pPointLst.push_back(m_pToolPoint[iPointCnt]);
			iPointCnt++;

			if (3 == iPointCnt)
			{
				m_pToolCell = CToolCell::Create(m_pToolView->Get_ToolViewDevice(),
					m_pToolPoint[0], m_pToolPoint[1], m_pToolPoint[2]);
				CObjMgr::GetInstance()->AddObject(m_pToolCell, CObjMgr::OBJ_CELL);
				m_pCellLst.push_back(m_pToolCell);

				strIndex.Format(L"%d번 Cell", m_iCellCnt);
				m_NaviList.InsertString(m_iCellCnt, strIndex);
				m_iCellCnt++;

				iPointCnt = 0;
			}
		}
		m_pToolView->Invalidate(TRUE);

		CloseHandle(hFile);
	}
}
