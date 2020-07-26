#include "stdafx.h"
#include "SkyDome.h"
#include "GraphicDevice.h"

CSkyDome::CSkyDome(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CSkyDome::CSkyDome(const CSkyDome & rhs)
	: Engine::CGameObject(rhs)
{
}

CSkyDome::~CSkyDome()
{
}

HRESULT CSkyDome::Ready_GameObjectPrototype()
{

#ifdef _DEBUG
	COUT_STR("Ready Prototype SkyDome");
#endif

	return S_OK;
}

HRESULT CSkyDome::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_SkyDome", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif






	return S_OK;
}

HRESULT CSkyDome::LateInit_GameObject()
{
	m_pTransCom->m_vPos = _vec3{ 300.f,0.f,300.f };
	m_pTransCom->m_vScale = _vec3{ 1.f,1.f,1.f };
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CSkyDome::Update_GameObject(const _float & fTimeDelta)
{

	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	
	FollowCamera();

	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CSkyDome::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_PRIORITY, this), -1);
	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/




	return NO_EVENT;
}


void CSkyDome::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);
}

void CSkyDome::FollowCamera()
{
	_matrix matViewInv = CGraphicDevice::Get_Instance()->GetViewMatrix();

	matViewInv = XMMatrixInverse(nullptr, matViewInv);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matViewInv._41, sizeof(_vec3));
	m_pTransCom->m_vPos = vCamPos;


}

HRESULT CSkyDome::Add_Component()
{
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_SkyDome", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	return S_OK;
}

void CSkyDome::Set_ConstantTable()
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

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject * CSkyDome::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CSkyDome(*this);


	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CSkyDome * CSkyDome::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CSkyDome* pInstance = new CSkyDome(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkyDome::Free()
{
	CGameObject::Free();

}
