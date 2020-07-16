#include "stdafx.h"
#include "LightObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "LightMgr.h"

CLightObject::CLightObject(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CLightObject::CLightObject(const CLightObject& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CLightObject::~CLightObject()
{
}

HRESULT CLightObject::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CLightObject::Ready_GameObject()
{



	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	//m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	if (m_eType == SYREN)
	{
		m_tagLight.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
		m_tagLight.m_vDiffuse = _vec4{ 0.9f,0.0f,0.0f,1.0f };
		m_tagLight.m_vAmbient = _vec4{ 0.6f,0.6f,0.6f,1.0f };
		m_tagLight.m_vSpecular = _vec4{ 0.6f,0.6f,0.6f,1.0f };
		m_tagLight.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
		m_tagLight.m_vPosition = _vec4{ 300.f,10.f,300.f,0.f };

		m_tagLight.m_fRange = 30.f;
	}
	m_tagLight.m_vPosition.x = m_pTransCom->m_vPos.x;
	m_tagLight.m_vPosition.y = m_pTransCom->m_vPos.y;
	m_tagLight.m_vPosition.z = m_pTransCom->m_vPos.z;

	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagLight)))
		return E_FAIL;




	return S_OK;
}

HRESULT CLightObject::LateInit_GameObject()
{
	

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CLightObject::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CLightObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	return NO_EVENT;
}

void CLightObject::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);
}


HRESULT CLightObject::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	return S_OK;
}

void CLightObject::Set_ConstantTable()
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



CGameObject* CLightObject::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CLightObject(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CLightObject*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CLightObject* CLightObject::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CLightObject* pInstance = new CLightObject(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLightObject::Free()
{
	CGameObject::Free();
}
