#include "stdafx.h"
#include "Reapear.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"

CReapear::CReapear(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CReapear::CReapear(const CReapear& rhs)
	: Engine::CGameObject(rhs)
{

}

CReapear::~CReapear()
{
}

HRESULT CReapear::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CReapear::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Reapear", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);
#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif

	m_eCurAniState = STATICPOS;
	m_ePreAniState = STATICPOS;

	m_fTime = 0.f;
	return S_OK;
}

HRESULT CReapear::LateInit_GameObject()
{


	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());


	m_pTransCom->m_vPos = _vec3(300.f, -800.f, 700.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);

	m_pTransCom->m_vAngle = _vec3(0.f, 0.f, 0.f);
	return S_OK;

}

_int CReapear::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;
	cout << m_pTransCom->m_vPos.y << endl;
	if (KEY_PRESSING(DIK_X))
		m_pTransCom->m_vPos.y += fTimeDelta * 10.f;


	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_fTime += fTimeDelta;
	return NO_EVENT;
}

_int CReapear::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	m_pMeshCom->Set_Animation((int)m_eCurAniState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(fTimeDelta * 3000.f);
	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/


	return NO_EVENT;
}

void CReapear::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

HRESULT CReapear::Add_Component()
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

void CReapear::Set_ConstantTable()
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



	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(m_fTime * 3000.f);

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CReapear::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CReapear(*this);


	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CReapear* CReapear::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CReapear* pInstance = new CReapear(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CReapear::Free()
{

	CGameObject::Free();
}
