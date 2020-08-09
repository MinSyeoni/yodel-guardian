#include "stdafx.h"
#include "OptionUI.h"
#include "HPBar.h"
#include "IconUI.h"
#include "GunUI.h"
#include "InvenUI.h"
#include "DirectInput.h"
#include "OnUI.h"
#include "QuestUI.h"
#include "EquipUI.h"
#include "MousePoint.h"
#include "BossHPBar.h"
#include "BossBack.h"

COptionUI::COptionUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

COptionUI::COptionUI(const COptionUI& rhs)
	: Engine::CGameObject(rhs)
{
}

COptionUI::~COptionUI()
{
}

HRESULT COptionUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT COptionUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT COptionUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;

	return S_OK;
}

_int COptionUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int COptionUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	Show_OptionUI();

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	return NO_EVENT;
}

void COptionUI::Show_OptionUI()
{
	if (!m_bIsDead && CDirectInput::Get_Instance()->KEY_DOWN(DIK_0) && !m_bIsShow)
	{
	
		m_bIsShow = true;
		Show_OtherUI();
		CObjectMgr::Get_Instance()->SetTimeStop(false);
	}
	else if (!m_bIsDead && CDirectInput::Get_Instance()->KEY_DOWN(DIK_0) && m_bIsShow)
	{

 		m_bIsShow = false;
		Show_OtherUI();
		CObjectMgr::Get_Instance()->SetTimeStop(true);
	}
}

void COptionUI::Show_OtherUI()
{
	list<CGameObject*>* pHpBarUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"HPBarUI");
	if (pHpBarUIList != nullptr)
	{
		for (auto& pSrc : *pHpBarUIList)
			dynamic_cast<CHPBar*>(pSrc)->Set_ShowUI(!m_bIsShow);
	}

	list<CGameObject*>* pOnUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"OnUI");
	if (pOnUIList != nullptr)
	{
		for (auto& pSrc : *pOnUIList)
			dynamic_cast<COnUI*>(pSrc)->Set_ShowUI(m_bIsShow);
	}

	list<CGameObject*>* pIconUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"IconUI");
	if (pIconUIList != nullptr)
	{
		for (auto& pSrc : *pIconUIList)
			dynamic_cast<CIconUI*>(pSrc)->Set_ShowUI(!m_bIsShow);
	}

	list<CGameObject*>* pQuestUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"QuestUI");
	if (pQuestUIList != nullptr)
	{
		for (auto& pSrc : *pQuestUIList)
			dynamic_cast<CQuestUI*>(pSrc)->Set_ShowUI(!m_bIsShow);
	}

	CGameObject* pGunUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"GunUI");
	if (pGunUI != nullptr)
		dynamic_cast<CGunUI*>(pGunUI)->Set_ShowUI(!m_bIsShow);

	CGameObject* pInvenUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
	if (pInvenUI != nullptr)
		dynamic_cast<CInvenUI*>(pInvenUI)->Set_ShowUI(!m_bIsShow);

	list<CGameObject*>* pEquipUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"EquipUI");
	if (pEquipUIList != nullptr)
	{
		for (auto& pSrc : *pEquipUIList)
			dynamic_cast<CEquipUI*>(pSrc)->Set_ShowUI(false);
	}

	CGameObject* pMousePoint = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"MouseUI");
	if (pMousePoint != nullptr)
		dynamic_cast<CMousePoint*>(pMousePoint)->Set_ShowUI(m_bIsShow);

	CGameObject* pBossHPUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"BossHPBar");
	if (pBossHPUI != nullptr)
		dynamic_cast<CBossHPBar*>(pBossHPUI)->Set_ShowUI(!m_bIsShow);

	CGameObject* pBossBackUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"BossBack");
	if (pBossBackUI != nullptr)
		dynamic_cast<CBossBack*>(pBossBackUI)->Set_ShowUI(!m_bIsShow);
}

void COptionUI::Render_GameObject(const _float& fTimeDelta)
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

HRESULT COptionUI::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture 
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_OptionUI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void COptionUI::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* COptionUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new COptionUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

COptionUI* COptionUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	COptionUI* pInstance = new COptionUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void COptionUI::Free()
{
	CGameObject::Free();
}
