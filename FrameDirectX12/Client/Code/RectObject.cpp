#include "stdafx.h"
#include "RectObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


CRectObject::CRectObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CRectObject::CRectObject(const CRectObject & rhs)
	: Engine::CGameObject(rhs)
{
}


CRectObject::~CRectObject()
{
}

HRESULT CRectObject::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}


HRESULT CRectObject::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone RectObject");
#endif
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

HRESULT CRectObject::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit RectObject");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/
	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif
	//m_pShaderCom->Set_Shader_Texture();

	return S_OK;
}

_int CRectObject::Update_GameObject(const _float & fTimeDelta)
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

_int CRectObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/


	return NO_EVENT;
}

void CRectObject::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

	CGraphicDevice::Get_Instance()-> Wait_ForGpuComplete();
}

HRESULT CRectObject::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CCubeTex*>(m_pComponentMgr->Clone_Component(L"Prototype_CubeTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone BufferCom");
#endif

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_DefaultTex*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_DefaultTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone ShaderCom");
#endif

	return S_OK;
}

void CRectObject::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	if (nullptr != m_pDynamicCamera)
	{
		matView = m_pDynamicCamera->Get_CameraInfo().matView;
		matProj = m_pDynamicCamera->Get_ProjInfo().matProj;
	}

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject * CRectObject::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CRectObject(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CRectObject * CRectObject::Create(ID3D12Device * pGraphicDevice,
								  ID3D12GraphicsCommandList * pCommandList)
{
	CRectObject* pInstance = new CRectObject(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRectObject::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
}
