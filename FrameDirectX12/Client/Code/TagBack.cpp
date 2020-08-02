#include "stdafx.h"
#include "TagBack.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


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

HRESULT CTagBack::Ready_GameObject()	// 복사본을 레디할 때 
{
	Add_Component();


	return S_OK;
}

HRESULT CTagBack::LateInit_GameObject()
{
	for(int i = 0; i < 2; ++i)
		m_pShaderCom[i]->Set_Shader_Texture(m_pTexture[i]->Get_Texture());	

	return S_OK;
}

_int CTagBack::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CTagBack::LateUpdate_GameObject(const _float & fTimeDelta)
{
	//NULL_CHECK_RETURN(m_pRenderer, -1);
	//FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);


	return NO_EVENT;
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

	for (int i = 0; i < 2; ++i)
	{
		// Shader
		m_pShaderCom[i] = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom[i]);

		// Texture
		wstring wstrTmp = L"";
		if (i == 0)
			wstrTmp = L"Prototype_Texture_TagBack_Red";
		else if(i == 1)
			wstrTmp = L"Prototype_Texture_TagBack_Clear";
		m_pTexture[i] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(wstrTmp.c_str(), COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture[i]);
	}

	return S_OK;
}

void CTagBack::Set_ConstantTable(_uint iIdx)
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

CGameObject * CTagBack::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CTagBack(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CTagBack* CTagBack::Create(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList)
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
