
#include "stdafx.h"
#include "Trigger.h"
#include "DirectSound.h"
#include "ObjectMgr.h"
#include "ColliderMgr.h"
#include "GraphicDevice.h"
#include "Monster.h"
#include "LobbyDoor.h"


CTrigger::CTrigger(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CGameObject(pGraphicDevice,pCommandList)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	:CGameObject(rhs),
	m_tColInfo(rhs.m_tColInfo)
{
}

CTrigger::~CTrigger()
{
}

HRESULT CTrigger::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype Monster");
#endif

	return S_OK;
}

HRESULT CTrigger::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

#ifdef _DEBUG
	COUT_STR("Success Monster - Clone Mesh");
#endif
	m_eCurShape = (TRIGGER_SHAPE)m_tColInfo.iType;

	FAILED_CHECK_RETURN(Add_Component(),E_FAIL);

	m_pTransCom->m_vPos = m_tColInfo.vCenter;
	m_pTransCom->m_vScale = m_tColInfo.vScale * 0.1f;
	m_pTransCom->m_vAngle = m_tColInfo.vRotate;
	m_iColID = m_tColInfo.iColID;

	if (m_eCurShape == CTrigger::TRIGGER_BOX && m_tColInfo.iColID == 2)
		m_pTransCom->m_vAngle = _vec3{ 0.f,0.f,0.f };

	return S_OK;
}

HRESULT CTrigger::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit Monster");
#endif

	return S_OK;
}

_int CTrigger::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	switch (m_eCurShape)
	{
	case CTrigger::TRIGGER_SPHERE:
	{
		m_pShereCol->Update_Collider(&m_pTransCom->m_matWorld);
		CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::TRIGGER, m_pShereCol);
	}
		break;
	case CTrigger::TRIGGER_BOX:
	{
		m_pBoxCol->Update_Collider(&m_pTransCom->m_matWorld);
		CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::TRIGGER, m_pBoxCol);
	
		_vec3 vShaveDir;
		for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYER))
		{
			if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCol, pCol, &vShaveDir))
			{
				//if (m_tColInfo.iColID == 1)
				//	CDirectSound::Get_Instance()->PlayDirectSoundFile(L"Siren");
				m_bIsActive = true;
			}
		}
	}
		break;
	case CTrigger::TRIGGER_END:
		break;
	default:
		break;
	}

	return NO_EVENT;
}

_int CTrigger::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	switch (m_eCurShape)
	{
	case CTrigger::TRIGGER_SPHERE:
	{
		FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol), -1);
	}
		break;
	case CTrigger::TRIGGER_BOX:
	{
		FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCol), -1);

		if (m_bIsActive && m_tColInfo.iColID != 10)
		{
			list<CGameObject*>* pList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Zombi");
			for (auto& pSrc : *pList)
			{
				if (m_tColInfo.iColID == 1)
				{
					CGameObject* pLobbyDoor = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"LobbyDoor");
					if (!static_cast<CLobbyDoor*>(pLobbyDoor)->Get_LobbyDoorIsOpen())
						return E_FAIL;
				}

				if (static_cast<CMonster*>(pSrc)->Get_MONKIND() == CMonster::ZOMBI &&
					m_tColInfo.iColID == static_cast<CMonster*>(pSrc)->Get_InitID())
					static_cast<CMonster*>(pSrc)->Set_IsActiveStart(true);
			}
			m_bIsDead = true;
		}
		else if (m_bIsActive && m_tColInfo.iColID == 10 && !m_bIsGoBoss)
		{
			m_bIsGoBoss = true;
			m_bIsDead = true;
		}
	}
		break;
	case CTrigger::TRIGGER_END:
		break;
	default:
		break;
	}

	return NO_EVENT;
}

void CTrigger::Render_GameObject(const _float & fTimeDelta)
{
//	Set_ConstantTable();

}

HRESULT CTrigger::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	switch (m_eCurShape)
	{
	case CTrigger::TRIGGER_SPHERE:
	{
		m_pShereCol = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, nullptr, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 20.f, _vec3(1.f, 1.f, 1.f), this));
		NULL_CHECK_RETURN(m_pShereCol, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol", m_pShereCol);
	}
		break;
	case CTrigger::TRIGGER_BOX:
	{
		m_pBoxCol = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, false, nullptr, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 100.f, 100.f), this));
		NULL_CHECK_RETURN(m_pBoxCol, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCol);
	}
		break;
	case CTrigger::TRIGGER_END:
		break;
	default:
		break;
	}

	return S_OK;
}

void CTrigger::Set_ConstantTable()
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

CGameObject * CTrigger::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CTrigger(*this);

	COLLIDER tMeshInfo = *reinterpret_cast<COLLIDER*>(prg);
	static_cast<CTrigger*>(pInstance)->m_tColInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CTrigger* CTrigger::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CTrigger* pInstance = new CTrigger(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTrigger::Free()
{

	CGameObject::Free();
}
