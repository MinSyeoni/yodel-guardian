#include "stdafx.h"
#include "ReapeorBullet.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
CReapeorBullet::CReapeorBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CReapeorBullet::CReapeorBullet(const CReapeorBullet& rhs)
	: Engine::CGameObject(rhs)
{

}

CReapeorBullet::~CReapeorBullet()
{
}

HRESULT CReapeorBullet::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CReapeorBullet::Ready_GameObject(REAPERBULLETINFO eInfo)
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Meteor", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif

	m_pBoxCom = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(300.f, 300.f, 300.f), this));
	NULL_CHECK_RETURN(m_pBoxCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCom);

	m_eType = (BUlletType)eInfo.eType;
	m_pTransCom->m_vPos = eInfo.vPos;
	m_pTransCom->m_vDir = eInfo.vDir;
	if (m_eType == FIRSTSHOT)
		m_pTransCom->m_vScale = _vec3(3.0f, 3.0f, 3.0f);
	else
		m_pTransCom->m_vScale = _vec3(1.0f, 1.0f, 1.0f);
	return S_OK;
}

HRESULT CReapeorBullet::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit RectObject");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/


#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());


	m_fAccTime = 0.f;
	return S_OK;
}

_int CReapeorBullet::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 2.0f)
	{
		if (m_eType == FIRSTSHOT)
			CreateSecondBullet();
		m_bIsDead = true;

	}

	if (m_bIsDead)
		return DEAD_OBJ;

	Moving(fTimeDelta);
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCom->Update_Collider(&m_pTransCom->m_matWorld);

	return NO_EVENT;
}

_int CReapeorBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsLateInit)
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCom), -1);

	return NO_EVENT;
}

void CReapeorBullet::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);

}

void CReapeorBullet::SetBulletType(_vec3 vPos, _vec3 vDir, BUlletType eType)
{

	m_eType = eType;
	m_pTransCom->m_vPos = vPos;
	m_pTransCom->m_vDir = vDir;

}

void CReapeorBullet::CreateSecondBullet()
{

	
	REAPERBULLETINFO eInfo;
	for (int i = 0; i < 6 ;++i)
	{
		_vec3 vRandDir = m_pTransCom->m_vDir;
		_vec3 vPos = m_pTransCom->m_vPos;

		vRandDir.y = vRandDir.y - (0.3f) + ((rand() % 6)*0.1f);
		vRandDir.x = vRandDir.x - (0.3f) + ((rand() % 6) * 0.1f);
		vRandDir.z = vRandDir.z - (0.3f) + ((rand() % 6) * 0.1f);
		eInfo.vPos = vPos;
		eInfo.vDir = vRandDir;
		eInfo.vDir.Normalize();
		eInfo.eType = (int)SECONDSHOT;


		m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Meteor", L"Meteor", &eInfo);
	}
}


HRESULT CReapeorBullet::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone ShaderCom");
#endif

	return S_OK;
}

void CReapeorBullet::Set_ConstantTable()
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

void CReapeorBullet::Moving(const float& fTimeDelta)
{
	float fSpeed = 200.f;
	if(m_eType==SECONDSHOT)
		float fSpeed = 350.f;


	m_pTransCom->m_vPos += m_pTransCom->m_vDir * fSpeed *fTimeDelta;

	m_pTransCom->m_vAngle.y += fTimeDelta*50.f;

}

CGameObject* CReapeorBullet::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CReapeorBullet(*this);

	REAPERBULLETINFO eInfo = *reinterpret_cast<REAPERBULLETINFO*>(prg);

	if (FAILED(static_cast<CReapeorBullet*>(pInstance)->Ready_GameObject(eInfo)))
		return nullptr;

	return pInstance;
}

CReapeorBullet* CReapeorBullet::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CReapeorBullet* pInstance = new CReapeorBullet(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CReapeorBullet::Free()
{

	CGameObject::Free();


}
