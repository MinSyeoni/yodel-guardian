#include "stdafx.h"
#include "DynamicCamera.h"
#include "DirectInput.h"

CDynamicCamera::CDynamicCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CCamera(pGraphicDevice, pCommandList)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera & rhs)
	: Engine::CCamera(rhs)
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


	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	/*____________________________________________________________________
	View За·Д Update.
	______________________________________________________________________*/

	if (KEY_PRESSING(DIK_UP))
	{
		m_tCameraInfo.vEye.y += 0.5f;
	}
	if (KEY_PRESSING(DIK_DOWN))
	{
		m_tCameraInfo.vEye.y -= 0.5f;
	}

	if (KEY_PRESSING(DIK_LEFT))
	{
		m_tCameraInfo.vEye.x -= 0.5f;
	}

	if (KEY_PRESSING(DIK_RIGHT))
	{
		m_tCameraInfo.vEye.x += 0.5f;
	}
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	Engine::CCamera::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CDynamicCamera::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return NO_EVENT;
}

void CDynamicCamera::Render_GameObject(const _float & fTimeDelta)
{
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
