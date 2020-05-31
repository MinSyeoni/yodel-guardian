// MapTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MapTab.h"
#include "afxdialogex.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapTab 대화 상자

IMPLEMENT_DYNAMIC(CMapTab, CDialogEx)

CMapTab::CMapTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iCntX(768)
	, m_iCntZ(768)
	, m_iInterval(1)
	, m_strTexList(_T(""))
	, m_fPosX(0.f)
	, m_fPosY(0.f)
	, m_fPosZ(0.f)
	, m_fScaleX(0.1f)
	, m_fScaleY(0.1f)
	, m_fScaleZ(0.1f)
	, m_fRotX(0.f)
	, m_fRotY(0.f)
	, m_fRotZ(0.f)
	, m_fTerrainPosX(0.f)
	, m_fTerrainPosY(0.f)
	, m_fTerrainPosZ(0.f)
	, m_fBrushRange(1)
	, m_fBrushHeight(1)
{
}

CMapTab::~CMapTab()
{
	for (auto& pCol : m_pColliderLst)
		Engine::Safe_Release(pCol);
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
	DDX_Control(pDX, IDC_CHECK1, m_BnColliderMode);
	DDX_Control(pDX, IDC_CHECK2, m_BnShowCollider);
	DDX_Control(pDX, IDC_CHECK3, m_BnSetOn);
	DDX_Control(pDX, IDC_COMBO1, m_CbColliderID);
	DDX_Text(pDX, IDC_EDIT27, m_fTerrainPosX);

	DDX_Text(pDX, IDC_EDIT34, m_fTerrainPosY);
	DDX_Text(pDX, IDC_EDIT35, m_fTerrainPosZ);
	DDX_Text(pDX, IDC_EDIT36, m_fBrushRange);
	DDX_Control(pDX, IDC_EDIT36, m_EditRange);
	DDX_Control(pDX, IDC_EDIT37, m_EditHeight);
	DDX_Text(pDX, IDC_EDIT37, m_fBrushHeight);
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
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTab::OnBnClickedSaveStaticObj)
	ON_BN_CLICKED(IDC_BUTTON9, &CMapTab::OnBnClickedLoadStaticObj)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTab::OnBnClickedCheck_ColliderMode)
	ON_BN_CLICKED(IDC_RADIO11, &CMapTab::OnBnClickedColliderSphereMode)
	ON_BN_CLICKED(IDC_RADIO12, &CMapTab::OnBnClickedColliderBoxMode)
	ON_BN_CLICKED(IDC_CHECK2, &CMapTab::OnBnClickedColliderShow)
	ON_BN_CLICKED(IDC_CHECK3, &CMapTab::OnBnClickedSetOn_Mesh)
	ON_BN_CLICKED(IDC_RADIO13, &CMapTab::OnBnClickedAddHeight)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTab::OnBnClickedSaveTerrain)
	ON_BN_CLICKED(IDC_RADIO19, &CMapTab::OnBnClickedBoxBrush)
	ON_BN_CLICKED(IDC_RADIO20, &CMapTab::OnBnClickedCircleBrush)
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
		return;
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

	Initialize_String();

	m_vTerrainPos = { 0.f, 0.f, 0.f };
	m_vMeshScale = { 0.1f, 0.1f, 0.1f };

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO8);
	pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO12);
	pButton->SetCheck(TRUE);

	m_iColliderState = 1;

	ZeroMemory(m_matColliderWorld, sizeof(_matrix));

	int i = sizeof(MESHDATA);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTab::Initialize_String()
{
	m_HeightTexLst.InsertString(0, L"NONE");
	m_TexListBox.SetCurSel(0);
	m_HeightTexLst.SetCurSel(0);

	m_StaticObjLst.InsertString(0, L"NONE");
	m_StaticObjLst.SetCurSel(0);

	m_CbColliderID.AddString(L"For_Collider");
	m_CbColliderID.AddString(L"For_Terrain");
	m_CbColliderID.AddString(L"For_Combat");
	m_CbColliderID.AddString(L"For_Trigger");
	m_CbColliderID.SetCurSel(0);
}

void CMapTab::OnBnClickedSetTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
		return;

	if (0 == m_iTexToolMode)
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
	dynamic_cast<Engine::CTerrainTex*>(pComponent)->Ready_Buffer(strPath, m_iCntX, m_iCntZ, m_iInterval);
}

