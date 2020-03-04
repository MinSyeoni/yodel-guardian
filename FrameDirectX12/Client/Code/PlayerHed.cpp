#include "stdafx.h"
#include "PlayerHed.h"
#include "GraphicDevice.h"
CPlayerHed::CPlayerHed(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CGameObject(pGraphicDevice, pCommandList)
{
}

CPlayerHed::CPlayerHed(const CPlayerHed & rhs)
	: CGameObject(rhs)
{
}

CPlayerHed::~CPlayerHed()
{
}

HRESULT CPlayerHed::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CPlayerHed::Ready_GameObject(HEDTYPE eType)
{
	m_eArmType = eType;
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_fTime = 0.f;
	return S_OK;
}

HRESULT CPlayerHed::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(),m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture());

	return S_OK;
}

_int CPlayerHed::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


	m_fTime += fTimeDelta;
	m_pTransCom->m_vPos = _vec3(50.f, 2.f, 20.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CPlayerHed::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);


	return NO_EVENT;
}

void CPlayerHed::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix,0,false);
}

HRESULT CPlayerHed::Add_Component()
{

	wstring MeshName;
	/*if (m_eArmType == HEAVY)*/
	MeshName = L"Mesh_HedBold";
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(MeshName.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);



	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);





	return S_OK;
}

void CPlayerHed::Set_ConstantTable()
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

	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation(m_iAnimationKey);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(m_fTime*3000.f);

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);

}

CGameObject * CPlayerHed::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CPlayerHed(*this);

	if (FAILED(dynamic_cast<CPlayerHed*>(pInstance)->Ready_GameObject(CPlayerHed::HEDTYPE::BOLD)))
		return nullptr;

	return pInstance;
}

CPlayerHed * CPlayerHed::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CPlayerHed* pInstance = new CPlayerHed(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayerHed::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pMeshCom);
	Engine::Safe_Release(m_pShaderCom);


}
