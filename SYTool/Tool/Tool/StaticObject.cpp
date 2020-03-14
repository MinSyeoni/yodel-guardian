#include "pch.h"
#include "StaticObject.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CStaticObject::~CStaticObject(void)
{

}

HRESULT CStaticObject::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bisDead = 0;
	
	m_pTransCom->Set_Pos(&m_vMeshPos);
	m_pTransCom->Set_Scale(m_vMeshScale.x, m_vMeshScale.y, m_vMeshScale.z);
	m_pTransCom->Rotation(Engine::ROT_X, m_vMeshRot.x);
	m_pTransCom->Rotation(Engine::ROT_Y, m_vMeshRot.y);
	m_pTransCom->Rotation(Engine::ROT_Z, m_vMeshRot.z);


	lstrcpy(m_szMeshTag, m_szFileTag);

	return S_OK;
}

_int CStaticObject::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return m_bisDead;
}

void CStaticObject::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_eState == STATIC_WIRE)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	FAILED_CHECK_RETURN(Set_ConstantTable(pEffect), );

	_uint	iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh();
	//m_pColliderCom->Render_Collider(Engine::COL_FALSE, m_pTransCom->Get_WorldMatrix());

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CStaticObject::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix2(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CStaticObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// TransCom;
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// staticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resources(RESOURCE_STAGE, m_szFileTag));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Collider 
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
	//														m_pMeshCom->Get_VtxPos(), 
	//														m_pMeshCom->Get_NumVtx(), 
	//														m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// ShaderCom
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>
		(Engine::Clone_Prototype(L"Proto_ShaderMesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}

void CStaticObject::Free(void)
{
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pCalculatorCom);
	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::CGameObject::Free();
}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticObject* pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPos, _vec3 pRot, _vec3 pScale, _tchar* pTag)
{
	CStaticObject* pInstance = new CStaticObject(pGraphicDev);

	pInstance->m_szFileTag = pTag;
	pInstance->m_vMeshPos = pPos;
	pInstance->m_vMeshRot = pRot;
	pInstance->m_vMeshScale = pScale;

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
