#include "pch.h"
#include "ToolCell.h"

CToolCell::CToolCell(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CToolCell::~CToolCell(void)
{
}

HRESULT CToolCell::Ready_Object()
{
	Engine::CComponent*		pComponent = nullptr;

	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);
	m_iCellindex=CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_CELL].size();	

	pComponent = m_pCalculCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	return S_OK;
}

_int CToolCell::Update_Object(const _float& fTimeDelta)
{
	m_pPointA->Update_Object(fTimeDelta);
	m_pPointB->Update_Object(fTimeDelta);
	m_pPointC->Update_Object(fTimeDelta);

	return m_bisDead;
}

void CToolCell::Render_Object(void)
{
	m_pPointA->Render_Object();
	m_pPointB-> Render_Object();
	m_pPointC->Render_Object();

	if(true == m_bIsShow)
		Render_FontOnCell();
}

void CToolCell::Render_FontOnCell()
{
	m_vPtPos[0] = m_pPointA->m_pTransCom->m_vInfo[Engine::INFO_POS];
	m_vPtPos[1] = m_pPointB->m_pTransCom->m_vInfo[Engine::INFO_POS];
	m_vPtPos[2] = m_pPointC->m_pTransCom->m_vInfo[Engine::INFO_POS];
	m_vPtPos[3] = m_pPointA->m_pTransCom->m_vInfo[Engine::INFO_POS];
	m_vFontPos = (m_vPtPos[0] + m_vPtPos[1] + m_vPtPos[2]) / 3;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&m_vPtPos[i], &m_vPtPos[i], &m_matView);
		if (m_vPtPos[i].z <= 0.1f)
			m_vPtPos[i].z = 0.1f;
		D3DXVec3TransformCoord(&m_vPtPos[i], &m_vPtPos[i], &m_matProj);
	}

	m_pD3DXLine->SetWidth(3.f);
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix matTemp;

	if(false == m_bIsCheckCell)
		m_pD3DXLine->DrawTransform(m_vPtPos, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	else if(true == m_bIsCheckCell)
		m_pD3DXLine->DrawTransform(m_vPtPos, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pD3DXLine->End();

	m_pGraphicDev->GetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matView);

	m_matWorld._41 = m_vFontPos.x;
	m_matWorld._42 = m_vFontPos.y;
	m_matWorld._43 = m_vFontPos.z;

	m_matViewPort = m_matView * m_matProj;

	D3DXVec3TransformCoord(&m_vFontPos, &m_vFontPos, &m_matViewPort);

	m_pGraphicDev->GetViewport(&m_ViewPort);

	m_vViewPortPos.x = (m_vFontPos.x + 1.f) * (m_ViewPort.Width * 0.5);
	m_vViewPortPos.y = -(m_vFontPos.y - 1.f) * (m_ViewPort.Height * 0.5);


	CString strBuffer = L"";
	strBuffer.Format(L"%d", m_iCellindex);

	if (false == m_bIsCheckCell)
		Engine::Render_Font(L"Font_Default", strBuffer, &_vec2(m_vViewPortPos.x, m_vViewPortPos.y), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));	
	else if (true == m_bIsCheckCell)
		Engine::Render_Font(L"Font_Default", strBuffer, &_vec2(m_vViewPortPos.x, m_vViewPortPos.y), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

HRESULT CToolCell::Add_Component(_vec3 Pos)
{
	return S_OK;
}

void CToolCell::Free(void)
{
	Engine::Safe_Release(m_pPointA);
	Engine::Safe_Release(m_pPointB);
	Engine::Safe_Release(m_pPointC);
	Engine::Safe_Release(m_pD3DXLine);
	Engine::Safe_Release(m_pCalculCom);
	Engine::CGameObject::Free();
}

CToolCell * CToolCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, CToolPoint* PosA, CToolPoint* PosB, CToolPoint* PosC)
{
	CToolCell* pInstance = new CToolCell(pGraphicDev);
	pInstance->m_pPointA = PosA;
	pInstance->m_pPointB = PosB;
	pInstance->m_pPointC = PosC;

	if (FAILED(pInstance->Ready_Object()))
		return nullptr;

	return pInstance;
}


