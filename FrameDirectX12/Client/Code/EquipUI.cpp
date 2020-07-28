#include "stdafx.h"
#include "EquipUI.h"
#include "DirectInput.h"

CEquipUI::CEquipUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CEquipUI::CEquipUI(const CEquipUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CEquipUI::~CEquipUI()
{
}

HRESULT CEquipUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CEquipUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CEquipUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());
	
	m_pTransCom->m_vScale = _vec3{ 0.07f, 0.04f, 0.07f };

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;

	return S_OK;
}

_int CEquipUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CEquipUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	Init_TypePos();

	return NO_EVENT;
}

void CEquipUI::Init_TypePos()
{
	switch (m_eEquipType)
	{
	case CEquipUI::E_KITEQUIP1:
	case CEquipUI::E_KITEQUIP2:
	case CEquipUI::E_KITEQUIP3:
	{
		m_pTransCom->m_vPos.x = (((2.0f * m_vPos.x) / WINCX) * 0.5f);
		m_pTransCom->m_vPos.y = (((-2.0f * m_vPos.y) / WINCY) * 0.5f);
	}
	break;
	case CEquipUI::E_CONVERSATION:
	case CEquipUI::E_KEYEQUIP:
	{
		m_pTransCom->m_vPos.x = (((2.0f * m_vPos.x) / WINCX) * 0.5f) - 0.3f;
		m_pTransCom->m_vPos.y = (((-2.0f * m_vPos.y) / WINCY) * 0.5f) - 0.2f;
	}
	break;
	case CEquipUI::E_HIDING:
		break;
	case CEquipUI::E_DOOROPEN_L:
	case CEquipUI::E_DOOROPEN_P:
	{
		m_pTransCom->m_vPos.x = (((2.0f * m_vPos.x) / WINCX) * 0.5f) - 0.3f;
		m_pTransCom->m_vPos.y = (((-2.0f * m_vPos.y) / WINCY) * 0.5f) - 0.2f;
	}
	break;
	case CEquipUI::EQUIP_END:
		break;
	default:
		break;
	}
}

void CEquipUI::Render_GameObject(const _float& fTimeDelta)
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

HRESULT CEquipUI::Add_Component()
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
	wstring wstrPrototype = L"";
	switch (m_eEquipType)
	{
	case CEquipUI::E_CONVERSATION:
		wstrPrototype = L"Prototype_Texture_E_Conversation";
		break;
	case CEquipUI::E_KEYEQUIP:
	case CEquipUI::E_KITEQUIP1:
	case CEquipUI::E_KITEQUIP2:
	case CEquipUI::E_KITEQUIP3:
		wstrPrototype = L"Prototype_Texture_E_Equip";
		break;
	case CEquipUI::E_HIDING:
		wstrPrototype = L"Prototype_Texture_E_Hiding";
		break;
	case CEquipUI::E_DOOROPEN_L:
	case CEquipUI::E_DOOROPEN_P:
		wstrPrototype = L"Prototype_Texture_E_DoorOpen";
		break;
	case CEquipUI::EQUIP_END:
		break;
	default:
		break;
	}
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(wstrPrototype, COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);
	
	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CEquipUI::Set_ConstantTable()
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

CGameObject* CEquipUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CEquipUI(*this);

	_uint iEquipIdx = *reinterpret_cast<_uint*>(pArg);
	static_cast<CEquipUI*>(pInstance)->m_eEquipType = (CEquipUI::EQUIP_TYPE)iEquipIdx;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CEquipUI* CEquipUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CEquipUI* pInstance = new CEquipUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CEquipUI::Free()
{
	CGameObject::Free();
}
