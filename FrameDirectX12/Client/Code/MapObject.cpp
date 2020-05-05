#include "stdafx.h"
#include "MapObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"


CMapObject::CMapObject(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CMapObject::CMapObject(const CMapObject& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CMapObject::~CMapObject()
{
}

HRESULT CMapObject::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CMapObject::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	//m_pTransCom->m_vPos = m_tMeshInfo.Pos - _vec3{ 0.f, 0.f, 180.f };
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	return S_OK;
}

HRESULT CMapObject::LateInit_GameObject()
{
	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CMapObject::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CMapObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	return NO_EVENT;
}

void CMapObject::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);
}

void CMapObject::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader);
	pShader->Set_ShadowFinish();
}

HRESULT CMapObject::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	return S_OK;
}

void CMapObject::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

void CMapObject::Set_ShadowTable(CShader_Shadow * pShader)
{
	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));

	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_SHADOW_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_SHADOW_INFO));

	matView = CFrustom::Get_Instance()->Get_LightView();
	matProj = CFrustom::Get_Instance()->Get_LightProj();
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY * m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	tCB_MatrixInfo.blsMesh = true;

	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);
}

CGameObject * CMapObject::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CMapObject(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CMapObject*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CMapObject* CMapObject::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CMapObject* pInstance = new CMapObject(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMapObject::Free()
{
	CGameObject::Free();
}
