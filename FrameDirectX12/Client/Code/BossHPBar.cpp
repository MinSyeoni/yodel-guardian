#include "stdafx.h"
#include "BossHPBar.h"
#include "Reapear.h"
#include "DirectInput.h"

CBossHPBar::CBossHPBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CBossHPBar::CBossHPBar(const CBossHPBar& rhs)
	: Engine::CGameObject(rhs)
{
}

CBossHPBar::~CBossHPBar()
{
}

HRESULT CBossHPBar::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CBossHPBar::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CBossHPBar::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;
	m_pTransCom->m_vPos.z = 0.01f; 

	CGameObject* pReapear = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Reapear");
	if(pReapear != nullptr)
		m_fPreHp = m_fCurHp = dynamic_cast<CReapear*>(pReapear)->Get_CurBossHP();

	return S_OK;
}

_int CBossHPBar::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CGameObject* pReapear = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Reapear");
	if (pReapear != nullptr)
		m_fCurHp = dynamic_cast<CReapear*>(pReapear)->Get_CurBossHP();

	if (m_fPreHp > m_fCurHp)
	{
		m_fPreHp -= 15.f * fTimeDelta;

		if (m_fPreHp <= m_fCurHp)
			m_fPreHp = m_fCurHp;
	}
	
	return NO_EVENT;
}

_int CBossHPBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	if (CDirectInput::Get_Instance()->KEY_PRESSING(DIK_N))
	{
		//	if (m_fPreHp > m_fCurHp)
		m_fPreHp -= 15.f * fTimeDelta;
	}

	m_matHPWorld._41 = m_fPreHp * (0.008f);

	return NO_EVENT;
}


void CBossHPBar::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable();
	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

HRESULT CBossHPBar::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_MPBAR", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture 
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_BossBlood", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CBossHPBar::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_matHPWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CBossHPBar::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CBossHPBar(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CBossHPBar* CBossHPBar::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CBossHPBar* pInstance = new CBossHPBar(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBossHPBar::Free()
{
	CGameObject::Free();
}
