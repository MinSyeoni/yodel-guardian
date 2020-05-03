#include "stdafx.h"
#include "Terrain.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "Frustom.h"

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
	m_pTransCom->m_vScale = _vec3{ 10.f, 10.f, 10.f };

	m_pShaderCom->Set_Shader_Texture(m_pDiffuseTexture->Get_Texture(), m_pNormalTexture->Get_Texture());
	return S_OK;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;


	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CTerrain::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
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

void CTerrain::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pBufferCom->Begin_Buffer();
	pShader->End_ShadowShader(false);
	m_pBufferCom->Render_Buffer();
	pShader->Set_ShadowFinish();

}

HRESULT CTerrain::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CTerrainTex*>(m_pComponentMgr->Clone_Component(L"Prototype_TerrainTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Terrain*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Terrain", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);


	//Texture
	
	m_pDiffuseTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Terrain", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pDiffuseTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_DiffuseTexture", m_pDiffuseTexture);

	m_pNormalTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_TerrainN", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pNormalTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_NormalTexture", m_pNormalTexture);

	return S_OK;
}

void CTerrain::Set_ShadowTable(CShader_Shadow * pShader)
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_SHADOW_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_SHADOW_INFO));

	matView = CFrustom::Get_Instance()->Get_LightView();
	matProj = CFrustom::Get_Instance()->Get_LightProj();


	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	tCB_MatrixInfo.blsMesh = false;


	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);


}

void CTerrain::Set_ConstantTable()
{
	_matrix matWorld = INIT_MATRIX;
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
	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);


	_matrix matLightView = INIT_MATRIX;
	_matrix matLightProj = INIT_MATRIX;

	CB_LIGHTMATRIX_INFO	tCB_LightMatrixInfo;
	ZeroMemory(&tCB_LightMatrixInfo, sizeof(CB_LIGHTMATRIX_INFO));
	XMStoreFloat4x4(&tCB_LightMatrixInfo.matLightView, XMMatrixTranspose(CFrustom::Get_Instance()->Get_LightView()));
	XMStoreFloat4x4(&tCB_LightMatrixInfo.matLightProj, XMMatrixTranspose(CFrustom::Get_Instance()->Get_LightProj()));

	m_pShaderCom->Get_UploadBuffer_LightMatrixInfo()->CopyData(0, tCB_LightMatrixInfo);

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


}
