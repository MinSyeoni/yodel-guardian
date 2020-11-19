#include "stdafx.h"
#include "FlameBullet.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"

#include "Player.h"
#include "Effect.h"
#include "LightMgr.h"
CFlameBullet::CFlameBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CFlameBullet::CFlameBullet(const CFlameBullet& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CFlameBullet::~CFlameBullet()
{
}

HRESULT CFlameBullet::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CFlameBullet::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	



	return S_OK;
}

HRESULT CFlameBullet::LateInit_GameObject()
{
	m_pDronEffect = dynamic_cast<CEffect*>(CObjectMgr::Get_Instance()->Get_NewGameObject(L"Prototype_Effect_DronBullet", L"NONE", &m_pTransCom->m_vPos));

	m_pTransCom->m_vPos = m_tInfo.vPos;
	return S_OK;
}

_int CFlameBullet::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;


	if (m_tInfo.flifeTime < m_fAccTime)
		m_bIsDead = true;

	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
	{
		if (m_pDronEffect != nullptr)
		{
		    m_pDronEffect->Dead_GameObject();
			m_pDronEffect->SetPos(m_pTransCom->m_vPos);
			m_pDronEffect->Update_GameObject(fTimeDelta);
		}
		return DEAD_OBJ;

	}
	if (m_pDronEffect != nullptr)
	{

		m_pDronEffect->SetPos(m_pTransCom->m_vPos);
		m_pDronEffect->Update_GameObject(fTimeDelta);
	}
	Move(fTimeDelta);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::COMBAT, m_pBoxCollider);


	return NO_EVENT;
}

_int CFlameBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{


	if (!m_bIsLateInit)
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);
	if (m_pDronEffect != nullptr)
		m_pDronEffect->LateUpdate_GameObject(fTimeDelta);

	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCollider), -1);

	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYER))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(pCol, m_pBoxCollider, &vShaveDir))
		{
			CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
			if (pPlayer == nullptr)
				return E_FAIL;
			dynamic_cast<CPlayer*>(pPlayer)->Set_FlameDamage(10);
			m_bIsDead = true;
		}
	}

	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYERVIEW))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(pCol, m_pBoxCollider, &vShaveDir))
		{
			m_bIsDead = true;
		}
	}


	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PASSBOX))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(pCol, m_pBoxCollider, &vShaveDir))
		{
			m_bIsDead = true;
		}
	}


	return NO_EVENT;
}

void CFlameBullet::Render_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
		return;

}

void CFlameBullet::Move(const _float& fTimeDelta)
{
	m_pTransCom->m_vPos += m_tInfo.vDir * m_tInfo.fSpeed * fTimeDelta;

}



void CFlameBullet::Render_ShadowDepth(CShader_Shadow* pShader)
{

}

HRESULT CFlameBullet::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Box
	m_pBoxCollider = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, false, nullptr, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pBoxCollider, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCollider);

	return S_OK;
}

void CFlameBullet::Set_ConstantTable()
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

void CFlameBullet::Set_ShadowTable(CShader_Shadow* pShader)
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

CGameObject* CFlameBullet::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CFlameBullet(*this);

	FLAMEINFO tMeshInfo = *reinterpret_cast<FLAMEINFO*>(prg);
	static_cast<CFlameBullet*>(pInstance)->m_tInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CFlameBullet* CFlameBullet::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CFlameBullet* pInstance = new CFlameBullet(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFlameBullet::Free()
{
	Safe_Release(m_pDronEffect);
	CGameObject::Free();
}
