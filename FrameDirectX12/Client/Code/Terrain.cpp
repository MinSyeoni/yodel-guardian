#include "stdafx.h"
#include "Terrain.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


CTerrain::CTerrain(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: Engine::CGameObject(rhs)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype CubeObject");
#endif


	return S_OK;
}

HRESULT CTerrain::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone CubeObject");
#endif
	CGameObject::Ready_GameObject();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

HRESULT CTerrain::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit CubeObject");
#endif

	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());
	return S_OK;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vPos.y = -2.f;
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CTerrain::LateUpdate_GameObject(const _float & fTimeDelta)
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

void CTerrain::Render_GameObject(const _float & fTimeDelta)
{
	Engine::CGraphicDevice::Get_Instance()->Set_PipelineState(m_pShaderCom->Get_PipelineState());
	Set_ConstantTable();
	
	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

}

HRESULT CTerrain::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CTerrainTex*>(m_pComponentMgr->Clone_Component(L"Prototype_TerrainTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Default*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Default", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);


	//Texture
	
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	return S_OK;
}

void CTerrain::Set_ConstantTable()
{
	_matrix matWorld = INIT_MATRIX;
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

CGameObject * CTerrain::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CTerrain * CTerrain::Create(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList)
{
	CTerrain* pInstance = new CTerrain(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTexture);
}
