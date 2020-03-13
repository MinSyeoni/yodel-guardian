
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView
HINSTANCE g_hInst;
HWND	  g_hWnd;

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
	CObjMgr::GetInstance()->DestroyInstance();
	CToolCamera::GetInstance()->DestroyInstance();

	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pDevice);

	Engine::DestroyUtility();
	Engine::DestroyResources();
	Engine::DestroySystem();

	CPickingMgr::GetInstance()->DestroyInstance();
	Engine::CKeyMgr::GetInstance()->DestroyInstance();

	Engine::CGraphicDev::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	Render_MainApp();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

HRESULT CToolView::Render_MainApp()
{
	_int			iExitCode = 0;

	if (nullptr == m_pGraphicDev ||
		nullptr == m_pDevice ||
		nullptr == m_pManagement)
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	m_pManagement->Render_Scene(m_pDevice);
	CObjMgr::GetInstance()->Render_Object();

	// 나중에 고쳐야함
	if (!m_pMapTab->m_pColliderLst.empty() && true == m_pMapTab->m_bIsColliderShow)
	{
		for (auto& pCol : m_pMapTab->m_pColliderLst)
		{
			pCol->Update_Collider(&pCol->Get_PareOriWorld());
			pCol->Render_Collider();
		}
	}

	m_pGraphicDev->Render_End();

	return iExitCode;
}

_int CToolView::Update_MainApp(const _float& fTimeDelta)
{
	_int			iExitCode = 0;

	if (nullptr == m_pManagement)
		return -1;

	m_pManagement->Update_Scene(fTimeDelta);

	Engine::CKeyMgr::GetInstance()->KeyCheck();
	CObjMgr::GetInstance()->Update_Object(fTimeDelta);
	CToolCamera::GetInstance()->Update_Camera(fTimeDelta);

	return iExitCode;
}

HRESULT CToolView::Ready_MainApp()
{
	if (FAILED(Ready_Default_Setting(CGraphicDev::MODE_WIN, g_iWinCX, g_iWinCY)))
		return E_FAIL;
	return NOERROR;
}

HRESULT  CToolView::Ready_Default_Setting(CGraphicDev::WINMODE eMode,
	const _uint& iWinCX,
	const _uint& iWinCY)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd, eMode, iWinCX, iWinCY, &m_pGraphicDev)))
		return E_FAIL;

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);
	m_pDevice->AddRef();

	if (FAILED(Engine::Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;

	return NOERROR;
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	Ready_MainApp();
	Ready_Buffer_Setting();

	Engine::Create_Management(m_pDevice, &m_pManagement);
	m_pManagement->AddRef();

	Initalize_Object();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitWnd.GetPane(0, 1));
	m_pMyForm = dynamic_cast<CMyform*>(pMainFrm->m_MainSplitWnd.GetPane(0, 0));
	m_pMapTab = m_pMyForm->m_pMapTab;
	m_pNaviTab = m_pMyForm->m_pNaviTab;
}

void CToolView::Initalize_Object()
{
	CToolCamera::GetInstance()->SetGrapicDevice(m_pDevice);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), float(g_iWinCX) / float(g_iWinCY), 1.f, 1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	// For.Timer_Default
	if (FAILED(Engine::Add_Timer(L"Timer_Default")))
		return;
	// For.Timer_60
	if (FAILED(Engine::Add_Timer(L"Timer_60")))
		return;
	// For.Timer_60
	if (FAILED(Engine::Ready_Frame(L"Frame60", 60.f)))
		return;
	// For.Font
	if (FAILED(Engine::Ready_Font(m_pDevice, L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL));
}

