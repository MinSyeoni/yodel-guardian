#include "stdafx.h"
#include "CardKey.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "PassageDoor.h"

CCardKey::CCardKey(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CCardKey::CCardKey(const CCardKey& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CCardKey::~CCardKey()
{
}

HRESULT CCardKey::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CCardKey::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	return S_OK;
}

HRESULT CCardKey::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), 
									m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CCardKey::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCollider);

	// 플레이어 키 연동 
	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYER))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCollider, pCol, &vShaveDir)
			&& CDirectInput::Get_Instance()->KEY_DOWN(DIK_E) && !m_bIsEquip)
			m_bIsEquip = true;
	}

	// 일단 카드 키를 문에다가 직접 댈 때
	PutTheCard_OnTheDoor();
	
	return NO_EVENT;
}

void CCardKey::PutTheCard_OnTheDoor()
{
	CGameObject* pPassageDoor = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"PassageDoor");

	if (nullptr == pPassageDoor)
		return;

	auto pBoxCol = dynamic_cast<CPassageDoor*>(pPassageDoor)->Get_DoorCollider();

	if (!m_bIsDead && CDirectInput::Get_Instance()->KEY_DOWN(DIK_E) && m_bIsEquip)
	{
		dynamic_cast<CPassageDoor*>(pPassageDoor)->Set_IsCardToDoor(true);
		m_bIsEquip = false;
		m_bIsDead = true;
	}
}

_int CCardKey::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCollider), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

	return NO_EVENT;
}

void CCardKey::Render_GameObject(const _float & fTimeDelta)
{
	if (m_bIsEquip)
		return;
		
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);
}

void CCardKey::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader);
	pShader->Set_ShadowFinish();
}

HRESULT CCardKey::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->
		Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// BoxCol
	m_pBoxCollider = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 100.f, 100.f), this));
	NULL_CHECK_RETURN(m_pBoxCollider, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCollider);

	return S_OK;
}

void CCardKey::Set_ConstantTable()
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

void CCardKey::Set_ShadowTable(CShader_Shadow * pShader)
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

CGameObject * CCardKey::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CCardKey(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CCardKey*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CCardKey* CCardKey::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CCardKey* pInstance = new CCardKey(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCardKey::Free()
{
	CGameObject::Free();
}
