#include "stdafx.h"
#include "BackGround.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


CBackGround::CBackGround(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	: Engine::CGameObject(rhs)
{

}


CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype BackGround");
#endif


	return S_OK;
}


HRESULT CBackGround::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone BackGround");
#endif
	CGameObject::Ready_GameObject();
	Add_Component();



	return S_OK;
}

HRESULT CBackGround::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit BackGround");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/


#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;

	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CBackGround::LateUpdate_GameObject(const _float & fTimeDelta)
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

void CBackGround::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();


	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();
}

HRESULT CBackGround::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_DefaultTex*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_DefaultTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);


	// Texture
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Logo", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pShaderCom);

	return S_OK;
}

void CBackGround::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	

	_matrix matWVP = INIT_MATRIX * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(INIT_MATRIX));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));



	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject * CBackGround::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CBackGround * CBackGround::Create(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList)
{
	CBackGround* pInstance = new CBackGround(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBackGround::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTexture);
}
