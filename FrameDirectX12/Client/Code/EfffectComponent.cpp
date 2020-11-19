#include "stdafx.h"
#include "EfffectComponent.h"

CEffectComponent::CEffectComponent(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CGameObject(pGraphicDevice,pCommandList)
{
}

CEffectComponent::CEffectComponent(const CEffectComponent& rhs)
	:CGameObject(rhs)
{
}

CEffectComponent::~CEffectComponent()
{
}

HRESULT CEffectComponent::Ready_GameObjectPrototype(EFFECTDATA tData, _vec3 vStartPos)
{
	m_tEffectData = tData;

	CGameObject::Ready_GameObject();

	if (m_tEffectData.eType == BUFFER_TYPE::TEXTURE)
	{
		m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);
		if (m_tEffectData.eState == TEXTURE_STATE::ALPHABLEND)
		{
			m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Effect_AlphaBlend", COMPONENTID::ID_STATIC));
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);


			m_pShaderCom = static_cast<Engine::CShader_Effect*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_EffectAlphaBlend", COMPONENTID::ID_STATIC));
			NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
			m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
	    }
	    



	}
	if(!tData.bIsUvInfinite)
	m_fRandZ = rand() % 360;

	m_pTransCom->m_vPos =  vStartPos + m_tEffectData.vOriPos;
	m_pTransCom->m_vAngle = m_tEffectData.vOriRot;
	m_pTransCom->m_vScale = m_tEffectData.vOriScale;

	m_tEffectData.fSpriteSpeed = 150.f;
	return S_OK;
}

HRESULT CEffectComponent::Ready_GameObject()
{
	return S_OK;
}

HRESULT CEffectComponent::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	return S_OK;
}

_int CEffectComponent::Update_GameObject(const _float& fTimeDelta)
{
	m_pTransCom->m_vAngle.z = rand() % 10;

	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);
	CGameObject::Update_GameObject(fTimeDelta);

	m_fAccTime += fTimeDelta;
	if (m_tEffectData.bIsUvSprite)
		m_fSpriteAnimation += fTimeDelta * m_tEffectData.fSpriteSpeed;
	UpdateAnimation();


	if (m_tEffectData.eType == BUFFER_TYPE::TEXTURE)
		BillBoard();




	if (m_fAccTime - m_tEffectData.fStartTime > m_tEffectData.fEndTime - m_tEffectData.fStartTime)
		m_bIsDead = true;

	return m_bIsDead;
}

_int CEffectComponent::LateUpdate_GameObject(const _float& fTimeDelta)
{

	

	NULL_CHECK_RETURN(m_pRenderer, -1);

	if(m_tEffectData.fStartTime<m_fAccTime)
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_ALPHA, this), -1);

	

	return S_OK;
}

void CEffectComponent::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();
	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader(m_tEffectData.iDrawId);
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

}

void CEffectComponent::UpdateAnimation()
{
	ScaleAnimation();
	RotationAnimation();

	if (m_tEffectData.bIsFadeIn)
		FadeInAnimation();

	if (m_tEffectData.bIsFadeOut)
		FadeOutAnimation();


	if(m_tEffectData.bIsUvSprite)
	SpriteAnimation();

}

void CEffectComponent::BillBoard()
{
	_matrix matBill, matView;

	 matBill = CGraphicDevice::Get_Instance()->GetViewMatrix();
	 matView = matBill;
	 ZeroMemory(&matBill.m[3][0], sizeof(_vec3));


	 matBill._11 = matView._11;
	 matBill._13 = matView._13;
	 matBill._31 = matView._31;
	 matBill._33 = matView._33;

	matBill= XMMatrixInverse(nullptr, matBill);

	

	_matrix matTemp = XMMatrixRotationZ(m_fRandZ);


	m_pTransCom->m_matWorld = matTemp* matBill * m_pTransCom->m_matWorld;
	_vec3 vPos;
	memcpy(&vPos, &m_pTransCom->m_matWorld._41, sizeof(_vec3));
	Compute_ViewZ(&vPos);
}

void CEffectComponent::SpriteAnimation()
{
	bool bIsStop = false;
	if (m_tEffectData.iUvWidth < m_fSpriteAnimation)
	{

		m_fSpriteAnimation = 0.f;
		m_iSpriteRow++;

		if (m_iSpriteRow >= m_tEffectData.iUvHeight)
		{
			m_iSpriteRow = 0;
            if(!m_tEffectData.bIsUvInfinite)
			m_bIsDead = true;
			
		}
	}
	int iRow = m_fSpriteAnimation;
	int iCol = m_iSpriteRow;

	m_fChapterX = (float)iRow / m_tEffectData.iUvWidth;
	m_fChapterY = (float)iCol / m_tEffectData.iUvHeight;


	if (bIsStop == true && m_tEffectData.bIsUvInfinite == false)
	{

		m_fChapterX = float(m_tEffectData.iUvWidth - 1) / m_tEffectData.iUvWidth;
		m_fChapterY = float(m_tEffectData.iUvHeight - 1) / m_tEffectData.iUvHeight;

	}




}

