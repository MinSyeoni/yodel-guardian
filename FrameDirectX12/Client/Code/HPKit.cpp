#include "stdafx.h"
#include "HPKit.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "InvenUI.h"
#include "EquipUI.h"

CHPKit::CHPKit(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CHPKit::CHPKit(const CHPKit& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CHPKit::~CHPKit()
{
}

HRESULT CHPKit::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CHPKit::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);
	m_iMeshID = m_tMeshInfo.iMeshID;

	return S_OK;
}

HRESULT CHPKit::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), 
									m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());
	m_pCamera = static_cast<CDynamicCamera*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));

	return S_OK;
}

_int CHPKit::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCollider);

	Get_EquipUI();

	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)m_eState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);

	return NO_EVENT;
}

void CHPKit::Get_EquipUI()
{
	list<CGameObject*>* pEquipUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"EquipUI");
	for (auto& pSrc : *pEquipUIList)
	{
		if (CEquipUI::E_KITEQUIP == dynamic_cast<CEquipUI*>(pSrc)->Get_EquipType())
		{
			m_pGameObject = dynamic_cast<CEquipUI*>(pSrc);
			break;
		}
	}
}

_int CHPKit::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCollider), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	HPKit_Ani();
	Open_TheKit();

	return NO_EVENT;
}

void CHPKit::HPKit_Ani()
{
	switch (m_eState)
	{
	case CHPKit::KIT_OPEN:
	{
		m_fAniDelay = 1500.f;
		dynamic_cast<CEquipUI*>(m_pGameObject)->Set_ShowUI(false);
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, KIT_OPEN))
			m_eState = KIT_ALREADYOPEN;
	}
	break;
	case CHPKit::KIT_CLOSE:
	{
		m_pCamera->Set_ZoomInOut(false);
		m_fAniDelay = 1500.f;
		dynamic_cast<CEquipUI*>(m_pGameObject)->Set_ShowUI(false);
		if (dynamic_cast<CMesh*>(m_pMeshCom)->Set_FindAnimation(m_fAniDelay, KIT_CLOSE))
			m_eState = KIT_IDLE;
	}
	break;
	case CHPKit::KIT_IDLE:
	{
		m_bIsOpen = false;
	}
		break;
	case CHPKit::KIT_ALREADYOPEN:
	{
		m_bIsOpen = true;

		// 카메라 줌
		m_pCamera->Set_ZoomInOut(true, 10.f);

		// E로 획득 

	}
		break;
	default:
		break;
	}
}

void CHPKit::Open_TheKit()
{
	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::PLAYER))
	{
		if (!m_bIsDead && CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCollider, pCol, &vShaveDir))
		{
			if (!m_bIsOpen && CDirectInput::Get_Instance()->KEY_DOWN(DIK_E))
				m_eState = KIT_OPEN;
			
			else if (m_bIsOpen && CDirectInput::Get_Instance()->KEY_DOWN(DIK_E))
				m_eState = KIT_CLOSE;
			
			// 여기다 HP 상호작용!!!!!!!!!!!!!!!

			// 인벤 연동
			//CGameObject* pInvenUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
			//dynamic_cast<CInvenUI*>(pInvenUI)->Set_AddItemNum(0, 1);
		}
	}
}

void CHPKit::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

void CHPKit::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}

HRESULT CHPKit::Add_Component()
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

void CHPKit::Set_ConstantTable()
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

void CHPKit::Set_ShadowTable(CShader_Shadow * pShader)
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

CGameObject * CHPKit::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CHPKit(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CHPKit*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CHPKit* CHPKit::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CHPKit* pInstance = new CHPKit(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CHPKit::Free()
{
	CGameObject::Free();
}
