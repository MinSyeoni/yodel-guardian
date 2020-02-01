#include "stdafx.h"
#include "LogoStart.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"


CLogoStart::CLogoStart(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList, true, true)
{
}

CLogoStart::CLogoStart(const CLogoStart & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pShaderCom(rhs.m_pShaderCom)
	, m_pDynamicCamera(rhs.m_pDynamicCamera)
{
}


CLogoStart::~CLogoStart()
{
}

HRESULT CLogoStart::Ready_GameObjectPrototype(const _vec3 & vScale,
	const _vec3 & vAngle,
	const _vec3 & vPos)
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype LogoStart");
#endif

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vScale = vScale;
	m_pTransCom->m_vAngle = vAngle;
	m_pTransCom->m_vPos = vPos;

	return S_OK;
}


HRESULT CLogoStart::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone LogoStart");
#endif

	return S_OK;
}

HRESULT CLogoStart::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit LogoStart");
#endif

	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/
	m_pDynamicCamera = static_cast<CDynamicCamera*>(m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"DynamicCamera"));
	NULL_CHECK_RETURN(m_pDynamicCamera, E_FAIL);

#ifdef _DEBUG
	COUT_STR("Success Get DynamicCamera");
#endif
	m_pShaderCom->Set_Shader_Texture();

	return S_OK;
}

_int CLogoStart::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;

	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CLogoStart::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/


	return NO_EVENT;
}

void CLogoStart::Render_GameObject(const _float & fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

	CGraphicDevice::Get_Instance()->Wait_ForGpuComplete();
}

HRESULT CLogoStart::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CCubeTex*>(m_pComponentMgr->Clone_Component(L"Prototype_CubeTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);
#ifdef _DEBUG
	COUT_STR("Success LogoStart - Clone BufferCom");
#endif

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_DefaultTex*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_DefaultTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success LogoStart - Clone ShaderCom");
#endif

	return S_OK;
}

void CLogoStart::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	if (nullptr != m_pDynamicCamera)
	{
		matView = m_pDynamicCamera->Get_CameraInfo().matView;
		matProj = m_pDynamicCamera->Get_ProjInfo().matProj;
	}

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject * CLogoStart::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CLogoStart(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CLogoStart * CLogoStart::Create(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList,
	const _vec3 & vScale,
	const _vec3 & vAngle,
	const _vec3 & vPos)
{
	CLogoStart* pInstance = new CLogoStart(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype(vScale, vAngle, vPos)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogoStart::Free()
{
	CGameObject::Free();

	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pShaderCom);
}
