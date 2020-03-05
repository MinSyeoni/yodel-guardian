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
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
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
	DDX_Control(pDX, IDC_LIST3, m_StaticObjLst);
	DDX_Control(pDX, IDC_EDIT4, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT5, m_EditPosY);
	DDX_Control(pDX, IDC_EDIT6, m_EditPosZ);
	DDX_Control(pDX, IDC_EDIT7, m_EditScaleX);
	DDX_Control(pDX, IDC_EDIT8, m_EditScaleY);
	DDX_Control(pDX, IDC_EDIT9, m_EditScaleZ);
	DDX_Control(pDX, IDC_EDIT10, m_EditRotX);
	DDX_Control(pDX, IDC_EDIT11, m_EditRotY);
	DDX_Control(pDX, IDC_EDIT12, m_EditRotZ);
	DDX_Text(pDX, IDC_EDIT4, m_fPosX);
	DDX_Text(pDX, IDC_EDIT5, m_fPosY);
	DDX_Text(pDX, IDC_EDIT6, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT10, m_fRotX);
	DDX_Text(pDX, IDC_EDIT11, m_fRotY);
	DDX_Text(pDX, IDC_EDIT12, m_fRotZ);
}


BEGIN_MESSAGE_MAP(CMapTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTab::OnBnClickedWireMode)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTab::OnBnClickedSolidMode)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTab::OnBnClickedCreateTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTab::OnBnClickedSetTexture)
	ON_BN_CLICKED(IDC_RADIO3, &CMapTab::OnBnClickedTextureMode)
	ON_BN_CLICKED(IDC_RADIO4, &CMapTab::OnBnClickedSplattingMode)
	ON_BN_CLICKED(IDC_RADIO5, &CMapTab::OnBnClickedHeightMode)
	ON_BN_CLICKED(IDC_RADIO6, &CMapTab::OnBnClickedStaticWireMode)
	ON_BN_CLICKED(IDC_RADIO7, &CMapTab::OnBnClickedStaticSolidMode)
	ON_BN_CLICKED(IDC_RADIO8, &CMapTab::OnBnClickedStaticCreateMode)
	ON_BN_CLICKED(IDC_RADIO10, &CMapTab::OnBnClickedStaticModyMode)
	ON_BN_CLICKED(IDC_RADIO9, &CMapTab::OnBnClickedStaticDeleteMode)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTab::OnBnClickedStaticCreate)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTab::OnBnClickedStaticDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTab::OnBnClickedStaticSet)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTab::OnBnClickedSaveStaticObj)
	ON_BN_CLICKED(IDC_BUTTON9, &CMapTab::OnBnClickedLoadStaticObj)
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
	m_iCntX = GetDlgItemInt(IDC_EDIT1);
	m_iCntZ = GetDlgItemInt(IDC_EDIT2);
	m_iInterval = GetDlgItemInt(IDC_EDIT3);

	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
	{
		Engine::CGameObject* pTerrain = CTerrain::Create(m_pToolView->Get_ToolViewDevice());
		CObjMgr::GetInstance()->AddObject(pTerrain, CObjMgr::OBJ_TERRAIN);
		return;
	}
	else
	{
		Engine::CGameObject* pObj = CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN);
		Engine::CComponent* pComponent = pObj->Get_Component(L"Com_Buffer", Engine::ID_STATIC);
		dynamic_cast<Engine::CTerrainTex*>(pComponent)->Ready_Buffer(m_iCntX, m_iCntZ, m_iInterval);
		return;
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

