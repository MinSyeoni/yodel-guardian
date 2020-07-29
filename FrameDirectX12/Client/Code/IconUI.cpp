#include "stdafx.h"
#include "IconUI.h"


CIconUI::CIconUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CIconUI::CIconUI(const CIconUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CIconUI::~CIconUI()
{
}

HRESULT CIconUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CIconUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CIconUI::LateInit_GameObject()
{
	for(int i = 0; i < 3; ++i)
		m_pShaderCom[i]->Set_Shader_Texture(m_pTexture[i]->Get_Texture());	

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;

	return S_OK;
}

_int CIconUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CIconUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	return NO_EVENT;
}

void CIconUI::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable(m_iIconIdx);
	m_pShaderCom[m_iIconIdx]->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom[m_iIconIdx]->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

HRESULT CIconUI::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	for (int i = 0; i < 3; ++i)
	{
		m_pShaderCom[i] = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom[i]);
	}
	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	// ≈ÿΩ∫√≥
	for (int i = 0; i < 3; ++i)
	{
		wstring wstrTemp = L"Prototype_Texture_PlayerIcon";
		if(i == 1)
			wstrTemp = L"Prototype_Texture_colleague1Icon";
		else if(i == 2)
			wstrTemp = L"Prototype_Texture_colleague2Icon";

		m_pTexture[i] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(wstrTemp, COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture[i]);
	}

	return S_OK;
}

void CIconUI::Set_ConstantTable(_uint iIdx)
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

	m_pShaderCom[iIdx]->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CIconUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CIconUI(*this);

	_uint iIdx = *reinterpret_cast<_uint*>(pArg);
	static_cast<CIconUI*>(pInstance)->m_iIconIdx = iIdx;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CIconUI* CIconUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CIconUI* pInstance = new CIconUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CIconUI::Free()
{
	CGameObject::Free();
}