void CMapTab::OnBnClickedTextureMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTexToolMode = 0;

	for (int i = 0; i < 3; ++i)
		m_bIsBrushMode[i] = false;

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO13);
	pButton->SetCheck(FALSE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO14);
	pButton->SetCheck(FALSE);
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

	if (true == m_bIsColliderMode)
	{
		// 나중에 옵션 넣어서 바꿔야 함, 함수로 정리할 것
		_matrix matScale, matTrans;
		_vec3 vPosTemp = { 0.f,0.f,0.f };
		int iColliderID = m_CbColliderID.GetCurSel();

		if (0 == m_iColliderState) // sphere
		{
			if (true == m_bIsSetOnMesh)
			{
				if (nullptr == m_pPickStaticObj) return;
				m_pPickStaticObj->Get_StaticTranscom()->Get_Info(Engine::INFO_POS, &m_vMeshPos);
				m_vMeshScale = m_pPickStaticObj->Get_StaticTranscom()->m_vScale;
			}

			m_pSphereCol = Engine::CSphereCollider::Create(
				m_pToolView->Get_ToolViewDevice(),
				m_vMeshScale.x,
				(CToolCollider::COLLID)iColliderID);

			D3DXMatrixScaling(&matScale, m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
			D3DXMatrixTranslation(&matTrans, m_fPosX, m_fPosY, m_fPosZ);

			m_matColliderWorld = matScale * matTrans;

			m_pSphereCol->Set_PareOriWorld(m_matColliderWorld);
			m_pSphereCol->Set_WorldMat(m_matColliderWorld);
			m_pColliderLst.push_back(m_pSphereCol);
			return;
		}
		else // box
		{
			if (true == m_bIsSetOnMesh)
			{
				/*
					m_pBoxCol = Engine::CBoxCollider::Create(
					m_pToolView->Get_ToolViewDevice(),
					m_pPickStaticObj->Get_StaticMesh()->Get_VtxPos(),
					m_pPickStaticObj->Get_StaticMesh()->Get_NumVtx(),
					m_pPickStaticObj->Get_StaticMesh()->Get_Stride(),
					CToolCollider::COLID_COLLIDER);
	
				m_pBoxCol->Set_PareOriWorld(*m_pPickStaticObj->Get_StaticTranscom()->Get_WorldMatrix());
				m_pBoxCol->Set_WorldMat(*m_pPickStaticObj->Get_StaticTranscom()->Get_WorldMatrix());
				*/
				if (nullptr == m_pPickStaticObj) return;
				m_pPickStaticObj->Get_StaticTranscom()->Get_Info(Engine::INFO_POS, &m_vMeshPos);
				m_vMeshScale = m_pPickStaticObj->Get_StaticTranscom()->m_vScale;
			}

			m_pBoxCol = Engine::CBoxCollider::Create(
				m_pToolView->Get_ToolViewDevice(),
				vPosTemp, m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z,
				(CToolCollider::COLLID)iColliderID);

			D3DXMatrixScaling(&matScale, m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
			D3DXMatrixTranslation(&matTrans, m_fPosX, m_fPosY, m_fPosZ);

			m_matColliderWorld = matScale * matTrans;

			m_pBoxCol->Set_PareOriWorld(m_matColliderWorld);
			m_pBoxCol->Set_WorldMat(m_matColliderWorld);
			m_pColliderLst.push_back(m_pBoxCol);
			return;
		}
	}
	else
	{
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
}

void CMapTab::OnBnClickedStaticDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (2 != m_iObjToolMode)
	{
		MessageBox(L"Delete_Mode 클릭");
		return;
	}

	if (true == m_bIsColliderMode)
	{
		if (m_pColliderLst.empty())
		{
			MessageBox(L"삭제 할 Collider 없다 만들어라");
			return;
		}
		if (m_pPickCollider != nullptr && true == m_bIsPickingCollider)
		{
			for (auto& iter = m_pColliderLst.begin(); iter != m_pColliderLst.end();)
			{
				if (m_pPickCollider == (*iter))
				{
					Engine::Safe_Release(*iter);
					iter = m_pColliderLst.erase(iter);
					break;
				}
				else
					++iter;
			}
		}
	}
	else
	{
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

	m_EditRange.GetWindowRect(&rc[9]);
	m_EditHeight.GetWindowRect(&rc[10]);
	ModifyBrush(rc, pt, zDelta);

	UpdateData(FALSE);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CMapTab::ModifyBrush(RECT  rc[11], CPoint& pt, short zDelta)
{
	if (PtInRect(&rc[9], pt))
	{
		if (zDelta > 0)
			m_fBrushRange += 1.f;
		else
			m_fBrushRange -= 1.f;
	}
	if (PtInRect(&rc[10], pt))
	{
		if (zDelta > 0)
			m_fBrushHeight += 0.1f;
		else
			m_fBrushHeight -= 0.1f;
	}
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

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._11 = m_vMeshScale.x;
			m_pPickCollider->Set_Scale(m_vMeshScale);
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
	}
	if (PtInRect(&rc[1], pt))	// ScaleY
	{
		if (zDelta > 0)
			m_fScaleY += 0.1f;
		else
			m_fScaleY -= 0.1f;
		m_vMeshScale.y = m_fScaleY;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._22 = m_vMeshScale.y;
			m_pPickCollider->Set_Scale(m_vMeshScale);
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
	}
	if (PtInRect(&rc[2], pt))	// ScaleZ
	{
		if (zDelta > 0)
			m_fScaleZ += 0.1f;
		else
			m_fScaleZ -= 0.1f;
		m_vMeshScale.z = m_fScaleZ;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._33 = m_vMeshScale.z;
			m_pPickCollider->Set_Scale(m_vMeshScale);
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
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

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Rotation(Engine::ROT_X, m_vMeshRot.x);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			m_vMeshRot.x = m_fRotX;
			m_pPickCollider->Set_Angle(&m_vMeshRot);
		}
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

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Rotation(Engine::ROT_Y, m_vMeshRot.y);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			m_vMeshRot.y = m_fRotY;
			m_pPickCollider->Set_Angle(&m_vMeshRot);
		}
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

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Rotation(Engine::ROT_Z, m_vMeshRot.z);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			m_vMeshRot.z = m_fRotZ;
			m_pPickCollider->Set_Angle(&m_vMeshRot);
		}
	}

	if (PtInRect(&rc[6], pt))	// PosX
	{
		if (zDelta > 0)
			m_fPosX += 0.1f;
		else
			m_fPosX -= 0.1f;
		m_vMeshPos.x = m_fPosX;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Pos(&m_vMeshPos);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._41 = m_fPosX;
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
	}
	if (PtInRect(&rc[7], pt))	// PosY
	{
		if (zDelta > 0)
			m_fPosY += 0.1f;
		else
			m_fPosY -= 0.1f;
		m_vMeshPos.y = m_fPosY;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Pos(&m_vMeshPos);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._42 = m_fPosY;
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
	}
	if (PtInRect(&rc[8], pt))	// PosZ
	{
		if (zDelta > 0)
			m_fPosZ += 0.1f;
		else
			m_fPosZ -= 0.1f;
		m_vMeshPos.z = m_fPosZ;

		if (true == m_bIsPickingStaticObj && nullptr != m_pPickStaticObj && false == m_bIsColliderMode)
			m_pPickStaticObj->Get_StaticTranscom()->
			Set_Pos(&m_vMeshPos);
		else if (true == m_bIsPickingCollider && nullptr != m_pPickCollider && true == m_bIsColliderMode)
		{
			_matrix matCol = m_pPickCollider->Get_PareOriWorld();
			matCol._43 = m_fPosZ;
			m_pPickCollider->Set_PareOriWorld(matCol);
		}
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
	if (true == m_bIsColliderMode)
		lstrcat(szCurPath, L"\\Data\\Collider");
	else
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

		if (true == m_bIsColliderMode)
		{
			COLLIDER	tColliderData;

			int	  iPathLength = 0;
			int	  iNameLength = 0;
			for (auto& pCollider : m_pColliderLst)
			{
				if (nullptr == pCollider)
					return;

				tColliderData.iType = pCollider->Get_CollShape();
				tColliderData.iOptionID = pCollider->Get_ColID();
				tColliderData.iColID = pCollider->Get_CollKind();
				tColliderData.vCenter.x = pCollider->Get_PareOriWorld()._41;
				tColliderData.vCenter.y = pCollider->Get_PareOriWorld()._42;
				tColliderData.vCenter.z = pCollider->Get_PareOriWorld()._43;

				tColliderData.vRotate = pCollider->Get_Angle();
				tColliderData.iGroupID = pCollider->Get_ColliderGroupID();

				//if (1 == tColliderData.iType)
				//{
					tColliderData.vScale.x = pCollider->Get_PareOriWorld()._11;
					tColliderData.vScale.y = pCollider->Get_PareOriWorld()._22;
					tColliderData.vScale.z = pCollider->Get_PareOriWorld()._33;
				//}
				//else
				//	tColliderData.vScale = pCollider->Get_Scale();

				tColliderData.dwNumVtx = pCollider->Get_dwNumVtx();
				tColliderData.dwVtxPos = pCollider->Get_VtxPos();
				tColliderData.dwStride = pCollider->Get_dwStride();
				tColliderData.matWorld = pCollider->Get_PareOriWorld();

				WriteFile(hFile, &tColliderData, sizeof(COLLIDER), &dwByte, nullptr);
			}
		}
		else
		{
			MESHDATA tMeshData;
			list<Engine::CGameObject*> pLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);

			// 일단 태그만 저장
			int		  iTagLength = 0;
			for (auto& pMesh : pLst)
			{
				if (nullptr == pMesh)
					return;

				auto pMeshTemp = dynamic_cast<CStaticObject*>(pMesh);
				iTagLength = lstrlen(pMeshTemp->m_szMeshTag) + 1;
				lstrcpy(tMeshData.m_MeshTag, pMeshTemp->m_szMeshTag);

				tMeshData.vScale = pMeshTemp->Get_StaticTranscom()->m_vScale;
				pMeshTemp->Get_StaticTranscom()->Get_Info(Engine::INFO_POS, &tMeshData.vPos);
				tMeshData.vRotate = pMeshTemp->Get_StaticTranscom()->m_vAngle;

				WriteFile(hFile, &iTagLength, sizeof(int), &dwByte, nullptr);
				WriteFile(hFile, &tMeshData, sizeof(MESHDATA), &dwByte, nullptr);
			}
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

	if (true == m_bIsColliderMode)
		lstrcat(szCurPath, L"\\Data\\Collider");
	else
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

		if (true == m_bIsColliderMode)
		{
			CToolCollider* pCollider = nullptr;

			COLLIDER tColliderData = {};
			_matrix matScale, matRot, matTrans, matWorld;

			if (!m_pColliderLst.empty())
			{
				for (auto& pCol : m_pColliderLst)
					Engine::Safe_Release(pCol);
				m_pColliderLst.clear();
			}

			while (1)
			{
				ReadFile(hFile, &tColliderData, sizeof(COLLIDER), &dwByte, nullptr);

				if (dwByte == 0)
					break;

				_vec3 vPosTemp = { 0.f,0.f,0.f };
				_vec3 vScaleTemp = { 0.f,0.f,0.f };
				// 메쉬의 크기 2.5, 2.5, 1 그대로 가져와서 크기가 이상해지는 것
				m_vMeshPos = tColliderData.vCenter;
				m_vMeshScale = tColliderData.vScale;
				m_vMeshRot = tColliderData.vRotate;
				int iColliderOptionID = tColliderData.iOptionID;
				matWorld = tColliderData.matWorld;

				if (0 == tColliderData.iType)	// 박스
				{
					// 충돌체 scale 구조가 이상함. 구조 내부에서 *10 해줌
					// 클라 충돌체 정보랑 다를 것이기 때문에 정보 불러올 때 문제가 생기면 바꿔줄 것.
					pCollider = Engine::CBoxCollider::Create(
						m_pToolView->Get_ToolViewDevice(),
						vPosTemp, m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z,
						(CToolCollider::COLLID)iColliderOptionID);
				}
				else if (1 == tColliderData.iType)	// 구
				{
					pCollider = Engine::CSphereCollider::Create(
						m_pToolView->Get_ToolViewDevice(),
						m_vMeshScale.x,
						(CToolCollider::COLLID)iColliderOptionID);
				}		

				pCollider->Set_PareOriWorld(matWorld);
				pCollider->Set_WorldMat(matWorld);
				pCollider->Set_Angle(&m_vMeshRot);

				m_pColliderLst.push_back(pCollider);
			}
		}
		else
		{
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

			int			 iTagLength = 0;
			CString		 strTag;

			while (1)
			{
				ReadFile(hFile, &iTagLength, sizeof(int), &dwByte, nullptr);
				ReadFile(hFile, &tObjData, sizeof(MESHDATA), &dwByte, nullptr);

				if (dwByte == 0)
					break;

				strTag = tObjData.m_MeshTag;

				m_vMeshPos = tObjData.vPos;
				m_vMeshScale = tObjData.vScale;
				m_vMeshRot = tObjData.vRotate;

				Engine::CGameObject* pStaticObj = CStaticObject::Create(
					m_pToolView->Get_ToolViewDevice(),
					m_vMeshPos, m_vMeshRot, m_vMeshScale,
					strTag.GetBuffer());
				CObjMgr::GetInstance()->AddObject(pStaticObj, CObjMgr::OBJ_OBJECT);
			}
		}
		m_pToolView->Invalidate(TRUE);

		CloseHandle(hFile);
	}
}

