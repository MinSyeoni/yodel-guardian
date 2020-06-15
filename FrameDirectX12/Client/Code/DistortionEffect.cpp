#include "stdafx.h"
#include "DistortionEffect.h"
#include "GraphicDevice.h"
CDistortionEffect::CDistortionEffect(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CDistortionEffect::CDistortionEffect(const CDistortionEffect& rhs)
	: Engine::CGameObject(rhs)
{

}



CDistortionEffect::~CDistortionEffect()
{
}

HRESULT CDistortionEffect::Ready_GameObjectPrototype()
{




	return S_OK;
}

HRESULT CDistortionEffect::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = m_tMeshInfo.Scale;
	m_pTransCom->m_vAngle = m_tMeshInfo.Rotation;




	return S_OK;
}

HRESULT CDistortionEffect::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit RectObject");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/


	m_pDistortShaderCom->Set_Shader_Texture(m_pMeshCom->Get_NormalTexture());
	return S_OK;
}

_int CDistortionEffect::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;
	m_fTime += fTimeDelta *5.f;

	

	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	BillBoard();

	return NO_EVENT;
}

_int CDistortionEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{

	if (!m_bIsLateInit)
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_DESTORTION, this), -1);
	return NO_EVENT;
}

void CDistortionEffect::Render_GameObject(const _float& fTimeDelta)
{
	
	return;

}

void CDistortionEffect::Render_Distortion(const _float& fTimeDelta)
{
	Set_ConstantTableDistort();

	m_pDistortShaderCom->Begin_Shader();

	m_pMeshCom->Render_Destortion(m_pDistortShaderCom);
}

void CDistortionEffect::BillBoard()
{
	_matrix matBill, matView;

	matBill = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matView = matBill;
	ZeroMemory(&matBill.m[3][0], sizeof(_vec3));


	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill = XMMatrixInverse(nullptr, matBill);


	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;



}



HRESULT CDistortionEffect::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);


	// Shader
	m_pDistortShaderCom = static_cast<Engine::CShader_Destortion*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Destortion", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pDistortShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pDistortShaderCom);


	return S_OK;
}

void CDistortionEffect::Set_ConstantTableDistort()
{
	
	_matrix matRotZ = XMMatrixRotationX(ToRadian(90));
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

	m_pDistortShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);


	CB_DISTORT_INFO tCB_DistortInfo;
	ZeroMemory(&tCB_DistortInfo, sizeof(CB_DISTORT_INFO));
	tCB_DistortInfo.DistorScale = _vec4(m_fTime, 0.f, 0.f, 1.f);

	m_pDistortShaderCom->Get_UploadBuffer_DistortInfo()->CopyData(0, tCB_DistortInfo);
}

void CDistortionEffect::Set_ConstantTable()
{
	


}



CGameObject* CDistortionEffect::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CDistortionEffect(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CDistortionEffect*>(pInstance)->SetMeshInfo(tMeshInfo);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CDistortionEffect* CDistortionEffect::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CDistortionEffect* pInstance = new CDistortionEffect(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDistortionEffect::Free()
{

	CGameObject::Free();


}
