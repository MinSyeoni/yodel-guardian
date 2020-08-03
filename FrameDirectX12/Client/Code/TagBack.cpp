#include "stdafx.h"
#include "TagBack.h"
#include "Player.h"
#include "DirectInput.h"

CTagBack::CTagBack(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CTagBack::CTagBack(const CTagBack& rhs)
	: Engine::CGameObject(rhs)
{
}

CTagBack::~CTagBack()
{
}

HRESULT CTagBack::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CTagBack::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CTagBack::LateInit_GameObject()
{
	for(int i = 0; i< 2; ++i)
		m_pShaderCom[i]->Set_Shader_Texture(m_pTexture[i]->Get_Texture());

	m_pTransCom->m_vPos = _vec3(261.9, 18.f, 500.f);
	m_pTransCom->m_vAngle = _vec3(0.f, -90.f, 0.f);
	m_pTransCom->m_vScale = _vec3(8.f, 6.f, 8.f);

	return S_OK;
}

_int CTagBack::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

//	BillBoard();

	return NO_EVENT;
}

_int CTagBack::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	_matrix matTmp = INIT_MATRIX;
	matTmp = m_pTransCom->m_matWorld;

	return NO_EVENT;
}

void CTagBack::BillBoard()
{
	_matrix matBill, matView;

	matBill = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matView = matBill;
	ZeroMemory(&matBill.m[3][0], sizeof(_vec3));

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill = XMMatrixInverse(nullptr, matBill);
	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;
}

void CTagBack::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable((_uint)m_eType);
	m_pShaderCom[(_uint)m_eType]->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom[(_uint)m_eType]->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

HRESULT CTagBack::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	wstring wstrText = L"";
	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
			wstrText = L"Prototype_Texture_TagBack_Red";
		else
			wstrText = L"Prototype_Texture_TagBack_Clear";

		wstring wstrText2 = L"";
		string strTemp2 = "";
		strTemp2 = "Com_Shader" + to_string(i);
		wstrText2.assign(strTemp2.begin(), strTemp2.end());

		// shader
		m_pShaderCom[i] = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(wstrText2.c_str(), m_pShaderCom[i]);

		wstrText2 = L"";
		strTemp2 = "Com_Texture" + to_string(i);
		wstrText2.assign(strTemp2.begin(), strTemp2.end());

		// Texture
		m_pTexture[i] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(wstrText.c_str(), COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(wstrText2.c_str(), m_pTexture[i]);
	}

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CTagBack::Set_ConstantTable(_uint iIdx)
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom[iIdx]->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CTagBack::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CTagBack(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CTagBack* CTagBack::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CTagBack* pInstance = new CTagBack(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTagBack::Free()
{
	CGameObject::Free();
}
