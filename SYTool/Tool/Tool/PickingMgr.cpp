#include "pch.h"
#include "PickingMgr.h"
#include "ToolCollider.h"

IMPLEMENT_SINGLETON(CPickingMgr)

CPickingMgr::CPickingMgr()
{
}


CPickingMgr::~CPickingMgr()
{
	//Release();
}

void CPickingMgr::PickTerrainCubePos(D3DXVECTOR3 * pOut, const VTXTEX * pTerrainVtx, float fDeap)
{
	Translation_ViewSpace();
	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	const VTXTEX*		pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for (_uint y = 0; y < m_dwRow; ++y)
	{
		for (_uint x = 0; x < m_dwCol; ++x)
		{
			int	iIndex = y * (m_dwCol + 1) + x;

			// 오른쪽 위
			if (D3DXIntersectTri(&pVertex[iIndex + m_dwCol + 2].vPos,
				&pVertex[iIndex + 1].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				*pOut = { (pVertex[iIndex].vPos.x + pVertex[iIndex + 1].vPos.x) / 2.f
					,(pVertex[iIndex + m_dwCol + 2].vPos.y + pVertex[iIndex + 1].vPos.y) / 2.f ,fDeap };

				return;
			}

			// 왼쪽 아래
			if (D3DXIntersectTri(&pVertex[iIndex].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&pVertex[iIndex + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				*pOut = { (pVertex[iIndex].vPos.x + pVertex[iIndex + 1].vPos.x) / 2.f
					, (pVertex[iIndex + m_dwCol + 2].vPos.y + pVertex[iIndex + 1].vPos.y) / 2.f, fDeap };

				return;
			}
		}
	}
}

void CPickingMgr::PickTerrainIndex(DWORD * pOutIndex, const VTXTEX * pTerrainVtx)
{
	Translation_ViewSpace();
	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	const VTXTEX*		pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for (_uint y = 0; y < m_dwRow; ++y)
	{
		for (_uint x = 0; x < m_dwCol; ++x)
		{
			int		iIndex = y * (m_dwCol + 1) + x;

			// 오른쪽 위
			if (D3DXIntersectTri(&pVertex[iIndex + m_dwCol + 2].vPos,
				&pVertex[iIndex + 1].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				*pOutIndex = iIndex;
				return;
			}

			//왼쪽 위
			if (D3DXIntersectTri(&pVertex[iIndex].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&pVertex[iIndex + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				*pOutIndex = iIndex;

				return;
			}
		}
	}
}

void CPickingMgr::SetTerrainSize(DWORD dwCol, DWORD dwRow)
{
	m_dwCol = dwCol;
	m_dwRow = dwRow;
}

void CPickingMgr::PickingTerrain(D3DXVECTOR3 * pOut, const VTXTEX * pVertex, const D3DXMATRIX * pmatWorld)
{
	Translation_ViewSpace();
	Translation_Local(pmatWorld);

	float	fU, fV, fDist;
	_ulong	dwVtxIdx[3];

	for (_ulong i = 0; i < m_dwRow - 1; ++i)
	{
		for (_ulong j = 0; j < m_dwCol - 1; ++j)
		{
			_ulong	dwIndex = i * m_dwRow + j;

			dwVtxIdx[0] = dwIndex + m_dwRow;
			dwVtxIdx[1] = dwIndex + m_dwRow + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pVertex[dwVtxIdx[1]].vPos,
				&pVertex[dwVtxIdx[2]].vPos,
				&pVertex[dwVtxIdx[0]].vPos,
				&m_tRay.vOri, &m_tRay.vDir,
				&fU, &fV, &fDist))
			{
				*pOut = _vec3(m_tRay.vOri + (m_tRay.vDir * fDist));
				return;
			}

			dwVtxIdx[0] = dwIndex + m_dwRow;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pVertex[dwVtxIdx[2]].vPos,
				&pVertex[dwVtxIdx[0]].vPos,
				&pVertex[dwVtxIdx[1]].vPos,
				&m_tRay.vOri, &m_tRay.vDir,
				&fU, &fV, &fDist))
			{
				*pOut = _vec3(m_tRay.vOri + (m_tRay.vDir * fDist));
				return;
			}
		}
	}

	*pOut = { 0.f,0.f,0.f };
	return;
}

void CPickingMgr::PickTerrainTextPos(D3DXVECTOR3 * pOut, const VTXTEX * pTerrainVtx, float fDeap)
{
	Translation_ViewSpace();
	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	const VTXTEX*		pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for (_uint y = 0; y < m_dwRow; ++y)
	{
		for (_uint x = 0; x < m_dwCol; ++x)
		{
			int	iIndex = y * (m_dwCol + 1) + x;

			// 오른쪽 위
			if (D3DXIntersectTri(&pVertex[iIndex + m_dwCol + 2].vPos,
				&pVertex[iIndex + 1].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				D3DXVECTOR3 MousePos = (pVertex[iIndex + m_dwCol + 2].vPos +
					(pVertex[iIndex + m_dwCol + 1].vPos - pVertex[iIndex + m_dwCol + 2].vPos) * fV +
					(pVertex[iIndex + 1].vPos - pVertex[iIndex + m_dwCol + 2].vPos)	*fU);

				*pOut = { MousePos.x , MousePos.y
					, fDeap };

				return;
			}

			// 왼쪽 아래
			if (D3DXIntersectTri(&pVertex[iIndex].vPos,
				&pVertex[iIndex + m_dwCol + 1].vPos,
				&pVertex[iIndex + 1].vPos,
				&m_tRay.vOri, &m_tRay.vDir, &fU, &fV, &fDist))
			{
				D3DXVECTOR3 MousePos = (pVertex[iIndex].vPos + (pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fV +
					(pVertex[iIndex + m_dwCol + 1].vPos - pVertex[iIndex].vPos) * fU);

				*pOut = { MousePos.x , MousePos.y
					, fDeap };

				return;
			}
		}
	}
}

bool CPickingMgr::IsCheckSphereCollider(Engine::CToolCollider* pCollider)
{
	Translation_ViewSpace();

	D3DXMATRIX	matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	matIdentity._41 = -(pCollider->Get_WorldMat()._41);
	matIdentity._42 = -(pCollider->Get_WorldMat()._42);
	matIdentity._43 = -(pCollider->Get_WorldMat()._43);

	D3DXVec3TransformCoord(&m_tRay.vOri, &m_tRay.vOri, &matIdentity);
	D3DXVec3TransformNormal(&m_tRay.vDir, &m_tRay.vDir, &matIdentity);

	float fV = D3DXVec3Dot(&m_tRay.vDir, &m_tRay.vDir);
	float fqV = D3DXVec3Dot(&m_tRay.vOri, &m_tRay.vDir);
	float fqQ = D3DXVec3Dot(&m_tRay.vOri, &m_tRay.vOri);
	float fR = (pCollider->Get_Radius() * pCollider->Get_Radius()) * 0.01f;

	return fqV * fqV - fV * (fqQ - fR) >= 0;
}

bool CPickingMgr::IsCheckColiderMesh(const LPD3DXMESH* pMesh, D3DXMATRIX pMeshWorld)
{
	Translation_ViewSpace();
	Translation_Local(&pMeshWorld);

	//D3DXMATRIX	matIdentity;
	//D3DXMatrixIdentity(&matIdentity);
	//Translation_Local(&matIdentity);

	_ulong   dwVtxIdx[3] = {};
	_float   fU, fV, fDist;

	BOOL      bCheck;

	D3DXIntersect(*pMesh, &m_tRay.vOri, &m_tRay.vDir, &bCheck, nullptr, &fU, &fV, &fDist, nullptr, nullptr);
	
	if (false == bCheck)
		return false;

	_vec3   Pos = m_tRay.vOri + (m_tRay.vDir * fDist);
	D3DXVec3TransformCoord(&Pos, &Pos, &pMeshWorld);

	return true;
}

bool CPickingMgr::IsCheckStaticObjgectMesh(CStaticObject* pMesh, D3DXMATRIX pMeshWorld, _float* pDist, _vec3* pPos)
{
	POINT      ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3      vMousePos;

	D3DVIEWPORT9      ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	auto pGraphicDev = Engine::CGraphicDev::GetInstance()->GetDevice();
	pGraphicDev->GetViewport(&ViewPort);

#ifdef CLIENT      

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

#else                                    
	RECT rcView = {};
	GetWindowRect(g_hWnd, &rcView);

	vMousePos.x = ptMouse.x / ((rcView.right - rcView.left) * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -((rcView.bottom - rcView.top) * 0.5f) + 1.f;
	vMousePos.z = 0.f;

#endif

	_matrix         matProj, matView;

	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	//뷰->월드
	_vec3      vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	//월드->로컬
	_matrix      matWorld;

	matWorld = *pMesh->Get_StaticTranscom()->Get_WorldMatrix();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong   dwVtxIdx[3] = {};
	_float   fU = 0.f, fV = 0.f, fDist = 0.f;

	BOOL      bCheck;

	D3DXIntersect(pMesh->Get_Mesh(), &vRayPos, &vRayDir, &bCheck, nullptr, &fU, &fV, &fDist, nullptr, nullptr);

	if (false == bCheck)
		return false;

	_vec3   Pos = vRayPos + (vRayDir * fDist);
	D3DXVec3TransformCoord(&Pos, &Pos, pMesh->Get_StaticTranscom()->Get_WorldMatrix());

	*pPos = Pos;
	*pDist = fDist;

	return true;
}

_vec3 CPickingMgr::IsCheckCollMesh(CStaticObject* pMesh, D3DXMATRIX pMeshWorld)
{
	POINT      ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3      vMousePos;

	D3DVIEWPORT9      ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	//Engine::CGraphicDev* pGraphicDev = nullptr;
	Engine::CGraphicDev::GetInstance()->GetDevice()->GetViewport(&ViewPort);

#ifdef CLIENT      

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

#else                                    
	RECT rcView = {};
	GetWindowRect(g_hWnd, &rcView);

	vMousePos.x = ptMouse.x / ((rcView.right - rcView.left) * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -((rcView.bottom - rcView.top) * 0.5f) + 1.f;
	vMousePos.z = 0.f;

#endif

	_matrix         matProj, matView;

	//
	Engine::CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	//뷰->월드
	_vec3      vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	Engine::CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	//월드->로컬
	_matrix      matWorld;

	matWorld = *pMesh->Get_StaticTranscom()->Get_WorldMatrix();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong   dwVtxIdx[3] = {};
	_float   fU, fV, fDist;

	BOOL      bCheck;

	D3DXIntersect(pMesh->Get_Mesh(), &vRayPos, &vRayDir, &bCheck, nullptr, &fU, &fV, &fDist, nullptr, nullptr);

	if (bCheck)
	{
		_vec3   Pos = vRayPos + (vRayDir * fDist);
		D3DXVec3TransformCoord(&Pos, &Pos, pMesh->Get_StaticTranscom()->Get_WorldMatrix());

		return Pos;
	}

	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CPickingMgr::Initialize(void)
{
	//m_pCameraObserver = CCameraObserver::Create();
	//m_pInfoSubject->Subscribe(m_pCameraObserver);

	return S_OK;
}

void CPickingMgr::Translation_ViewSpace(void)
{
	D3DXVECTOR3	vMouse = CCursorMgr::GetCursPos(g_hWnd);
	_matrix	matProj;
	
	Engine::CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	//Engine::CGameObject* pDynamicCamera = Engine::Get_GameObject(SCENE_STAGE, LAYER_CAMERA);
	//_matrix	matProj = dynamic_cast<CCamera_Dynamic*>(pDynamicCamera)->Get_Transform(D3DTS_PROJECTION);


	// 뷰 포트 -> 투영 -> 뷰 스페이스
	D3DXVECTOR3	vTemp;

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitWnd.GetPane(0, 1));

	RECT rc;
	GetClientRect(pToolView->m_hWnd, &rc);

	vTemp.x = (vMouse.x / (rc.right >> 1) - 1.f);
	vTemp.y = (-vMouse.y / (rc.bottom >> 1) + 1.f);
	vTemp.z = 0.f;

	D3DXMatrixInverse(&matProj, 0, &matProj);

	D3DXVec3TransformCoord(&vTemp, &vTemp, &matProj);

	m_tRay.vOri = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tRay.vDir = vTemp - m_tRay.vOri;

}

void CPickingMgr::Translation_Local(const D3DXMATRIX * pWorld)
{
	//D3DXMATRIX	matProj = CToolCamera::GetInstance()->Get_Transform(D3DTS_PROJECTION);
	//D3DXMATRIX	matView = CToolCamera::GetInstance()->Get_Transform(D3DTS_VIEW);
	_matrix matProj, matView;
	Engine::CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	Engine::CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&m_tRay.vOri, &m_tRay.vOri, &matView);
	D3DXVec3TransformNormal(&m_tRay.vDir, &m_tRay.vDir, &matView);

	D3DXMATRIX matWorld;
	D3DXMatrixInverse(&matWorld, 0, pWorld);

	D3DXVec3TransformCoord(&m_tRay.vOri, &m_tRay.vOri, &matWorld);
	D3DXVec3TransformNormal(&m_tRay.vDir, &m_tRay.vDir, &matWorld);
}

void CPickingMgr::Free()
{
}
