#include "stdafx.h"
#include "CubeObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


CCubeObject::CCubeObject(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CCubeObject::CCubeObject(const CCubeObject & rhs)
	: Engine::CGameObject(rhs)
{
}

CCubeObject::~CCubeObject()
{
}

HRESULT CCubeObject::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype CubeObject");
#endif

	return S_OK;
}

HRESULT CCubeObject::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone CubeObject");
#endif
	CGameObject::Ready_GameObject();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

HRESULT CCubeObject::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit CubeObject");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/
	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif

	return S_OK;
}

_int CCubeObject::Update_GameObject(const _float & fTimeDelta)
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

_int CCubeObject::LateUpdate_GameObject(const _float & fTimeDelta)
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

void CCubeObject::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->End_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

	//m_pNaviMesh->Render_NaviMesh();
}

HRESULT CCubeObject::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CCubeTex*>(m_pComponentMgr->Clone_Component(L"Prototype_CubeTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);
#ifdef _DEBUG
	COUT_STR("Success CubeObject - Clone BufferCom");
#endif

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_ColorBuffer*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_ColorBuffer", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success CubeObject - Clone ShaderCom");
#endif

	/*m_pNaviMesh = static_cast<Engine::CNaviMesh*>(m_pComponentMgr->Clone_Component(L"Prototype_NaviMesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pNaviMesh, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_NaviMesh", m_pNaviMesh);*/

#ifdef _DEBUG
	COUT_STR("Success CubeObject - Clone Navi");
#endif

	return S_OK;
}

void CCubeObject::Set_ConstantTable()
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

CGameObject * CCubeObject::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CCubeObject(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CCubeObject * CCubeObject::Create(ID3D12Device * pGraphicDevice,
								  ID3D12GraphicsCommandList * pCommandList)
{
	CCubeObject* pInstance = new CCubeObject(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCubeObject::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
}