void CMapTab::OnBnClickedCheck_ColliderMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iCheck = m_BnColliderMode.GetCheck();

	if (1 == iCheck)
	{
		m_bIsColliderMode = true;
		MessageBox(L"Collider 모드로 변환합니다.");
	}
	else
	{
		m_bIsColliderMode = false;
		MessageBox(L"StaticObject 모드로 변환합니다.");
	}

	m_vMeshPos = { 0.f,0.f,0.f };
	m_vMeshRot = { 0.f,0.f,0.f };
	m_vMeshScale = { 2.f, 2.f, 2.f };
	m_fPosX = 0.f, m_fPosY = 0.f, m_fPosZ = 0.f;
	m_fRotX = 0.f, m_fRotY = 0.f, m_fRotZ = 0.f;
	m_fScaleX = 2.f, m_fScaleY = 2.f, m_fScaleZ = 2.f;
	UpdateData(FALSE);
}

void CMapTab::OnBnClickedColliderSphereMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iColliderState = 0;
}

void CMapTab::OnBnClickedColliderBoxMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iColliderState = 1;
}

void CMapTab::OnBnClickedColliderShow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCheck = m_BnShowCollider.GetCheck();

	if (1 == iCheck)
		m_bIsColliderShow = true;
	else
		m_bIsColliderShow = false;
}

