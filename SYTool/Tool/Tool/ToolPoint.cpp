#include "pch.h"
#include "ToolPoint.h"

CToolPoint::CToolPoint(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CToolPoint::~CToolPoint(void)
{

}

HRESULT CToolPoint::Ready_Object(_vec3 Pos)
{
	FAILED_CHECK_RETURN(Add_Component(Pos), E_FAIL);

	return S_OK;
}

_int CToolPoint::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	D3DXMatrixScaling(&m_matColScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&m_matColTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matColWorld = m_matColScale * m_matColTrans;

	m_pSphereCol->Set_PareOriWorld(m_matColWorld);
	m_pSphereCol->Set_WorldMat(m_matColWorld);

	if (true == m_bIsCheck)
		m_pSphereCol->Set_ColType(CToolCollider::COL_TRUE);
	else if (false == m_bIsCheck)
		m_pSphereCol->Set_ColType(CToolCollider::COL_FALSE);

	m_pSphereCol->Update_Collider(&m_pSphereCol->Get_PareOriWorld());

	return S_OK;
}

void CToolPoint::Render_Object(void)
{
	m_pSphereCol->Render_Collider();
}

HRESULT CToolPoint::Add_Component(_vec3 Pos)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	m_vPos = m_pTransCom->m_vInfo[Engine::INFO_POS] = Pos;
	m_vScale = m_pTransCom->m_vScale = _vec3{ 1.f, 1.f, 1.f };

	m_pSphereCol = Engine::CSphereCollider::Create(m_pGraphicDev, m_pTransCom->m_vScale.x, CToolCollider::COLID_NAVI);
}

void CToolPoint::Free(void)
{
	Engine::Safe_Release(m_pSphereCol);
	Engine::Safe_Release(m_pTransCom);
	Engine::CGameObject::Free();
}

CToolPoint* CToolPoint::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 Pos)
{
	CToolPoint* pInstance = new CToolPoint(pGraphicDev);
	if (FAILED(pInstance->Ready_Object(Pos)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
