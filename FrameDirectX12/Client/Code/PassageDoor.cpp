#include "stdafx.h"
#include "PassageDoor.h"
#include "ObjectMgr.h"
#include "DirectInput.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "EquipUI.h"

CPassageDoor::CPassageDoor(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
			:CGameObject(pGraphicDevice,pCommandList)
{
}

CPassageDoor::CPassageDoor(const CPassageDoor& rhs)
			:CGameObject(rhs),
			m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CPassageDoor::~CPassageDoor()
{
}

HRESULT CPassageDoor::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CPassageDoor::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = _vec3(m_tMeshInfo.Pos.x, m_tMeshInfo.Pos.y + 0.5f, m_tMeshInfo.Pos.z + 2.f);
	m_pTransCom->m_vScale = _vec3(0.07f, 0.07f, 0.07f);
	m_pTransCom->m_vAngle = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CPassageDoor::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CPassageDoor::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)	
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

//	if (!m_bIsOpen)
//	{
		m_pBoxCol->Update_Collider(&m_pTransCom->m_matWorld);
		CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCol);
//	}

	list<CGameObject*>* pEquipUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"EquipUI");
	for (auto& pSrc : *pEquipUIList)
		if (CEquipUI::E_DOOROPEN_P == dynamic_cast<CEquipUI*>(pSrc)->Get_EquipType())
			m_pGameObject = dynamic_cast<CEquipUI*>(pSrc);

	OpenTheDoor();

	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)m_eDoorState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);

	return NO_EVENT;
}

void CPassageDoor::OpenTheDoor()
{
	if (!m_bIsCardKey)
		return;

	if (CDirectInput::Get_Instance()->KEY_DOWN(DIK_E) && !m_bIsOpen && m_bIsCollision)
		m_eDoorState = PASSAGE_OPEN;
	else if (!m_bIsCollision && m_bIsOpen)
		m_eDoorState = PASSAGE_CLOSE;
}

_int CPassageDoor::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCol), -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

	PassageDoor_AniState();
	ColiisionTheDoor();

	return NO_EVENT;
}

void CPassageDoor::ColiisionTheDoor()
{
	// 스1과 통로 연결할 곳
	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYER))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCol, pCol, &vShaveDir))
		{
			dynamic_cast<CEquipUI*>(m_pGameObject)->Set_ShowUI(!m_bIsOpen);
			m_bIsCollision = true;
		}
		else
		{
			dynamic_cast<CEquipUI*>(m_pGameObject)->Set_ShowUI(false);
			m_bIsCollision = false;
		}
	}
}

void CPassageDoor::PassageDoor_AniState()
{
	switch (m_eDoorState)
	{
	case CPassageDoor::PASSAGE_IDLE:
	{
		m_bIsOpen = false;
	}
	break;
	case CPassageDoor::PASSAGE_ALREADYOPEN:
	{
		m_bIsOpen = true;
	}
	break;
	case CPassageDoor::PASSAGE_OPEN:
	{
		m_fAniDelay = 4000.f;		
		dynamic_cast<CEquipUI*>(m_pGameObject)->Set_ShowUI(false);
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, PASSAGE_OPEN))
			m_eDoorState = PASSAGE_ALREADYOPEN;
	}
	break;
	case CPassageDoor::PASSAGE_CLOSE:
	{
		m_bIsOpen = false;
		m_fAniDelay = 3000.f;
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, PASSAGE_CLOSE))
			m_eDoorState = PASSAGE_IDLE;
	}
	break;
	default:
		break;
	}
}

void CPassageDoor::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

HRESULT CPassageDoor::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	// BoxCollider
	m_pBoxCol = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 100.f, 100.f), this));
	NULL_CHECK_RETURN(m_pBoxCol, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCol);

	return S_OK;
}

void CPassageDoor::Set_ConstantTable()
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

void CPassageDoor::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}

void CPassageDoor::Set_ShadowTable(CShader_Shadow* pShader)
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

CGameObject * CPassageDoor::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CPassageDoor(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CPassageDoor*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CPassageDoor* CPassageDoor::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CPassageDoor* pInstance = new CPassageDoor(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPassageDoor::Free()
{
	CGameObject::Free();
}
