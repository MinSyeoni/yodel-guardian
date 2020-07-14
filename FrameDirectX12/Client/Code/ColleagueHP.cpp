#include "stdafx.h"
#include "ColleagueHP.h"

CColleagueHP::CColleagueHP(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CColleagueHP::CColleagueHP(const CColleagueHP& rhs)
	: Engine::CGameObject(rhs)
{
}

CColleagueHP::~CColleagueHP()
{
}

HRESULT CColleagueHP::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CColleagueHP::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CColleagueHP::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	

	// 동료 HP로 
//	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
//	m_fPreHp = m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();

	return S_OK;
}

_int CColleagueHP::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vScale = _vec3(0.15f, 0.2f, 0.15f);

	m_pTransCom->m_vPos.x = _float(WINCX / 2.f) / _float(WINCX / 0.5f) - 1.f;
	m_pTransCom->m_vPos.y = _float(WINCY / 1.5f) / _float(WINCY / 0.1f) - 0.8f;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	// 동료 HP로 
	//CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");

	//m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();

	//if (m_fPreHp > m_fCurHp)
	//{
	//	m_fPreHp -= 15.f * fTimeDelta;

	//	if (m_fPreHp <= m_fCurHp)
	//		m_fPreHp = m_fCurHp;
	//}

	return NO_EVENT;
}

_int CColleagueHP::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

//	m_matHPWorld._11 = (m_fPreHp * 2 - 314) * 0.01;

	return NO_EVENT;
}

void CColleagueHP::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow || m_fPreHp <= 0.f)
		return;

	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();
}

HRESULT CColleagueHP::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_PlayerHP", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CColleagueHP::Set_ConstantTable()
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

CGameObject* CColleagueHP::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CColleagueHP(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CColleagueHP* CColleagueHP::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CColleagueHP* pInstance = new CColleagueHP(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CColleagueHP::Free()
{
	CGameObject::Free();
}
