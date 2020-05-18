#include "pch.h"
#include "EffectDefault.h"

CToolEffect::CToolEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CToolEffect::~CToolEffect(void)
{
}

HRESULT CToolEffect::Ready_Object()
{

	m_bisDead = 0;
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CToolEffect::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return m_bisDead;
}

void CToolEffect::Render_Object(void)
{


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	FAILED_CHECK_RETURN(Set_ConstantTable(pEffect), );

	_uint	iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CToolEffect::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix2(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	m_pTexCom->Set_Texture(pEffect, "g_BaseTexture", 0);

	Engine::Safe_Release(pEffect);

	return S_OK;

}

HRESULT CToolEffect::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	// TransCom;
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// staticMesh
	pComponent = m_pTexCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resources(RESOURCE_STAGE, L"Texture_Default"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Tex", pComponent);


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resources(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// ShaderCom
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>
		(Engine::Clone_Prototype(L"Proto_ShaderSample"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

void CToolEffect::Free(void)
{
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pCalculatorCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTexCom);
	Engine::CGameObject::Free();


}

CToolEffect* CToolEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolEffect* pInstance = new CToolEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);


	return pInstance;
}
