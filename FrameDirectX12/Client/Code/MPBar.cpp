#include "stdafx.h"
#include "MPBar.h"
#include "Player.h"
#include "DirectInput.h"
#include "Player.h"
CMPBar::CMPBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CMPBar::CMPBar(const CMPBar& rhs)
	: Engine::CGameObject(rhs)
{
}

CMPBar::~CMPBar()
{
}

HRESULT CMPBar::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CMPBar::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CMPBar::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;
	m_pTransCom->m_vPos.z = 0.01f; 

	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	m_fPreMp = m_fCurMp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurMp();

	return S_OK;
}

_int CMPBar::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	m_fCurMp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurMp();

	if (m_iMPType == 0)
	{
		if (m_fPreMp > m_fCurMp)
		{
			m_fPreMp -= 25.f * fTimeDelta;

			if (m_fPreMp <= m_fCurMp)
				m_fPreMp = m_fCurMp;
		}
	}
	else if (m_iMPType == 1)
	{
		if (m_fPreMp < m_fCurMp)
		{
			m_fPreMp += 60.f * fTimeDelta;

			if (m_fPreMp >= m_fCurMp)
				m_fPreMp = m_fCurMp;
		}
	}

	return NO_EVENT;
}

_int CMPBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	m_matMPWorld._41 = m_fPreMp * (0.0028 / 3);

	return NO_EVENT;
}


void CMPBar::Render_GameObject(const _float& fTimeDelta)
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

HRESULT CMPBar::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_PlayerMP", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CMPBar::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_matMPWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CMPBar::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CMPBar(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CMPBar* CMPBar::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CMPBar* pInstance = new CMPBar(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMPBar::Free()
{
	CGameObject::Free();
}
