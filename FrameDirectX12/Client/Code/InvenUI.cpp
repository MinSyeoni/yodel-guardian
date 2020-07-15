#include "stdafx.h"
#include "InvenUI.h"


CInvenUI::CInvenUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CInvenUI::CInvenUI(const CInvenUI& rhs)
	: Engine::CGameObject(rhs)
	, m_eIconType(rhs.m_eIconType)
	, m_bIsShow(rhs.m_bIsShow)
{
}

CInvenUI::~CInvenUI()
{
}

HRESULT CInvenUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CInvenUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CInvenUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	

	switch (m_eIconType)
	{
	case CInvenUI::ICON_MEDICINE:
	{
		m_pTransCom->m_vScale = _vec3(0.045f, 0.045f, 0.045f);
		m_pTransCom->m_vPos.x = _float(WINCX / 2.f) / _float(WINCX / 0.5f) + 0.6f;
		m_pTransCom->m_vPos.y = _float(WINCY / 1.5f) / _float(WINCY / 0.1f) - 0.47f;
	}
	break;
	case CInvenUI::ICON_BANDAGE:
	{
		m_pTransCom->m_vScale = _vec3(0.045f, 0.045f, 0.045f);
		m_pTransCom->m_vPos.x = _float(WINCX / 2.f) / _float(WINCX / 0.5f) + 0.6f;
		m_pTransCom->m_vPos.y = _float(WINCY / 1.5f) / _float(WINCY / 0.1f) - 0.57f;
	}
	break;
	case CInvenUI::ICON_SYRINGE:
	{
		m_pTransCom->m_vScale = _vec3(0.045f, 0.045f, 0.045f);
		m_pTransCom->m_vPos.x = _float(WINCX / 2.f) / _float(WINCX / 0.5f) + 0.6f;
		m_pTransCom->m_vPos.y = _float(WINCY / 1.5f) / _float(WINCY / 0.1f) - 0.67f;
	}
	break;
	default:
		break;
	}

	return S_OK;
}

_int CInvenUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CInvenUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	return NO_EVENT;
}

void CInvenUI::Render_GameObject(const _float& fTimeDelta)
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

HRESULT CInvenUI::Add_Component()
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

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);
	
	// TextureCom
	switch (m_eIconType)
	{
	case CInvenUI::ICON_MEDICINE:
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_MedicineIcon", COMPONENTID::ID_STATIC));
		break;
	case CInvenUI::ICON_BANDAGE:
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_BandageIcon", COMPONENTID::ID_STATIC));
		break;
	case CInvenUI::ICON_SYRINGE:
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_SyringeIcon", COMPONENTID::ID_STATIC));
		break;
	default:
		break;
	}
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	return S_OK;
}

void CInvenUI::Set_ConstantTable()
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

CGameObject* CInvenUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CInvenUI(*this);

	INVEN_ICON tIconType = *reinterpret_cast<INVEN_ICON*>(pArg);
	static_cast<CInvenUI*>(pInstance)->m_eIconType = tIconType;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CInvenUI* CInvenUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CInvenUI* pInstance = new CInvenUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CInvenUI::Free()
{
	CGameObject::Free();
}
