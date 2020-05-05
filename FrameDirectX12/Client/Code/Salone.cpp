#include "stdafx.h"
#include "Salone.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
#include"Frustom.h"
CSalone::CSalone(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CSalone::CSalone(const CSalone & rhs)
	: Engine::CGameObject(rhs)
{

}

CSalone::~CSalone()
{
}

HRESULT CSalone::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CSalone::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Salone", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	m_pShereCol = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 10.f/*여기반지름*/, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pShereCol, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol", m_pShereCol);
	//여기야시영

#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_pTransCom->m_vPos = _vec3(300.f, 0.f, 300.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vAngle =  _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CSalone::LateInit_GameObject()
{


	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());

	return S_OK;

}

_int CSalone::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	_matrix matBone = *m_pMeshCom->Find_BoneMatrix("Chest");//이거포인터라서 한번만 받아도되시영 예시는 그냥 값만받앗어
	_matrix matBoneOffset = *m_pMeshCom->Find_BoneOffset("Chest");

	_matrix Offset = XMMatrixInverse(nullptr, matBoneOffset);

	_matrix BoneMatrix = Offset * matBone * m_pTransCom->m_matWorld;

	m_pShereCol->Update_Collider(&BoneMatrix);//여기
	m_pShereCol->Set_IsCol(true);

	return NO_EVENT;
}

_int CSalone::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/
	m_pMeshCom->Set_Animation((int)m_eCurState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(fTimeDelta*3000.f);

	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol), -1);
	return NO_EVENT;
}

void CSalone::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

void CSalone::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}

HRESULT CSalone::Add_Component()
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

void CSalone::Set_ConstantTable()
{
	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();


	_matrix matWVP = matRotY * m_pTransCom->m_matWorld* matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY*m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);

}
void CSalone::Set_ShadowTable(CShader_Shadow * pShader)
{
	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_SHADOW_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_SHADOW_INFO));

	matView = CFrustom::Get_Instance()->Get_LightView();
	matProj = CFrustom::Get_Instance()->Get_LightProj();

	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY*m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	tCB_MatrixInfo.blsMesh = true;


	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);

}
CGameObject * CSalone::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CSalone(*this);


	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CSalone * CSalone::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CSalone* pInstance = new CSalone(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSalone::Free()
{
	CGameObject::Free();
}