void CMapTab::OnBnClickedSetOn_Mesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCheck = m_BnSetOn.GetCheck();

	if (1 == iCheck)
		m_bIsSetOnMesh = true;
	else
		m_bIsSetOnMesh = false;
}

void CMapTab::OnBnClickedAddHeight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (1 == m_iTexToolMode)
	{
		m_bIsBrushMode[0] = true;
		m_bIsBrushMode[1] = false;
	}
	else
	{
		MessageBox(L"Height모드 먼저 클릭할 것");
		return;
	}
}

void CMapTab::OnBnClickedSaveTerrain()
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
	
	if (m_iTexToolMode == 1)
		lstrcat(szCurPath, L"\\Data\\Terrain\\HeightMap");
	else
		return;

	// 대화상자를 열 때 초기 경로를 설정한다.
	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		TERRAIN_DATA tTerrainData;

		if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
			return;

		LPDIRECT3DTEXTURE9 pHeight = nullptr;
		D3DXCreateTexture(Engine::CGraphicDev::GetInstance()->GetDevice(), m_iCntX, m_iCntZ, m_iInterval, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pHeight);
		D3DLOCKED_RECT LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));	// 제로 메모리 필수

		pHeight->LockRect(0, &LockRect, nullptr, 0);
		DWORD* pColor = (DWORD*)LockRect.pBits;

		Engine::CGameObject* pObj = CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN);
		Engine::CTerrainTex* pBufferCom = dynamic_cast<Engine::CTerrainTex*>(pObj->Get_Component(L"Com_Buffer", Engine::ID_STATIC));
		auto pBufferPos = pBufferCom->Get_VtxPos();

		for (_ulong i = 0; i < m_iCntX; ++i)
		{
			for (_int j = 0; j < m_iCntZ; ++j)
			{
				_ulong dwIndex = (i * m_iCntX) + j;
				_ulong dwInvndex = ((m_iCntX - i - 1) * m_iCntX) + j;
				D3DXCOLOR temp = *pBufferPos[dwInvndex];

				temp.a = pBufferPos[dwInvndex].y / 7.f;
				temp.r = pBufferPos[dwInvndex].y / 7.f;
				temp.g = pBufferPos[dwInvndex].y / 7.f;
				temp.b = pBufferPos[dwInvndex].y / 7.f;

				pColor[dwIndex] = temp;

				//tTerrainData.fHeight = 
			}
		}
		pHeight->UnlockRect(0);

		auto pBMPFileName = PathFindFileName(strFileName); // 경로에서 파일 이름만 찾기 
		PathRenameExtension(pBMPFileName, L".bmp");		// 파일 확장자 교체
		CString strPath;
		strPath = L"../Resources/Texture/HeightMap/" + (CString)pBMPFileName;
		D3DXSaveTextureToFile(strPath, D3DXIFF_BMP, pHeight, NULL);

		tTerrainData.dwVtxCntX = m_iCntX;
		tTerrainData.dwVtxCntZ = m_iCntZ;
		tTerrainData.dwInterval = m_iInterval;

		WriteFile(hFile, &tTerrainData, sizeof(TERRAIN_DATA), &dwByte, nullptr);
		
		CloseHandle(hFile);
	}
}


void CMapTab::OnBnClickedBoxBrush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iBrushShape = 0;
}


void CMapTab::OnBnClickedCircleBrush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iBrushShape = 1;
}
