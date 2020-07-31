#include "stdafx.h"
#include "OnUI.h"
#include "DirectInput.h"

COnUI::COnUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

COnUI::COnUI(const COnUI& rhs)
	: Engine::CGameObject(rhs)
{
}

COnUI::~COnUI()
{
}

HRESULT COnUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT COnUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT COnUI::LateInit_GameObject()
{
	m_pShaderCom[0]->Set_Shader_Texture(m_pTexture[0]->Get_Texture());
	m_pShaderCom[1]->Set_Shader_Texture(m_pTexture[1]->Get_Texture());

	Set_OnUI_Pos();

	return S_OK;
}

void COnUI::CheckRenderer()
{
	m_pRenderer->SetOption(m_iOnIdx,!m_bIsOff);
}

void COnUI::Set_OnUI_Pos()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_iOnIdx == i)
		{
			m_pTransCom->m_vScale = _vec3{ 0.06f, 0.035f, 0.06f };
			m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.085f;
			m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + (1.12f - i * 0.14f);
		}
	}
	for (int i = 4; i < 7; ++i)
	{
		if (m_iOnIdx == i)
		{
			m_pTransCom->m_vScale = _vec3{ 0.06f, 0.035f, 0.06f };
			m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 0.62f;
			m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + (1.12f - (i - 4) * 0.14f);
		}
	}
}

_int COnUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	GetCursorPos(&m_pt);
	ScreenToClient(g_hWnd, &m_pt);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int COnUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CheckRenderer();
	NULL_CHECK_RETURN(m_pRenderer, -1);

	// 전체 On, Off
	if (!m_bIsDead && CDirectInput::Get_Instance()->KEY_DOWN(DIK_N) && m_bIsShow && m_bIsOff)
		m_bIsOff = false;
	else if (!m_bIsDead && CDirectInput::Get_Instance()->KEY_DOWN(DIK_N) && m_bIsShow && !m_bIsOff)
		m_bIsOff = true;

	Show_Option_OnOff();

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	return NO_EVENT;
}

void COnUI::Show_Option_OnOff()
{
	if (Engine::CDirectInput::Get_Instance()->Mouse_KeyDown(DIM_LB) && !m_bIsDead && m_bIsShow)
	{
		_float X = (((2.0f * m_pt.x) / WINCX) - 1.0f);
		_float Y = (((-2.0f * m_pt.y) / WINCY) + 1.0f);
		cout << X << "마우스" << Y << endl;
		for (int i = 0; i < 4; ++i)
		{
			if (m_iOnIdx == i)
			{
				_float fX = _float(2.f / WINCX * WINCX / 2) - 1.085f;
				_float fY = _float(-2.f / WINCY * WINCY / 2) + (1.12f - i * 0.14f);

				if ((X >= (fX - 0.1f) &&
					(X <= (fX + 0.06f))) &&
					(Y <= (fY + 0.08f) &&
					(Y >= fY)))
					if (m_bIsOff)
						m_bIsOff = false;
					else
						m_bIsOff = true;
			}
		}
		for (int i = 4; i < 7; ++i)
		{
			if (m_iOnIdx == i)
			{
				_float fX = _float(2.f / WINCX * WINCX / 2) - 0.62f;
				_float fY = _float(-2.f / WINCY * WINCY / 2) + (1.12f - (i - 4) * 0.14f);

				if ((X >= (fX - 0.1f) &&
					(X <= (fX + 0.06f))) &&
					(Y <= (fY + 0.08f) &&
					(Y >= fY)))
					if (m_bIsOff)
						m_bIsOff = false;
					else
						m_bIsOff = true;
			}
		}
	}
}

void COnUI::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable((int)m_bIsOff);
	m_pShaderCom[(int)m_bIsOff]->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom[(int)m_bIsOff]->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

HRESULT COnUI::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	wstring wstrComponent = L"Com_Shader";
	// Shader
	for (int i = 0; i < 2; ++i)
	{
		wstrComponent += i;
		m_pShaderCom[i] = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(wstrComponent, m_pShaderCom[i]);
	}

	// Texture 
	m_pTexture[0] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_OnUI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture1", m_pTexture[0]);

	m_pTexture[1] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_OffUI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture2", m_pTexture[1]);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void COnUI::Set_ConstantTable(int iIdx)
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

CGameObject* COnUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new COnUI(*this);

	_uint iOnIdx = *reinterpret_cast<_uint*>(pArg);
	static_cast<COnUI*>(pInstance)->m_iOnIdx = iOnIdx;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

COnUI* COnUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	COnUI* pInstance = new COnUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void COnUI::Free()
{
	CGameObject::Free();
}
