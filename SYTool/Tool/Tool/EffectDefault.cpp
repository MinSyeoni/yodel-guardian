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

	if (m_bIsPlayPattern)
	{
		m_fSpriteTime += fTimeDelta * m_tEffectData.fSpriteSpeed;
		UpdateAnimation();

	}
	else
	{
		m_tEffectData.vOriPos = m_pTransCom->m_vInfo[INFO_POS];
		m_tEffectData.vOriScale = m_pTransCom->m_vScale;
	}



	

	m_pSphereCol->Update_Collider(&m_pTransCom->m_matWorld);
	m_pSphereCol->Set_ColType(CToolCollider::COL_TRUE);

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

	_uint iPass = 0;
	if (m_eState == ALPHATEST)
		iPass = 1;
	else if (m_eState == ALPHABLEND)
		iPass = 2;


	pEffect->BeginPass(iPass);

	m_pBufferCom->Render();
	

	pEffect->EndPass();
	pEffect->End();
	m_pSphereCol->Set_WorldMat(m_pTransCom->m_matWorld);
	if(m_bIsCheck==true)
	m_pSphereCol->Render_Collider();



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
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	pEffect->SetFloat("g_fX", m_tEffectData.iUvWidth);
	pEffect->SetFloat("g_fY", m_tEffectData.iUvHeight);
	pEffect->SetFloat("g_fChapterX", m_fChapterX);
	pEffect->SetFloat("g_fChapterY", m_fChapterY);

	m_pTexCom->Set_Texture(pEffect, "g_BaseTexture", m_iDrawId);

	Engine::Safe_Release(pEffect);

	return S_OK;

}

HRESULT CToolEffect::SetTexture(int iDrawId, TEXTURE_STATE eState)
{

	m_iDrawId = iDrawId;


	if (m_eState != eState)
	{
		m_eState = eState;
		
		Engine::Safe_Release(m_pTexCom);
		m_mapComponent[ID_STATIC].erase(L"Com_Tex");

		Engine::CComponent* pComponent = nullptr;

		if (m_eState == ALPHABLEND)
		{

			pComponent = m_pTexCom = static_cast <Engine::CTexture*>(Engine::Clone_Resources(RESOURCE_STAGE, L"Texture_AlphaBlend"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Tex", pComponent);

		}
		else if (m_eState == ALPHATEST)
		{
			pComponent = m_pTexCom = static_cast<Engine::CTexture*>(Engine::Clone_Resources(RESOURCE_STAGE, L"Texture_AlphaTest"));
			NULL_CHECK_RETURN(pComponent, E_FAIL);
			m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Tex", pComponent);

		}
       

	}



}

void CToolEffect::UvAnimation()
{

	if (m_tEffectData.iUvWidth < m_fSpriteTime)
	{
		m_fSpriteTime = 0.f;
		m_iSpriteRow++;

		if (m_iSpriteRow >= m_tEffectData.iUvHeight)
		{
			m_iSpriteRow = 0;
		}


	}
	int iRow = m_fSpriteTime;
	int iCol = m_iSpriteRow;
    
	m_fChapterX = (float)iRow/m_tEffectData.iUvWidth;
	m_fChapterY = (float)iCol /m_tEffectData.iUvHeight;



}

void CToolEffect::SetCheck(_bool bIsCheck)
{

	m_bIsPlayPattern = bIsCheck;

	if (!m_bIsPlayPattern)
	{
		m_pTransCom->m_vScale = m_tEffectData.vOriScale;
		m_pTransCom->m_vAngle = m_tEffectData.vOriRot;

		m_fAlpha = 1.f;
	}
                                 
}

HRESULT CToolEffect::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	// TransCom;
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// CalculatorºÎ
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



	m_pSphereCol = Engine::CSphereCollider::Create(m_pGraphicDev, 1.f, CToolCollider::COLID_COLLIDER);

	m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);

	return S_OK;
}

void CToolEffect::Free(void)
{
	Engine::Safe_Release(m_pTransCom);
	Engine::Safe_Release(m_pCalculatorCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTexCom);
	Engine::Safe_Release(m_pSphereCol);
	Engine::CGameObject::Free();


}

CToolEffect* CToolEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolEffect* pInstance = new CToolEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CToolEffect::UpdateAnimation()
{

	UpdateScaleAni();
	UpdateRotAni();
	if(m_tEffectData.bIsFadeIn)
	UpdateFadeIn();

	if(m_tEffectData.bIsFadeOut)
	UpdateFadeOut();
	

	
	UvAnimation();

}

void CToolEffect::UpdateScaleAni()
{


	if (m_fAccTime < m_tEffectData.fStartScale)
	{
		return;
	}

	float fScalePattenTime = m_tEffectData.fEndScale - m_tEffectData.fStartScale;

	float fAccTime = m_fAccTime - m_tEffectData.fStartScale;
	
	m_pTransCom->m_vScale = ((m_tEffectData.vScalePat-m_tEffectData.vOriScale) * fAccTime / fScalePattenTime)+(m_tEffectData.vOriScale);


	if (fAccTime / fScalePattenTime >= 1.f)
		m_pTransCom->m_vScale = m_tEffectData.vScalePat;





}

void CToolEffect::UpdateRotAni()
{


	if (m_fAccTime < m_tEffectData.fStartRot)
		return;

	float fScalePattenTime = m_tEffectData.fEndRot - m_tEffectData.fStartRot;

	float fAccTime = m_fAccTime - m_tEffectData.fStartRot;

	m_pTransCom->m_vAngle = ((m_tEffectData.vRotPat - m_tEffectData.vOriRot) * fAccTime / fScalePattenTime) + (m_tEffectData.vOriRot);


	if (fAccTime / fScalePattenTime >= 1.f)
		m_pTransCom->m_vAngle = m_tEffectData.vRotPat;






}

void CToolEffect::UpdateFadeOut()
{
	if (m_fAccTime < m_tEffectData.fFadeOutStartTime)
		return;

	float fFadeInPattenTime = m_tEffectData.fFadeOutEndTime - m_tEffectData.fFadeOutStartTime;

	float fAccTime = m_fAccTime - m_tEffectData.fFadeOutStartTime;

	m_fAlpha =1.f-(fAccTime / fFadeInPattenTime);

	if (m_fAlpha < 0.f)
		m_fAlpha = 0.f;

}

void CToolEffect::UpdateFadeIn()
{

	if (m_fAccTime < m_tEffectData.fFadeInStartTime)
		return;

	float fFadeInPattenTime = m_tEffectData.fFadeInEndTime - m_tEffectData.fFadeInStartTime;

	float fAccTime = m_fAccTime - m_tEffectData.fFadeInStartTime;

	m_fAlpha = (fAccTime / fFadeInPattenTime);


}
