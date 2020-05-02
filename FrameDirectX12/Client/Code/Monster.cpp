#include "stdafx.h"
#include "Monster.h"

#include "ObjectMgr.h"
#include "DirectInput.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"


CMonster::CMonster(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CGameObject(pGraphicDevice,pCommandList)
{
}

CMonster::CMonster(const CMonster& rhs)
	:CGameObject(rhs),
	m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype Monster");
#endif

	return S_OK;
}

HRESULT CMonster::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

#ifdef _DEBUG
	COUT_STR("Success Monster - Clone Mesh");
#endif

	if(m_tMeshInfo.MeshTag == L"Flamethrower")
		m_eMonName = FLAMETHROWER;
	else if (m_tMeshInfo.MeshTag == L"Zombi")
		m_eMonName = ZOMBI;

	FAILED_CHECK_RETURN(Add_Component(),E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos + _vec3{300.f, 0.f, 300.f};
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vDir = _vec3(-1.f, 0.f, 1.f);

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
		m_pFlameThrower = new CFlameThrower;
		m_pFlameThrower->Set_Transform(m_pTransCom);
		break;
	case CMonster::ZOMBI:
		m_pZombiState = new CZombiState;
		m_pZombiState->Set_Transform(m_pTransCom);
		m_pZombiState->Set_NaviMesh(m_pNaviMesh);
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit Monster");
#endif
	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
		m_pFlameThrower->Late_Initialized();
		break;
	case CMonster::ZOMBI:
		m_pZombiState->Late_Initialized();
		break;
	default:
		break;
	}

	return S_OK;
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCom->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCom);

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
	{
		if (m_pFlameThrower == nullptr)
			return E_FAIL;

		m_pFlameThrower->Update_FlameThrower(fTimeDelta, m_pTransCom, m_pMeshCom);
		m_iCurMonState = m_pFlameThrower->Get_CurState();
		m_iPreMonState = m_pFlameThrower->Get_PreState();
	}
		break;
	case CMonster::ZOMBI:
	{
		if (m_pZombiState == nullptr)
			return E_FAIL;

		m_pZombiState->Update_Zombi(fTimeDelta, m_pTransCom, m_pMeshCom);
		m_iCurMonState = m_pZombiState->Get_CurState();
		m_iPreMonState = m_pZombiState->Get_PreState();
	}
		break;
	default:
		break;
	}

	dynamic_cast<CMesh*>(m_pMeshCom)->Set_AnimationBlend((_int)m_iCurMonState, (_int)m_iPreMonState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransformsBlend(5000.f * fTimeDelta, 5000.f * fTimeDelta);

	return NO_EVENT;
}

_int CMonster::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCom), -1);

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
		if (m_pFlameThrower == nullptr)
			return E_FAIL;
		m_pFlameThrower->LateUpdate_FlameThrower(fTimeDelta, m_pTransCom, m_pMeshCom);
		break;
	case CMonster::ZOMBI:
		if (m_pZombiState == nullptr)
			return E_FAIL;
		m_pZombiState->LateUpdate_Zombi(fTimeDelta, m_pTransCom, m_pMeshCom);
		break;
	default:
		break;
	}

	return NO_EVENT;
}

void CMonster::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

HRESULT CMonster::Add_Component()
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

	m_pNaviMesh = static_cast<Engine::CNaviMesh*>(CComponentMgr::Get_Instance()->Clone_Component(L"Mesh_Navi", ID_STATIC));
	NULL_CHECK_RETURN(m_pNaviMesh, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_NaviMesh", m_pNaviMesh);

	m_pBoxCom = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(300.f, 300.f, 300.f), this));
	NULL_CHECK_RETURN(m_pBoxCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCom);
	return S_OK;
}

void CMonster::Set_ConstantTable()
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

void CMonster::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader);
	pShader->Set_ShadowFinish();
}

void CMonster::Set_ShadowTable(CShader_Shadow* pShader)
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

void CMonster::Update_BoundaryBox()
{
	_matrix matBoundary = INIT_MATRIX;
	_matrix matScale = INIT_MATRIX;

	matBoundary = m_pTransCom->m_matWorld;

	matBoundary._11 = 0.2f;
	matBoundary._33 = 0.2f;

	m_pBoundary->Update_Collider(&matBoundary);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoundary);
}

CGameObject * CMonster::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CMonster(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CMonster*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CMonster* CMonster::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CMonster* pInstance = new CMonster(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free()
{
	CGameObject::Free();
	Safe_Delete(m_pFlameThrower);
	Safe_Delete(m_pZombiState);
}
