#include "stdafx.h"
#include "StoneParticle.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
CStonePaticle::CStonePaticle(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CStonePaticle::CStonePaticle(const CStonePaticle& rhs)
	: Engine::CGameObject(rhs)
{

}

CStonePaticle::~CStonePaticle()
{
}

HRESULT CStonePaticle::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CStonePaticle::Ready_GameObject(_vec3 vPos)
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Meteor", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif

	int vRandX = rand() % 100-50;
	int vRandY = rand() % 100;
	int vRandZ = rand() % 100-50;
	m_pTransCom->m_vPos = vPos;

	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_vDir = _vec3((float)vRandX * 0.01f, (float)vRandY * 0.01f, (float)vRandZ * 0.01f);
	m_vDir.Normalize();
	return S_OK;
}

HRESULT CStonePaticle::LateInit_GameObject()
{


	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CStonePaticle::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_fAccTime += fTimeDelta;
	MovePaticle(fTimeDelta);
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CStonePaticle::LateUpdate_GameObject(const _float& fTimeDelta)
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

void CStonePaticle::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsLateInit)
		return;

	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);

}

void CStonePaticle::MovePaticle(const _float& fTimeDelta)
{
	m_pTransCom->m_vPos.x += m_vDir.x * fTimeDelta * 7.f;
	m_pTransCom->m_vPos.y += m_vDir.y * fTimeDelta * 10.f;
	m_pTransCom->m_vPos.z += m_vDir.y * fTimeDelta * 7.f;
	m_pTransCom->m_vPos.y -=  (m_fGravity * m_fAccTime);

	if (m_pTransCom->m_vPos.y < 0)
		m_bIsDead = true;
}


HRESULT CStonePaticle::Add_Component()
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

void CStonePaticle::Set_ConstantTable()
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



CGameObject* CStonePaticle::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CStonePaticle(*this);

	_vec3 vPos = *reinterpret_cast<_vec3*>(prg);
	if (FAILED(static_cast<CStonePaticle*>(pInstance)->Ready_GameObject(vPos)))
		return nullptr;

	return pInstance;
}

CStonePaticle* CStonePaticle::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CStonePaticle* pInstance = new CStonePaticle(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStonePaticle::Free()
{

	CGameObject::Free();


}