void CMapTab::Load_ResourceList(CString strPath, CListBox* pListBox, CString pDir)
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

			Load_ResourceList(strFolderPath, pListBox, pDir);
		}
		else if (finder.IsArchived())
		{
			CString		strFileName = finder.GetFileName();
			CString		strExtension = PathFindExtension(strFileName);

			if (strExtension.CompareNoCase(pDir) == 0)
			{
				pListBox->InsertString(iIdx, strFileName);
				iIdx++;
			}
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

	Load_ResourceList(L"../Resources/Texture/Terrain/", &m_TexListBox, L".png");
	Load_ResourceList(L"../Resources/Texture/HeightMap/", &m_HeightTexLst, L".bmp");
	Load_ResourceList(L"../Resources/StaticMesh/", &m_StaticObjLst, L".X");

	m_HeightTexLst.InsertString(0, L"NONE");
	m_TexListBox.SetCurSel(0);	
	m_HeightTexLst.SetCurSel(0);

	m_StaticObjLst.InsertString(0, L"NONE");
	m_StaticObjLst.SetCurSel(0);

	m_vMeshScale = { 1.f, 1.f, 1.f };

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO8);
	pButton->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTab::OnBnClickedSetTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(0 == m_iTexToolMode)
		Change_TerrainTexture();
	else if (1 == m_iTexToolMode)
		Change_HeightMapTexture();
}

void CMapTab::Change_HeightMapTexture()
{
	CString strPath;
	_int iIdx = m_HeightTexLst.GetCurSel();
	m_HeightTexLst.GetText(iIdx, strPath);

	strPath = L"../Resources/Texture/HeightMap/" + strPath;
	
	Engine::CGameObject* pObj = CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN);
	Engine::CComponent* pComponent = pObj->Get_Component(L"Com_Buffer", Engine::ID_STATIC);
	dynamic_cast<Engine::CTerrainTex*>(pComponent)->Ready_Buffer((TCHAR*)(LPCTSTR)strPath, m_iCntX, m_iCntZ, m_iInterval);
}

void CMapTab::OnBnClickedTextureMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTexToolMode = 0;
}

void CMapTab::OnBnClickedHeightMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTexToolMode = 1;
}

void CMapTab::OnBnClickedSplattingMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTexToolMode = 2;
}



void CMapTab::OnBnClickedStaticWireMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
	{
		MessageBox(L"Object 없다 만들어라");
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO6);
		pButton->SetCheck(FALSE);
	}
	else	// 나중에 물체 클릭해서 바꿀수 있도록?
	{
		list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
		for (auto& pObject : pLst)
			dynamic_cast<CStaticObject*>(pObject)->Set_StaticCurState(CStaticObject::STATIC_WIRE);
	}
}


void CMapTab::OnBnClickedStaticSolidMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
	{
		MessageBox(L"Object 없다 만들어라");
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO7);
		pButton->SetCheck(FALSE);
	}
	else
	{
		list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
		for (auto& pObject : pLst)
			dynamic_cast<CStaticObject*>(pObject)->Set_StaticCurState(CStaticObject::STATIC_SOLID);
	}
}

void CMapTab::OnBnClickedStaticCreateMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iObjToolMode = 0;
}
void CMapTab::OnBnClickedStaticModyMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iObjToolMode = 1;
}
void CMapTab::OnBnClickedStaticDeleteMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iObjToolMode = 2;
}


void CMapTab::OnBnClickedStaticCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (0 != m_iObjToolMode)
	{
		MessageBox(L"Create_Mode 클릭");
		return;
	}

	CString strTag;
	_int iIdx = m_StaticObjLst.GetCurSel();
	m_StaticObjLst.GetText(iIdx, strTag);

	if (L"NONE" == strTag)
	{
		MessageBox(L"만들고 싶은 StaticObject 리스트에서 클릭할 것");
		return;
	}

	Engine::CGameObject* pStaticObj = CStaticObject::Create(m_pToolView->Get_ToolViewDevice(),
										m_vMeshPos, m_vMeshRot, m_vMeshScale,
										strTag.GetBuffer());
	CObjMgr::GetInstance()->AddObject(pStaticObj, CObjMgr::OBJ_OBJECT);
}


