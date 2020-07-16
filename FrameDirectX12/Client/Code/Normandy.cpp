#include "stdafx.h"
#include "Normandy.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"

CNormandy::CNormandy(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CNormandy::CNormandy(const CNormandy& rhs)
	: Engine::CGameObject(rhs)
{

}

CNormandy::~CNormandy()
{
}

HRESULT CNormandy::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CNormandy::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Normandy", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_pTransCom->m_vPos = _vec3(0.f, 0.f, 50.f);


	m_fTime = 0.f;
	return S_OK;
}

HRESULT CNormandy::LateInit_GameObject()
{


	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;

}

_int CNormandy::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vAngle.y = 90.f;
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_fTime += fTimeDelta;
	return NO_EVENT;
}

_int CNormandy::LateUpdate_GameObject(const _float& fTimeDelta)
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

void CNormandy::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

HRESULT CNormandy::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone ShaderCom");
#endif

	return S_OK;
}

void CNormandy::Set_ConstantTable()
{
	_matrix matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	_matrix matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	CB_MATRIX_INFO	tCB_MatrixInfo;
	CB_BONE_INFO	tCB_BoneInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));


	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));



	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(m_fTime * 3000.f);

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CNormandy::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CNormandy(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CNormandy* CNormandy::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CNormandy* pInstance = new CNormandy(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNormandy::Free()
{

	CGameObject::Free();
}
