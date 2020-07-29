#include "stdafx.h"
#include "Medi_Syringe.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "HPKit.h"
#include "InvenUI.h"

CMedi_Syringe::CMedi_Syringe(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CMedi_Syringe::CMedi_Syringe(const CMedi_Syringe& rhs)
	: Engine::CGameObject(rhs)
	, m_tMeshInfo(rhs.m_tMeshInfo)
{
}

CMedi_Syringe::~CMedi_Syringe()
{
}

HRESULT CMedi_Syringe::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CMedi_Syringe::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	return S_OK;
}

HRESULT CMedi_Syringe::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;
}

_int CMedi_Syringe::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCollider);

	//ReScale_SphereCol();

	return NO_EVENT;
}

void CMedi_Syringe::ReScale_SphereCol()
{
	_matrix matTmp = INIT_MATRIX;
	matTmp._11 = 0.01f;	// SCALE
	matTmp._22 = 0.01f;
	matTmp._33 = 0.01f;

	matTmp._41 = m_pTransCom->m_vPos.x;		// POS
	matTmp._42 = m_pTransCom->m_vPos.y;
	matTmp._43 = m_pTransCom->m_vPos.z;

	m_pShereCol->Update_Collider(&matTmp);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pShereCol);
}

_int CMedi_Syringe::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
		return NO_EVENT; 

	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_ALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCollider), -1);
	//FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol), -1);

	Check_ItemAndMouse();

	return NO_EVENT;
}

void CMedi_Syringe::Check_ItemAndMouse()
{
	_uint iMinDist = 999;
	float iDist = 0;
	bool Collision = false;

	list<CGameObject*>* pKitList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"ItemObject");
	if (pKitList != nullptr)
	{
		for (auto& pSrc : *pKitList)
		{
			if (!dynamic_cast<CHPKit*>(pSrc)->Get_IsOpenAndZoom())
				continue;

			for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::OBJECT))
			{
				if (pCol != m_pBoxCollider)
					continue;

				Collision = CMathMgr::Get_Instance()->Collision_BoxWithMousePoint(pCol, g_hWnd, &iDist);
				if ((iDist < iMinDist) && Collision)
				{
					iMinDist = iDist;
					pCol->Set_IsCol(true);
					m_bIsClick = true;
					break;
				}
				else
				{
					pCol->Set_IsCol(false);
					m_bIsClick = false;
				}
			}
		}
	}

	if (m_bIsClick && MOUSE_KEYDOWN(MOUSEBUTTON::DIM_LB))
	{
		CGameObject* pInvenUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
		if (pInvenUI != nullptr)
			dynamic_cast<CInvenUI*>(pInvenUI)->Set_AddItemNum(2, 1);

		m_bIsClick = false;
		m_bIsDead = true;
	}
}

void CMedi_Syringe::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);
}

void CMedi_Syringe::Render_ShadowDepth(CShader_Shadow* pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader);
	pShader->Set_ShadowFinish();
}

HRESULT CMedi_Syringe::Add_Component()
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

	// Box
	m_pBoxCollider = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 100.f, 100.f), this));
	NULL_CHECK_RETURN(m_pBoxCollider, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCollider);

	//Sphere
	//m_pShereCol = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 20.f, _vec3(1.f, 1.f, 1.f), this));
	//NULL_CHECK_RETURN(m_pShereCol, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol", m_pShereCol);

	return S_OK;
}

void CMedi_Syringe::Set_ConstantTable()
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

void CMedi_Syringe::Set_ShadowTable(CShader_Shadow* pShader)
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

CGameObject* CMedi_Syringe::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CMedi_Syringe(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CMedi_Syringe*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CMedi_Syringe* CMedi_Syringe::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CMedi_Syringe* pInstance = new CMedi_Syringe(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMedi_Syringe::Free()
{
	CGameObject::Free();
}