void CToolView::Ready_Buffer_Setting()
{
	if (FAILED(Engine::Reserve_ContainerSize(RESOURCE_END), E_FAIL));

	if (FAILED(Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX)))
		return;

	if (FAILED(Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX,
		129,
		129,
		1)))
		return;

	if (FAILED(Engine::Ready_Mesh(m_pDevice, RESOURCE_STAGE, L"TombStone.X", Engine::TYPE_STATIC, L"../Resources/StaticMesh/", L"TombStone.x"), E_FAIL))
		return;
	if (FAILED(Engine::Ready_Mesh(m_pDevice, RESOURCE_STAGE, L"Tree01.X", Engine::TYPE_STATIC, L"../Resources/StaticMesh/", L"Tree01.x"), E_FAIL))
		return;

	//if (FAILED(Engine::Ready_Mesh(m_pDevice,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navigation",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL)));

	if (FAILED((Engine::Ready_Buffer(m_pDevice,
		RESOURCE_STATIC,
		L"Buffer_CubeTex",
		Engine::BUFFER_CUBETEX))))
		return;

	if (FAILED(Engine::Ready_Texture(m_pDevice,
		RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../Resources/Texture/Terrain/Terrain%d.png", 2)))
		return;

	//if (FAILED(Engine::Ready_Texture(m_pDevice,
	//	RESOURCE_STAGE,
	//	L"Texture_Brush",
	//	Engine::TEX_NORMAL,
	//	L"../../../Resource/Texture/Brush/Brush%d.tga", 2)))
	//	return;
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (0 == m_pMyForm->m_iCurTab)
	{
		m_pMapTab->UpdateData(TRUE);	// MapTab
		if (0 == m_pMapTab->m_iObjToolMode)	// 생성 모드
		{
			bool retflag;
			Picking_TerrainOnStaticObject(retflag);
			Picking_MeshOnStaticObject(retflag);
			if (retflag) return;
		}
		else // 수정, 삭제 시 메쉬 클릭
		{
			if (true == m_pMapTab->m_bIsColliderMode)
			{
				bool retflag;
				Picking_MouseOnCollider(retflag);
				if (retflag) return;
			}
			else
			{
				bool retflag;
				Picking_MouseOnStaticObject(retflag);
				if (retflag) return;
			}
		}
		m_pMapTab->UpdateData(FALSE);
	}
	else if (1 == m_pMyForm->m_iCurTab)
	{
		m_pNaviTab->UpdateData(TRUE);	// NaviTab
		if (0 == m_pNaviTab->m_iCurNaviMode)	// 생성 모드
		{
			Get_TerrainInfo();
			if (nullptr != CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
			{
				_float fDistTemp = 10000000.f;
				_float fFixDist = 0.f;
				list<Engine::CGameObject*> pObjLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
				for (auto& pObject : pObjLst)
				{
					Engine::CTransform* pTransCom = dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom();

					if (CPickingMgr::GetInstance()->IsCheckStaticObjgectMesh(
						dynamic_cast<CStaticObject*>(pObject),
						*pTransCom->Get_WorldMatrix(),
						&fDistTemp,
						&m_vMeshPos))
					{
						if (fFixDist <= fDistTemp)
							fDistTemp = fFixDist;
						break;
					}
				}
			}			
			Create_NaviPointCell();
		}
		else if (1 == m_pNaviTab->m_iCurNaviMode)	// 수정 모드
		{
			bool retflag;
			Modify_NaviPointCell(retflag);
			if (retflag) return;
		}

		m_pNaviTab->UpdateData(FALSE);
	}

	CView::OnLButtonDown(nFlags, point);
}

void CToolView::Modify_NaviPointCell(bool& retflag)
{
	retflag = true;
	if (m_pNaviTab->m_pPointLstTmp.empty() || m_pNaviTab->m_pCellLstTmp.empty())
		return;

	int iTmp = 0;
	for (auto& pPoint : m_pNaviTab->m_pPointLstTmp)
	{
		if (true == CPickingMgr::GetInstance()->IsCheckSphereCollider(pPoint->m_pSphereCol))
		{
			pPoint->Set_CheckPoint(true);
			m_pNaviTab->m_pPointTmp = pPoint;
			m_pNaviTab->m_NaviList.SetCurSel(iTmp);
		}
		if (false == CPickingMgr::GetInstance()->IsCheckSphereCollider(pPoint->m_pSphereCol))
			pPoint->Set_CheckPoint(false);
		iTmp++;
	}

	for (auto& pCell : m_pNaviTab->m_pCellLstTmp)
	{
		if (nullptr == m_pNaviTab->m_pPointTmp)
			return;

		if (pCell->m_pPointA == m_pNaviTab->m_pPointTmp)
		{
			m_pNaviTab->Get_NaviPointPos(0);
			break;
		}
		else if (pCell->m_pPointB == m_pNaviTab->m_pPointTmp)
		{
			m_pNaviTab->Get_NaviPointPos(1);
			break;
		}
		else if (pCell->m_pPointC == m_pNaviTab->m_pPointTmp)
		{
			m_pNaviTab->Get_NaviPointPos(2);
			break;
		}
	}
	retflag = false;
}

void CToolView::Create_NaviPointCell()
{
	CString	strIndex = L"";

	if (!m_pNaviTab->m_pPointLstTmp.empty())	// 겹치게 찍었을 때
	{
		for (auto& pPoint : m_pNaviTab->m_pPointLstTmp)
		{
			if (true == CPickingMgr::GetInstance()->IsCheckSphereCollider(pPoint->m_pSphereCol))
			{
				m_vMeshPos.x = pPoint->m_pSphereCol->Get_WorldMat()._41;
				m_vMeshPos.y = pPoint->m_pSphereCol->Get_WorldMat()._42;
				m_vMeshPos.z = pPoint->m_pSphereCol->Get_WorldMat()._43;
				break;
			}
		}
	}

	m_pNaviTab->m_vNaviPos[m_iIdxCnt] = m_vMeshPos;
	m_pNaviTab->m_fPosX[m_iIdxCnt] = m_vMeshPos.x;
	m_pNaviTab->m_fPosY[m_iIdxCnt] = m_vMeshPos.y;
	m_pNaviTab->m_fPosZ[m_iIdxCnt] = m_vMeshPos.z;

	m_pNaviTab->m_pToolPoint[m_iIdxCnt] = CToolPoint::Create(m_pGraphicDev->GetDevice(), m_vMeshPos);
	CObjMgr::GetInstance()->AddObject(m_pNaviTab->m_pToolPoint[m_iIdxCnt], CObjMgr::OBJ_POINT);
	m_pNaviTab->m_pPointLstTmp.push_back(m_pNaviTab->m_pToolPoint[m_iIdxCnt]);

	strIndex.Format(L"%d.Point_%d", m_pNaviTab->m_iPointCnt, m_iIdxCnt);
	m_pNaviTab->m_NaviList.InsertString(m_pNaviTab->m_iPointCnt, strIndex);

	m_iIdxCnt++;
	m_pNaviTab->m_iPointCnt++;

	if (3 == m_iIdxCnt)
	{
		m_pNaviTab->m_pToolCell = CToolCell::Create(m_pGraphicDev->GetDevice(),
			m_pNaviTab->m_pToolPoint[0], m_pNaviTab->m_pToolPoint[1], m_pNaviTab->m_pToolPoint[2]);
		CObjMgr::GetInstance()->AddObject(m_pNaviTab->m_pToolCell, CObjMgr::OBJ_CELL);
		m_pNaviTab->m_pCellLstTmp.push_back(m_pNaviTab->m_pToolCell);
		m_iIdxCnt = 0;
	}
	return;
}

void CToolView::Get_TerrainInfo()
{
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN))
	{
		MessageBox(L"피킹 할 Terrain 없다 만들어라");
		return;
	}
	Engine::CGameObject* pObj = CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_TERRAIN);
	Engine::CTerrainTex* pBufferCom = dynamic_cast<CTerrain*>(pObj)->Get_BufferCom();
	Engine::CTransform* pTransCom = dynamic_cast<CTerrain*>(pObj)->Get_TransCom();

	CPickingMgr::GetInstance()->SetTerrainSize(m_pMapTab->m_iCntX, m_pMapTab->m_iCntZ);
	CPickingMgr::GetInstance()->PickingTerrain(&m_vMeshPos, pBufferCom->m_pVtxTexOrigin, pTransCom->Get_WorldMatrix());
}
void CToolView::Picking_MouseOnCollider(bool& retflag)
{
	retflag = true;
	if (m_pMapTab->m_pColliderLst.empty())
		return;

	for (auto& iter = m_pMapTab->m_pColliderLst.begin(); iter != m_pMapTab->m_pColliderLst.end();)
	{
		if (true == CPickingMgr::GetInstance()->IsCheckColiderMesh((*iter)->Get_ColliderMesh(), (*iter)->Get_WorldMat())
			|| true == CPickingMgr::GetInstance()->IsCheckSphereCollider((*iter)))
		{
			(*iter)->Set_ColType(CToolCollider::COL_TRUE);
			m_pMapTab->m_CbColliderID.SetCurSel((*iter)->Get_ColID());

			m_pMapTab->m_fPosX = (*iter)->Get_WorldMat()._41;
			m_pMapTab->m_fPosY = (*iter)->Get_WorldMat()._42;
			m_pMapTab->m_fPosZ = (*iter)->Get_WorldMat()._43;
			m_pMapTab->m_vMeshPos = { m_pMapTab->m_fPosX, m_pMapTab->m_fPosY, m_pMapTab->m_fPosZ };

			m_pMapTab->m_fRotX = (*iter)->Get_Angle().x;
			m_pMapTab->m_fRotY = (*iter)->Get_Angle().y;
			m_pMapTab->m_fRotZ = (*iter)->Get_Angle().z;
			m_pMapTab->m_vMeshRot = { m_pMapTab->m_fRotX, m_pMapTab->m_fRotY, m_pMapTab->m_fRotZ };

			if (CPickingMgr::GetInstance()->IsCheckSphereCollider((*iter)))
			{
				m_pMapTab->m_fScaleX = (*iter)->Get_WorldMat()._11;
				m_pMapTab->m_fScaleY = (*iter)->Get_WorldMat()._22;
				m_pMapTab->m_fScaleZ = (*iter)->Get_WorldMat()._33;
			}
			else
			{
				m_pMapTab->m_fScaleX = (*iter)->Get_Scale().x;
				m_pMapTab->m_fScaleY = (*iter)->Get_Scale().y;
				m_pMapTab->m_fScaleZ = (*iter)->Get_Scale().z;
			}
			m_pMapTab->m_vMeshScale = { m_pMapTab->m_fScaleX, m_pMapTab->m_fScaleY, m_pMapTab->m_fScaleZ };

			m_pMapTab->m_pPickCollider = (*iter);
			m_pMapTab->m_bIsPickingCollider = true;
		//	break;
		}
		else
		{
			(*iter)->Set_ColType(CToolCollider::COL_FALSE);
			m_pMapTab->m_bIsPickingCollider = false;
		}
		iter++;
	}
	retflag = false;
}
void CToolView::Picking_MeshOnStaticObject(bool& retflag)
{
	retflag = true;
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
		retflag = false;

	_float fDistTemp = 10000000.f;
	_float fFixDist = 0.f;

	list<Engine::CGameObject*> pObjLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
	for (auto& pObject : pObjLst)
	{
		Engine::CTransform* pTransCom = dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom();

		if (CPickingMgr::GetInstance()->IsCheckStaticObjgectMesh(
			dynamic_cast<CStaticObject*>(pObject),
			*pTransCom->Get_WorldMatrix(),
			&fDistTemp,
			&m_vMeshPos))
		{
			if (fFixDist <= fDistTemp)
			{
				fDistTemp = fFixDist;

				if (true == m_pMapTab->m_bIsColliderMode)
				{
					dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom()->Get_Info(Engine::INFO_POS, &m_vMeshPos);
					m_pMapTab->m_pPickStaticObj = dynamic_cast<CStaticObject*>(pObject);
				}

				m_pMapTab->m_vMeshPos = m_vMeshPos;
				m_pMapTab->m_fPosX = m_vMeshPos.x;
				m_pMapTab->m_fPosY = m_vMeshPos.y;
				m_pMapTab->m_fPosZ = m_vMeshPos.z;
				break;
			}
		}
	}
	retflag = false;
}
void CToolView::Picking_MouseOnStaticObject(bool& retflag)
{
	retflag = true;
	if (nullptr == CObjMgr::GetInstance()->GetGameObject(CObjMgr::OBJ_OBJECT))
	{
		MessageBox(L"수정할 Object 없다 만들어라");
		return;
	}

	_float fDistTemp = 10000000.f;
	_float fFixDist = 0.f;

	list<Engine::CGameObject*> pObjLst = CObjMgr::GetInstance()->GetGameObjectLst(CObjMgr::OBJ_OBJECT);
	for (auto& pObject : pObjLst)
	{
		Engine::CTransform* pTransCom = dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom();

		if (CPickingMgr::GetInstance()->IsCheckStaticObjgectMesh(
			dynamic_cast<CStaticObject*>(pObject),
			*pTransCom->Get_WorldMatrix(),
			&fDistTemp,
			&m_vMeshPos))
		{
			if (fFixDist <= fDistTemp)
			{
				fDistTemp = fFixDist;

				dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom()->Get_Info(Engine::INFO_POS, &m_vMeshPos);
				m_vMeshScale = dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom()->m_vScale;
				m_vMeshRot = dynamic_cast<CStaticObject*>(pObject)->Get_StaticTranscom()->m_vAngle;

				m_pMapTab->m_vMeshPos = m_vMeshPos;
				m_pMapTab->m_fPosX = m_vMeshPos.x;
				m_pMapTab->m_fPosY = m_vMeshPos.y;
				m_pMapTab->m_fPosZ = m_vMeshPos.z;

				m_pMapTab->m_vMeshScale = m_vMeshScale;
				m_pMapTab->m_fScaleX = m_vMeshScale.x;
				m_pMapTab->m_fScaleY = m_vMeshScale.y;
				m_pMapTab->m_fScaleZ = m_vMeshScale.z;

				m_pMapTab->m_vMeshRot = m_vMeshRot;
				m_pMapTab->m_fRotX = m_vMeshRot.x;
				m_pMapTab->m_fRotY = m_vMeshRot.y;
				m_pMapTab->m_fRotZ = m_vMeshRot.z;

				m_pMapTab->m_pPickStaticObj = dynamic_cast<CStaticObject*>(pObject);
				m_pMapTab->m_bIsPickingStaticObj = true;
				break;
			}
		}
		else
			m_pMapTab->m_bIsPickingStaticObj = false;
	}
	retflag = false;
}
void CToolView::Picking_TerrainOnStaticObject(bool& retflag)
{
	retflag = true;
	Get_TerrainInfo();

	m_pMapTab->m_vMeshPos = m_vMeshPos;
	m_pMapTab->m_fPosX = m_vMeshPos.x;
	m_pMapTab->m_fPosY = m_vMeshPos.y;
	m_pMapTab->m_fPosZ = m_vMeshPos.z;

	retflag = false;
}
