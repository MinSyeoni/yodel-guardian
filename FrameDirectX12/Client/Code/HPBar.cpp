#include "stdafx.h"
#include "HPBar.h"
#include "Player.h"
#include "DirectInput.h"

CHPBar::CHPBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CHPBar::CHPBar(const CHPBar& rhs)
	: Engine::CGameObject(rhs)
	, m_eType(rhs.m_eType)
{
}

CHPBar::~CHPBar()
{
}

HRESULT CHPBar::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CHPBar::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CHPBar::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	Init_CharacterHp();

	return S_OK;
}

void CHPBar::Init_CharacterHp()
{
	switch (m_eType)
	{
	case CHPBar::PLAYER_HPBAER:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fPreHp = m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	case CHPBar::COLLEAGUE_HPBAR:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fPreHp = m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	case CHPBar::COLLEAGUE2_HPBAR:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fPreHp = m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	default:
		break;
	}
}

_int CHPBar::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Set_HP_Damage(fTimeDelta);

	return NO_EVENT;
}

void CHPBar::Set_HP_Damage(const _float& fTimeDelta)
{
	switch (m_eType)
	{
	case CHPBar::PLAYER_HPBAER:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	case CHPBar::COLLEAGUE_HPBAR:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	case CHPBar::COLLEAGUE2_HPBAR:
	{
		CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
		m_fCurHp = dynamic_cast<CPlayer*>(pPlayer)->Get_CurHP();
	}
	break;
	default:
		break;
	}

	if (m_iHPType == 0)
	{
		if (m_fPreHp > m_fCurHp)
		{
			m_fPreHp -= 15.f * fTimeDelta;

			if (m_fPreHp <= m_fCurHp)
				m_fPreHp = m_fCurHp;
		}
	}
	else if (m_iHPType == 1)
	{
		if (m_fPreHp < m_fCurHp)
		{
			m_fPreHp += 15.f * fTimeDelta;

			if (m_fPreHp >= m_fCurHp)
				m_fPreHp = m_fCurHp;
		}
	}
}

_int CHPBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	// hp 줄어드는거 테스트트트트트트
	//if (CDirectInput::Get_Instance()->KEY_PRESSING(DIK_9))
	//{
	//	//	if (m_fPreHp > m_fCurHp)
	//	m_fPreHp -= 15.f * fTimeDelta;
	//}

	m_matHPWorld._11 = (m_fPreHp * 2 - 314) * 0.01;

	return NO_EVENT;
}

void CHPBar::Render_GameObject(const _float& fTimeDelta)
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

HRESULT CHPBar::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	switch (m_eType)
	{
	case CHPBar::PLAYER_HPBAER:
	{
		// Shader
		m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_HPBAR", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

		// Texture
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_PlayerHP", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);
	}
		break;
	case CHPBar::COLLEAGUE_HPBAR:
	{		
		// Shader
		m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_HPBAR1", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

		// Texture
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_HP_colleague1", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);
	}
		break;
	case CHPBar::COLLEAGUE2_HPBAR:
	{
		// Shader
		m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_HPBAR2", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

		// Texture
		m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_HP_colleague2", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture); 
	}
		break;
	default:
		break;
	}

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CHPBar::Set_ConstantTable()
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

CGameObject* CHPBar::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CHPBar(*this);

	HPBAR_TYPE tHpBarType = *reinterpret_cast<HPBAR_TYPE*>(pArg);
	static_cast<CHPBar*>(pInstance)->m_eType = tHpBarType;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CHPBar* CHPBar::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CHPBar* pInstance = new CHPBar(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CHPBar::Free()
{
	CGameObject::Free();
}
