#include "stdafx.h"
#include "LoadingBar.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"
#include "LoadingTexture.h"
CLoadingBar::CLoadingBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CLoadingBar::CLoadingBar(const CLoadingBar& rhs)
	: Engine::CGameObject(rhs)
{

}


CLoadingBar::~CLoadingBar()
{
}

HRESULT CLoadingBar::Ready_GameObjectPrototype()
{


	return S_OK;
}


HRESULT CLoadingBar::Ready_GameObject()	// 복사본을 레디할 때 
{
#ifdef _DEBUG
	COUT_STR("Ready Clone LoadingBar");
#endif
	//CGameObject::Ready_GameObject();
	Add_Component();



	return S_OK;
}

HRESULT CLoadingBar::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit LoadingBar");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/



#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	// 셰이더에 텍스쳐 알려줘여 함.

	return S_OK;
}

_int CLoadingBar::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	//m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;

	if (m_fNewGaugeBar > m_fGaugeBar)
	{
		m_fGaugeBar += fTimeDelta*0.5f;
	}

	if (m_fGaugeBar >= 1.0f)
	{

		static_cast<CLoadingTexture*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"LoadingTexture"))->SetFinish();

	}


	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CLoadingBar::LateUpdate_GameObject(const _float& fTimeDelta)
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

void CLoadingBar::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();
	// 세이더 - 버퍼 - 세이더 - 버퍼 순서 중요 


	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader(3);
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

}

HRESULT CLoadingBar::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Logo", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	return S_OK;
}

void CLoadingBar::Set_ConstantTable()
{
	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matView = INIT_MATRIX;
	_matrix matProj = CGraphicDevice::Get_Instance()->GetOrthoMatrix();
	_matrix matWorld = INIT_MATRIX;



	matWorld._11 = WINSIZEX/2;
	matWorld._22 = WINSIZEY/2;
	matWorld._33 = 1.0f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;
	matWorld._43 = 0.09f;
	_matrix matWVP = matWorld * matView * matProj;


	matWorld._41 = 1.f;
	matWorld._42 = 1-m_fGaugeBar;  //0.3~0.7  

	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CLoadingBar::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CLoadingBar(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CLoadingBar* CLoadingBar::Create(ID3D12Device* pGraphicDevice,
	ID3D12GraphicsCommandList* pCommandList)
{
	CLoadingBar* pInstance = new CLoadingBar(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLoadingBar::Free()
{
	CGameObject::Free();
}