void CMapTab::OnBnClickedStaticDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (2 != m_iObjToolMode)
	{
		MessageBox(L"Delete_Mode 클릭");
		return;
	}

	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
	{
		MessageBox(L"삭제 할 Object 없다 만들어라");
		return;
	}

	if (m_pPickStaticObj != nullptr && true == m_bIsPickingStaticObj)
	{
		list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
		for (auto& pObject : pLst)
		{
			if (dynamic_cast<CStaticObject*>(pObject) == m_pPickStaticObj)
			{
				dynamic_cast<CStaticObject*>(pObject)->m_bisDead = true;
				m_bIsPickingStaticObj = false;
				m_pPickStaticObj = nullptr;
				break;
			}
		}
	}
}


void CMapTab::OnBnClickedStaticSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CMapTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	RECT rc[11] = {};
	m_EditScaleX.GetWindowRect(&rc[0]);
	m_EditScaleY.GetWindowRect(&rc[1]);
	m_EditScaleZ.GetWindowRect(&rc[2]);
	m_EditRotX.GetWindowRect(&rc[3]);
	m_EditRotY.GetWindowRect(&rc[4]);
	m_EditRotZ.GetWindowRect(&rc[5]);
	m_EditPosX.GetWindowRect(&rc[6]);
	m_EditPosY.GetWindowRect(&rc[7]);
	m_EditPosZ.GetWindowRect(&rc[8]);

	ModifyStaticObj(rc, pt, zDelta);

	UpdateData(FALSE);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CMapTab::ModifyStaticObj(RECT  rc[11], CPoint& pt, short zDelta)
{
	if (PtInRect(&rc[0], pt))	// ScaleX
	{
		if (zDelta > 0)
			m_fScaleX += 0.1f;
		else
			m_fScaleX -= 0.1f;
		m_vMeshScale.x = m_fScaleX;
		
		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
	}
	if (PtInRect(&rc[1], pt))	// ScaleY
	{
		if (zDelta > 0)
			m_fScaleY += 0.1f;
		else
			m_fScaleY -= 0.1f;
		m_vMeshScale.y = m_fScaleY;
		
		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
	}
	if (PtInRect(&rc[2], pt))	// ScaleZ
	{
		if (zDelta > 0)
			m_fScaleZ += 0.1f;
		else
			m_fScaleZ -= 0.1f;
		m_vMeshScale.z = m_fScaleZ;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
	}

	if (PtInRect(&rc[3], pt))	// RotX
	{
		if (zDelta > 0)
		{
			m_fRotX += 0.1f;
			m_vMeshRot.x = 0.1f;
		}
		else
		{
			m_fRotX -= 0.1f;
			m_vMeshRot.x = -0.1f;
		}

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Rotation(Engine::ROT_X, m_vMeshRot.x);
	}
	if (PtInRect(&rc[4], pt))	// RotY
	{
		if (zDelta > 0)
		{
			m_fRotY += 0.1f;
			m_vMeshRot.y = 0.1f;
		}
		else
		{
			m_fRotY -= 0.1f;
			m_vMeshRot.y = -0.1f;
		}

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Rotation(Engine::ROT_Y, m_vMeshRot.y);
	}
	if (PtInRect(&rc[5], pt))	// RotZ
	{
		if (zDelta > 0)
		{
			m_fRotZ += 0.1f;
			m_vMeshRot.z = 0.1f;
		}
		else
		{
			m_fRotZ -= 0.1f;
			m_vMeshRot.z = -0.1f;
		}

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Rotation(Engine::ROT_Z, m_vMeshRot.z);
	}

	if (PtInRect(&rc[6], pt))	// PosX
	{
		if (zDelta > 0)
			m_fPosX += 0.1f;
		else
			m_fPosX -= 0.1f;
		m_vMeshPos.x = m_fPosX;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Pos(&m_vMeshPos);
	}
	if (PtInRect(&rc[7], pt))	// PosY
	{
		if (zDelta > 0)
			m_fPosY += 0.1f;
		else
			m_fPosY -= 0.1f;
		m_vMeshPos.y = m_fPosY;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Pos(&m_vMeshPos);
	}
	if (PtInRect(&rc[8], pt))	// PosZ
	{
		if (zDelta > 0)
			m_fPosZ += 0.1f;
		else
			m_fPosZ -= 0.1f;
		m_vMeshPos.z = m_fPosZ;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj)
			m_pPickStaticObj->Get_StaticMeshTranscom()->
			Set_Pos(&m_vMeshPos);
	}
}


void CMapTab::OnBnClickedSaveStaticObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[256] = L"";

	// GetCurrentDirectory: 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(256, szCurPath);

	// PathRemoveFileSpec: 현재 경로 상에서 파일명을 제거하는 함수.
	// 제거해야할 파일명이 없을 경우에는 가장 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data\\StaticObj");

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
		MESHDATA tMeshData;

		list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);

		// 일단 태그만 저장
		//int	  iPathLength = 0;
		//int	  iNameLength = 0;
		int		  iTagLength = 0;
		for (auto& pMesh : pLst)
		{
			if (nullptr == pMesh)
				return;

			auto pMeshTemp = dynamic_cast<CStaticObject*>(pMesh);

			//iNameLength = lstrlen(pMeshTemp->m_szFilePath) + 1;
			//lstrcpy(tMeshData.szFilePath, pMeshTemp->m_szFilePath);
			//iNameLength = lstrlen(pMeshTemp->m_szFileName) + 1;
			//lstrcpy(tMeshData.szFileName, pMeshTemp->m_szFileName);
			iTagLength = lstrlen(pMeshTemp->m_szMeshTag) + 1;
			lstrcpy(tMeshData.m_MeshTag, pMeshTemp->m_szMeshTag);

			tMeshData.vScale = pMeshTemp->Get_StaticMeshTranscom()->m_vScale;
			pMeshTemp->Get_StaticMeshTranscom()->Get_Info(Engine::INFO_POS, &tMeshData.vPos);
			tMeshData.vRotate = pMeshTemp->Get_StaticMeshTranscom()->m_vAngle;

			//tMeshData.byMeshID = pMeshTemp->Get_StaticObjID();
			//tMeshData.byDrawID = pMeshTemp->Get_StaticObjDrawID();

			//WriteFile(hFile, &iPathLength, sizeof(int), &dwByte, nullptr);
			//WriteFile(hFile, &iNameLength, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &iTagLength, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &tMeshData, sizeof(MESHDATA), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CMapTab::OnBnClickedLoadStaticObj()
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
	lstrcat(szCurPath, L"\\Data\\StaticObj");

	// 대화상자를 열 때 초기 경로를 설정한다.
	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		CStaticObject* pStaticObj = nullptr;

		MESHDATA tObjData = {};

		if (!CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT).empty())
		{
			list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
			for (auto& pGameObj : pLst)
			{
				dynamic_cast<CStaticObject*>(pGameObj)->m_bisDead = true;
			}
			pLst.clear();
		}

		//int		 iPathLength = 0;
		//int		 iNameLength = 0;
		int			 iTagLength = 0;
		CString		 strTag;

		while (1)
		{
			ReadFile(hFile, &iTagLength, sizeof(int), &dwByte, nullptr);
			//ReadFile(hFile, &iPathLength, sizeof(int), &dwByte, nullptr);
			//ReadFile(hFile, &iNameLength, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &tObjData, sizeof(MESHDATA), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			//m_strMeshFullName = tObjData.szFileName;
			//m_strMeshPathName = tObjData.szFilePath;
			strTag = tObjData.m_MeshTag;

			m_vMeshPos = tObjData.vPos;
			m_vMeshScale = tObjData.vScale;
			m_vMeshRot = tObjData.vRotate;
			//m_byMeshID = tObjData.byMeshID;
			//m_byDrawID = tObjData.byDrawID;

			Engine::CGameObject* pStaticObj = CStaticObject::Create(m_pToolView->Get_ToolViewDevice(),
				m_vMeshPos, m_vMeshRot, m_vMeshScale,
				strTag.GetBuffer());
			CObjMgr::GetInstance()->AddObject(pStaticObj, CObjMgr::OBJ_OBJECT);
		}
		m_pToolView->Invalidate(TRUE);

		CloseHandle(hFile);
	}
}
