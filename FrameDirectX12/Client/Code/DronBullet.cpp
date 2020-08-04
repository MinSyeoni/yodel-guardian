#include "stdafx.h"
#include "DronBullet.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"

#include "Player.h"

CDronBullet::CDronBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CDronBullet::CDronBullet(const CDronBullet& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CDronBullet::~CDronBullet()
{
}

HRESULT CDronBullet::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CDronBullet::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	return S_OK;
}

HRESULT CDronBullet::LateInit_GameObject()
{
	//m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CDronBullet::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pSphereCollider->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::COMBAT, m_pSphereCollider);

	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	if (pPlayer == nullptr)
		return E_FAIL;
	
	m_vPlayerPos = pPlayer->Get_Transform()->m_vPos;
	m_vPlayerPos.y = 13.f;
	_vec3 vChaseDir = m_vPlayerPos - m_pTransCom->m_vPos;
	vChaseDir.Normalize();
	m_pTransCom->m_vPos = m_pTransCom->m_vPos + vChaseDir * 50.f * fTimeDelta;

	return NO_EVENT;
}

_int CDronBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	//if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
	//	return NO_EVENT; 

	NULL_CHECK_RETURN(m_pRenderer, -1);

	//FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	//FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pSphereCollider), -1);
	
	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::SPHERE, CColliderMgr::PLAYER))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_Spere(pCol, m_pSphereCollider, &vShaveDir))
		{
			m_bIsDead = true;
		}
	}

	return NO_EVENT;
}

void CDronBullet::Render_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return;

	//Set_ConstantTable();
	//m_pShaderCom->Begin_Shader();
	//m_pMeshCom->Render_Mesh(m_pShaderCom);
}

void CDronBullet::Render_ShadowDepth(CShader_Shadow* pShader)
{
	//Set_ShadowTable(pShader);
	//m_pMeshCom->Render_ShadowMesh(pShader);
	//pShader->Set_ShadowFinish();
}

HRESULT CDronBullet::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Shader
	//m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	//NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Buffer
	//m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	//NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	// Sphere
	m_pSphereCollider = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, nullptr, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 1.f, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pSphereCollider, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol", m_pSphereCollider);

	return S_OK;
}

void CDronBullet::Set_ConstantTable()
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

	//m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

void CDronBullet::Set_ShadowTable(CShader_Shadow* pShader)
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
	tCB_MatrixInfo.blsMesh = true;

	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);
}

CGameObject* CDronBullet::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CDronBullet(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CDronBullet*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CDronBullet* CDronBullet::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CDronBullet* pInstance = new CDronBullet(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDronBullet::Free()
{
	CGameObject::Free();
}