void CEffectComponent::RotationAnimation()
{
	if (m_fAccTime < m_tEffectData.fStartRot)
		return;

	float fScalePattenTime = m_tEffectData.fEndRot - m_tEffectData.fStartRot;

	float fAccTime = m_fAccTime - m_tEffectData.fStartRot;

	m_pTransCom->m_vAngle = ((m_tEffectData.vRotPat - m_tEffectData.vOriRot) * (fAccTime / fScalePattenTime)) + (m_tEffectData.vOriRot);


	if (fAccTime / fScalePattenTime >= 1.f)
		m_pTransCom->m_vAngle = m_tEffectData.vRotPat;

}

void CEffectComponent::ScaleAnimation()
{
	if (m_fAccTime < m_tEffectData.fStartScale)
	{
		return;
	}

	float fScalePattenTime = m_tEffectData.fEndScale - m_tEffectData.fStartScale;

	float fAccTime = m_fAccTime - m_tEffectData.fStartScale;

	m_pTransCom->m_vScale = ((m_tEffectData.vScalePat - m_tEffectData.vOriScale) * (fAccTime / fScalePattenTime)) + (m_tEffectData.vOriScale);


	if (fAccTime / fScalePattenTime >= 1.f)
		m_pTransCom->m_vScale = m_tEffectData.vScalePat;
}

void CEffectComponent::FadeInAnimation()
{

	if (m_fAccTime < m_tEffectData.fFadeInStartTime)
		return;

	float fFadeInPattenTime = m_tEffectData.fFadeInEndTime - m_tEffectData.fFadeInStartTime;

	float fAccTime = m_fAccTime - m_tEffectData.fFadeInStartTime;

	m_fAlpha = (fAccTime / fFadeInPattenTime);

	if (m_fAlpha > 1.f)
		m_fAlpha = 1.f;
}

void CEffectComponent::FadeOutAnimation()
{
	if (m_fAccTime < m_tEffectData.fFadeOutStartTime)
		return;

	float fFadeInPattenTime = m_tEffectData.fFadeOutEndTime - m_tEffectData.fFadeOutStartTime;

	float fAccTime = m_fAccTime - m_tEffectData.fFadeOutStartTime;

	m_fAlpha = 1.f - (fAccTime / fFadeInPattenTime);

	if (m_fAlpha < 0.f)
		m_fAlpha = 0.f;
}

HRESULT CEffectComponent::Add_Component()
{
	return S_OK;
}

void CEffectComponent::Set_ConstantTable()
{
	_matrix matWorld = m_pTransCom->m_matWorld;
	_matrix matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	_matrix matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);


	CB_EFFECT_INFO tCB_EFfectInfo;

	ZeroMemory(&tCB_EFfectInfo, sizeof(CB_EFFECT_INFO));

	XMFLOAT4 SpriteInfo = { m_fChapterX,m_fChapterY,(float)m_tEffectData.iUvWidth,(float)m_tEffectData.iUvHeight };
	XMFLOAT4 ColorInfo = { m_tEffectData.vColor.x,m_tEffectData.vColor.y,m_tEffectData.vColor.z,m_tEffectData.vColor.w };
	XMFLOAT4 AlphaTimeInfo = { m_fAlpha,0.f,0.f,0.f };
	tCB_EFfectInfo.AlphaTimeInfo = AlphaTimeInfo;
	tCB_EFfectInfo.ColorInfo = ColorInfo;
	tCB_EFfectInfo.SpriteInfo = SpriteInfo;

	m_pShaderCom->Get_UploadBuffer_EffectInfo()->CopyData(0, tCB_EFfectInfo);



}

CGameObject* CEffectComponent::Clone_GameObject(void* pArg)
{
	return nullptr;
}

CEffectComponent* CEffectComponent::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, EFFECTDATA tData, _vec3 vStartPos)
{
	CEffectComponent* pComponent = new CEffectComponent(pGraphicDevice,pCommandList);


	if(FAILED(pComponent->Ready_GameObjectPrototype(tData,  vStartPos)))
		return nullptr;

	return pComponent;
}

void CEffectComponent::Free()
{
	CGameObject::Free();
}
