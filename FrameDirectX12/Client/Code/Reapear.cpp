#include "stdafx.h"
#include "Reapear.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"
#include "LightMgr.h"
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

	m_tagSpotLightEye.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
	m_tagSpotLightEye.m_vDiffuse = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightEye.m_vAmbient = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightEye.m_vSpecular = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightEye.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
	m_tagSpotLightEye.m_vPosition = _vec4{ 300.f,10.f,300.f,0.f };
	m_tagSpotLightEye.m_fRange = 50.f;

	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagSpotLightEye)))
		return E_FAIL;

	m_uiSpotLightLeftEyeIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
	CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightLeftEyeIndex, true);

	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagSpotLightEye)))
		return E_FAIL;

	m_uiSpotLightRightEyeIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
	CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightRightEyeIndex, true);


	m_tagSpotLightBody.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
	m_tagSpotLightBody.m_vDiffuse = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightBody.m_vAmbient = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightBody.m_vSpecular = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLightBody.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
	m_tagSpotLightBody.m_vPosition = _vec4{ 300.f,10.f,300.f,0.f };
	m_tagSpotLightBody.m_fRange = 70.f;

	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagSpotLightBody)))
		return E_FAIL;

	m_uiSpotLightBodyIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
	CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightBodyIndex, true);



	m_pRightEyeBone = m_pMeshCom->Find_BoneMatrix("eyeRight");
	m_pRightEyeOffset = m_pMeshCom->Find_BoneOffset("eyeRight");

	m_pLeftEyeBone = m_pMeshCom->Find_BoneMatrix("eyeLeft");
	m_pLeftEyeOffset = m_pMeshCom->Find_BoneOffset("eyeLeft");

	m_pBodyBone = m_pMeshCom->Get_AnimationComponent()->Get_ChestdiscMatrix();
	

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

	UpdateLight(fTimeDelta);


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
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_LIMLIGHT, this), -1);
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

void CReapear::Render_LimLight(CShader_LimLight* pShader)
{

	Set_LimTable(pShader);
	m_pMeshCom->Render_LimMesh(pShader, m_vecMatrix, true);
	pShader->Set_LimFinish();
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

void CReapear::Set_LimTable(CShader_LimLight* pShader)
{

	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	_matrix matWorld = m_pTransCom->m_matWorld;
	matWorld._11 = 0.1005f;
	matWorld._22 = 0.1005f;
	matWorld._33 = 0.1005f;
	_matrix matWVP =  matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose( matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));



	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_MatrixInfo()->CopyData(offset, tCB_MatrixInfo);
}

void CReapear::UpdateLight(const _float& fTimeDelta)
{
	_matrix matBlend;


	_vec4 vLeftEyePos;
	matBlend = XMMatrixInverse(nullptr, *m_pLeftEyeOffset);

	_matrix matLeftEye = matBlend * *m_pLeftEyeBone * m_pTransCom->m_matWorld;
	memcpy(&vLeftEyePos, &matLeftEye._41, sizeof(_vec3));

	vLeftEyePos.w = 1.f;

	m_tagSpotLightEye.m_vPosition = vLeftEyePos;

	CLight_Manager::Get_Instance()->Set_LightInfo(m_uiSpotLightLeftEyeIndex, m_tagSpotLightEye);





	_vec4 vRightEyePos;
	matBlend = XMMatrixInverse(nullptr, *m_pRightEyeOffset);

	_matrix matRightEye = matBlend * *m_pRightEyeBone * m_pTransCom->m_matWorld;
	memcpy(&vRightEyePos, &matRightEye._41, sizeof(_vec3));

	vRightEyePos.w = 1.f;

	m_tagSpotLightEye.m_vPosition = vRightEyePos;

	CLight_Manager::Get_Instance()->Set_LightInfo(m_uiSpotLightRightEyeIndex, m_tagSpotLightEye);



	_vec4 vBodyPos;


	_matrix matBody = *m_pBodyBone * m_pTransCom->m_matWorld;
	memcpy(&vBodyPos, &matBody._41, sizeof(_vec3));

	vBodyPos.w = 1.f;

	m_tagSpotLightBody.m_vPosition = vBodyPos;

	CLight_Manager::Get_Instance()->Set_LightInfo(m_uiSpotLightBodyIndex, m_tagSpotLightBody);




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
