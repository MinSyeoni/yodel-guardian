#include "stdafx.h"
#include "PlayerArm.h"
#include "GraphicDevice.h"
#include "DirectInput.h"
#include "Frustom.h"
CPlayerArm::CPlayerArm(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CGameObject(pGraphicDevice, pCommandList)
{
}

CPlayerArm::CPlayerArm(const CPlayerArm & rhs)
	:CGameObject(rhs)
	, m_fSpineAngle(rhs.m_fSpineAngle)
{
}

CPlayerArm::~CPlayerArm()
{
}

HRESULT CPlayerArm::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CPlayerArm::Ready_GameObject(ARMTYPE eType)
{
	m_eArmType = eType;
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_fTime = 0.f;

	return S_OK;
}

HRESULT CPlayerArm::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(),m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(),m_pMeshCom->Get_EmissiveTexture());

	return S_OK;

}

_int CPlayerArm::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	m_fTime += fTimeDelta;

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_ePreAnimationKey != m_eCurAnimationKey)
	{
		m_fTime = 0.f;
		m_ePreAnimationKey = m_eCurAnimationKey;
	}


	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)m_eCurAnimationKey);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f*fTimeDelta, CAniCtrl::PLAYER, m_fSpineAngle);
	return NO_EVENT;
}

_int CPlayerArm::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);


	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);



	return NO_EVENT;
}

void CPlayerArm::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

void CPlayerArm::AnimationBlending()
{


	for (int i = 0; i < m_vecMatrix.size(); i++)
	{
		float fAccBody = 0.7f;
		float fAccLeg = 0.3f;
		for (int j = 0; j < m_vecMatrix[i].size(); j++)
		{

			m_vecMatrix[i][j] = (m_vecMatrix[i][j] * fAccBody) + (m_vecLegMatrix[i][j] * fAccLeg);
		}
	}

}

void CPlayerArm::Render_ShadowDepth(CShader_Shadow * pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}



void CPlayerArm::Set_ShadowTable(CShader_Shadow * pShader)
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

HRESULT CPlayerArm::Add_Component()
{

	wstring MeshName;
	/*if (m_eArmType == HEAVY)*/
	MeshName = L"Mesh_Player";
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(MeshName.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);



	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);





	return S_OK;
}

void CPlayerArm::Set_ConstantTable()
{

	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	CB_BONE_INFO	tCB_BoneInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();


	_matrix matWVP = matRotY* m_pTransCom->m_matWorld* matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY*m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);

}

CGameObject * CPlayerArm::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CPlayerArm(*this);

	if (FAILED(dynamic_cast<CPlayerArm*>(pInstance)->Ready_GameObject(CPlayerArm::ARMTYPE::HEAVY)))
		return nullptr;

	return pInstance;
}

CPlayerArm * CPlayerArm::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CPlayerArm* pInstance = new CPlayerArm(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayerArm::Free()
{
	CGameObject::Free();

	Safe_Release(m_pTransCom);


}
