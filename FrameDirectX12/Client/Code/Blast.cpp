#include "stdafx.h"
#include "Blast.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"
#include "Reapear.h"
#include "LightMgr.h"
#include "ReapeorBullet.h"
CBlast::CBlast(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CBlast::CBlast(const CBlast& rhs)
	: Engine::CGameObject(rhs)
{

}

CBlast::~CBlast()
{
}

HRESULT CBlast::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CBlast::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Blast", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


	CGameObject* pReapear = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Reapear");

	CMesh* pMesh =dynamic_cast<CMesh*>(pReapear->Get_Component(L"Com_Mesh", ID_STATIC));

	m_matBone = pMesh->Get_AnimationComponent()->Get_mouthdiscMatrix();

	m_pReaper =pReapear->Get_Transform();

	m_tagSpotLight.m_eType = LIGHTTYPE::D3DLIGHT_POINT;
	m_tagSpotLight.m_vDiffuse = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLight.m_vAmbient = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLight.m_vSpecular = _vec4{ 1.5f,0.0f,0.0f,1.0f };
	m_tagSpotLight.m_vDirection = _vec4{ 1.0f,1.0f,-1.f,1.0f };
	m_tagSpotLight.m_vPosition = _vec4{ 0.f,10.f,0.f,0.f };
	m_tagSpotLight.m_fRange = 50.f;

	if (FAILED(CLight_Manager::Get_Instance()->Add_Light(m_pGraphicDevice, m_pCommandList, &m_tagSpotLight)))
		return E_FAIL;

	m_uiSpotLightIndex = CLight_Manager::Get_Instance()->Get_LightIndex();
	CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightIndex, true);
	return S_OK;
}

HRESULT CBlast::LateInit_GameObject()
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

	return S_OK;
}

_int CBlast::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	m_fTime += fTimeDelta;
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	_matrix matScale = XMMatrixScaling(4.f, 4.f, 4.f);
	_matrix matRotY = XMMatrixRotationY(ToDegree(60.f));
	m_pTransCom->m_matWorld = matScale * matRotY **m_matBone * m_pReaper->m_matWorld;


	_vec4 vPos;
	memcpy(&vPos, &m_pTransCom->m_matWorld._41, sizeof(_vec3));
		
	vPos.w = 1.f;

	m_tagSpotLight.m_vPosition = vPos;

	CLight_Manager::Get_Instance()->Set_LightInfo(m_uiSpotLightIndex, m_tagSpotLight);


	_vec3 vLook;
	memcpy(&vLook, &m_pTransCom->m_matWorld._31, sizeof(_vec3));
	vLook.Normalize();


	if (m_fTime > 7.f)
		m_bIsDead = true;

	if (m_bIsDead)
	{
		CLight_Manager::Get_Instance()->Set_LightOnOff(m_uiSpotLightIndex, false);
		CreateMeteor();
		return DEAD_OBJ;

	}


	return NO_EVENT;
}

_int CBlast::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsLateInit)
		return NO_EVENT;
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_ALPHA, this), -1);


	return NO_EVENT;
}

void CBlast::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom);

}

void CBlast::CreateMeteor()
{

	REAPERBULLETINFO eInfo; 

	_vec3 vPos;
	memcpy(&vPos, &m_pTransCom->m_matWorld._41, sizeof(_vec3));


	eInfo.vPos = vPos;
	eInfo.eType = (int)CReapeorBullet::FIRSTSHOT;

	_vec3 vPlayerPos = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player")->Get_Transform()->m_vPos;
	vPlayerPos.y += 20.f;

	eInfo.vDir =  vPlayerPos-vPos;
	eInfo.vDir.Normalize();



	m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Meteor", L"Meteor",&eInfo);

}


HRESULT CBlast::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UvAni", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone ShaderCom");
#endif

	return S_OK;
}

void CBlast::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));


	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();


	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	_matrix matTemp = INIT_MATRIX;
	matTemp._11 = 0.f;
	matTemp._12 = m_fTime;

	matTemp._21 = 0.8f;
	matTemp._22 = 0.0f;
	matTemp._23 = 0.0f;
	matTemp._24 = 1.0f;

	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matTemp));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

void CBlast::Set_ShadowTable(CShader_Shadow* pShader)
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

CGameObject* CBlast::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CBlast(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CBlast* CBlast::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CBlast* pInstance = new CBlast(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBlast::Free()
{

	CGameObject::Free();


}
