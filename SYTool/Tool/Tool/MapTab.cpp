// MapTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MapTab.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapTab 대화 상자

IMPLEMENT_DYNAMIC(CMapTab, CDialogEx)

CMapTab::CMapTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iCntX(129)
	, m_iCntZ(129)
	, m_iInterval(1)
	, m_strTexList(_T(""))
	, m_bIsMode(false)
{
}

CMapTab::~CMapTab()
{
}

void CMapTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iCntX);
	DDX_Text(pDX, IDC_EDIT2, m_iCntZ);
	DDX_Text(pDX, IDC_EDIT3, m_iInterval);
	DDX_LBString(pDX, IDC_LIST1, m_strTexList);
	DDX_Control(pDX, IDC_LIST1, m_TexListBox);
	DDX_Control(pDX, IDC_LIST2, m_HeightTexLst);
}


BEGIN_MESSAGE_MAP(CMapTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTab::OnBnClickedWireMode)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTab::OnBnClickedSolidMode)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTab::OnBnClickedCreateTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTab::OnBnClickedSetTexture)
END_MESSAGE_MAP()


// CMapTab 메시지 처리기


void CMapTab::OnBnClickedWireMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
	{
		MessageBox(L"Terrain 없다 만들어라");
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton->SetCheck(FALSE);
	}
	else
		dynamic_cast<CTerrain*>(CObjMgr::GetInstance()->
			GetGameObject(CObjMgr::OBJ_TERRAIN))->Set_TerrainCurState(CTerrain::STATE_WIRE);
}

void CMapTab::OnBnClickedSolidMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
	{
		MessageBox(L"Terrain 없다 만들어라");
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton->SetCheck(FALSE);
	}
	else
		dynamic_cast<CTerrain*>(CObjMgr::GetInstance()->
			GetGameObject(CObjMgr::OBJ_TERRAIN))->Set_TerrainCurState(CTerrain::STATE_SOLID);
}

void CMapTab::OnBnClickedCreateTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
	{
		Engine::CGameObject* pTerrain = CTerrain::Create(m_pToolView->Get_ToolViewDevice());
		CObjMgr::GetInstance()->AddObject(pTerrain, CObjMgr::OBJ_TERRAIN);
		return;
	}
	else
	{
		return;
		//m_iCntX = GetDlgItemInt(IDC_EDIT1);
		//m_iCntZ = GetDlgItemInt(IDC_EDIT2);
		//m_iInterval = GetDlgItemInt(IDC_EDIT3);

		//CObjMgr::GetInstance()->ReleaseGroup(CObjMgr::OBJ_TERRAIN);
		//Engine::CGameObject* pTerrain = CTerrain::Create(m_pToolView->Get_ToolViewDevice());
		//CObjMgr::GetInstance()->AddObject(pTerrain, CObjMgr::OBJ_TERRAIN);
	}
}

void CMapTab::Change_TerrainTexture()
{
	m_iIdx = m_TexListBox.GetCurSel();
	CString str;
	m_TexListBox.GetText(m_iIdx, str);
	int i = 0;
	for (; i < str.GetLength(); ++i)
	{
		if (isdigit(str[i]))
			break;
	}
	str.Delete(0, i);
	int iDrawId = _ttoi(str);
	dynamic_cast<CTerrain*> (CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))->Set_TerrainDrawID(iDrawId);
}

void CMapTab::Load_TerrainTexture(CString strPath, CListBox* pListBox, CString pDir)
{
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strPath + _T("/*.*"));
	int iIdx = 0;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		//찾은 폴더
		else if (finder.IsDirectory())
		{
			CString strFileName = finder.GetFileName();
			CString strFolderPath = finder.GetFilePath();

			pListBox->InsertString(iIdx, strFileName);
			iIdx++;

			Load_TerrainTexture(strFolderPath, pListBox, pDir);
		}
		else if (finder.IsArchived())
		{
			CString		strFileName = finder.GetFileName();
			CString		strExtension = PathFindExtension(strFileName);

			if (strExtension.CompareNoCase(pDir) == 0)
				pListBox->InsertString(iIdx, strFileName);
			iIdx++;
		}
	}

	finder.Close();
}

BOOL CMapTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CToolView*>(m_pMainFrm->m_MainSplitWnd.GetPane(0, 1));

	Load_TerrainTexture(L"../Resources/Texture/Terrain/", &m_TexListBox, L".png");
	Load_TerrainTexture(L"../Resources/Texture/HeightMap/", &m_HeightTexLst, L".bmp");

	m_HeightTexLst.InsertString(0, L"NONE");
	m_TexListBox.SetCurSel(0);
	m_HeightTexLst.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapTab::OnBnClickedSetTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Change_TerrainTexture();
	//Change_HeightMapTexture();
}

void CMapTab::Change_HeightMapTexture()
{
	CString strPath;
	_int iIdx = m_HeightTexLst.GetCurSel();
	m_HeightTexLst.GetText(iIdx, strPath);

	strPath = L"../Resources/Texture/HeightMap/" + strPath;
	
	Engine::CGameObject* pObj = CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN);
	Engine::CComponent* pComponent = pObj->Get_Component(L"Com_Buffer", Engine::ID_STATIC);
	dynamic_cast<Engine::CTerrainTex*>(pComponent)->Ready_Buffer(strPath, m_iInterval);
}
