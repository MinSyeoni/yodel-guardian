#include "stdafx.h"
#include "DynamicCamera.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
CDynamicCamera::CDynamicCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CCamera(pGraphicDevice, pCommandList)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera & rhs)
	: Engine::CCamera(rhs)
	,m_fViewZ(rhs.m_fViewZ)
	,m_vDir(rhs.m_vDir)
{
}


CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObjectPrototype(const CAMERA_INFO& tCameraInfo,
												  const PROJ_INFO& tProjInfo,
												  const ORTHO_INFO& tOrthoInfo)
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype DynamicCamera");
#endif

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(tCameraInfo, tProjInfo, tOrthoInfo), E_FAIL);

	return S_OK;
}

HRESULT CDynamicCamera::Ready_GameObject()
{
#ifdef _DEBUG
	COUT_STR("Ready Clone DynamicCamera");
#endif

	return S_OK;
}

HRESULT CDynamicCamera::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit DynamicCamera");
#endif
	if (nullptr != CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player"))
		m_pPlayer = static_cast<CPlayer*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player"));



	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	/*____________________________________________________________________
	View За·Д Update.
	______________________________________________________________________*/

	MouseInput();
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	Engine::CCamera::Update_GameObject(fTimeDelta);

	CGraphicDevice::Get_Instance()->SetViewMatrix(m_tCameraInfo.matView);
	CGraphicDevice::Get_Instance()->SetProjMatrix(m_tProjInfo.matProj);

	return NO_EVENT;
}

_int CDynamicCamera::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return NO_EVENT;
}

void CDynamicCamera::Render_GameObject(const _float & fTimeDelta)
{
}

void CDynamicCamera::MouseInput()
{
	
	
	_long dwMouseMove;

	if (dwMouseMove = CDirectInput::Get_Instance()->Get_DIMouseMove(CDirectInput::DIMM_X))
	{

		_vec3 vUp = _vec3{ 0.f,1.f,0.f };
		_matrix matRot;

		matRot = XMMatrixRotationAxis(vUp.Get_XMVECTOR(), XMConvertToRadians(dwMouseMove / 10.f));
		m_vDir.TransformNormal(m_vDir, matRot);
		m_vDir.Normalize();
	}



	if (m_pPlayer != nullptr)
	{
		CTransform* pTransfrom = nullptr;
		pTransfrom = static_cast<CTransform*> (m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));
		if (pTransfrom != nullptr)
		{
			m_tCameraInfo.vAt = pTransfrom->m_vPos;

			m_tCameraInfo.vEye =m_vDir *m_fViewZ + m_tCameraInfo.vAt;
			m_tCameraInfo.vAt.y += 16.f;
			m_tCameraInfo.vEye.y += 17.0f;
		}
	}

}

CGameObject * CDynamicCamera::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CDynamicCamera(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CDynamicCamera * CDynamicCamera::Create(ID3D12Device * pGraphicDevice, 
										ID3D12GraphicsCommandList* pCommandList,
										const CAMERA_INFO& tCameraInfo,
										const PROJ_INFO& tProjInfo,
										const ORTHO_INFO& tOrthoInfo)
{
	CDynamicCamera* pInstnace = new CDynamicCamera(pGraphicDevice, pCommandList);

	if (FAILED(pInstnace->Ready_GameObjectPrototype(tCameraInfo, tProjInfo, tOrthoInfo)))
		Engine::Safe_Release(pInstnace);

	return pInstnace;
}

void CDynamicCamera::Free()
{
	CCamera::Free();
}
