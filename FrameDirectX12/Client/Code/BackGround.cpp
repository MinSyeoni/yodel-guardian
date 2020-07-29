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


	return S_OK;
}


HRESULT CBackGround::Ready_GameObject()	// 복사본을 레디할 때 
{
#ifdef _DEBUG
	COUT_STR("Ready Clone BackGround");
#endif
	//CGameObject::Ready_GameObject();
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
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	// 셰이더에 텍스쳐 알려줘여 함.

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	//m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;

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
	// 세이더 - 버퍼 - 세이더 - 버퍼 순서 중요 


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
	m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Logo", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	return S_OK;
}

void CBackGround::Set_ConstantTable()
{
	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matView = INIT_MATRIX;
	_matrix matProj = CGraphicDevice::Get_Instance()->GetOrthoMatrix();
	_matrix matWorld = INIT_MATRIX;



	matWorld._11 = WINSIZEX/2 ;
	matWorld._22 = WINSIZEY/2;
	matWorld._33 = 1.0f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;
	matWorld._43 = 0.11f;
	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
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
}
