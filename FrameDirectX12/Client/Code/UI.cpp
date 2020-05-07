#include "stdafx.h"
#include "UI.h"


CUI::CUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CUI::CUI(const CUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CUI::Ready_GameObject()	// 복사본을 레디할 때 
{
	//CGameObject::Ready_GameObject();
	Add_Component();

	return S_OK;
}

HRESULT CUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	// 셰이더에 텍스쳐 알려줘여 함.

	return S_OK;
}

_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX/2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY/2) + 1.f;

	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/

	return NO_EVENT;
}

void CUI::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();
	// 세이더 - 버퍼 - 세이더 - 버퍼 순서 중요 

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();
}

HRESULT CUI::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_UI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CUI::Set_ConstantTable()
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

CGameObject* CUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CUI* CUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CUI* pInstance = new CUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CUI::Free()
{
	CGameObject::Free();
}
