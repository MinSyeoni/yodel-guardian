#include "stdafx.h"
#include "StaticCamera.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"
CStaticCamera::CStaticCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CCamera(pGraphicDevice,pCommandList)
{
}

CStaticCamera::CStaticCamera(const CStaticCamera& rhs)
	: Engine::CCamera(rhs)
{

}

HRESULT CStaticCamera::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CStaticCamera::Ready_GameObject(StaticCameraInfo tInfo)
{
	m_tStaticCamInfo = tInfo;


	if (nullptr != CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_Camera", L"DynamicCamera"))
		CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_Camera", L"DynamicCamera")->Dead_GameObject();

	m_tProjInfo = PROJ_INFO(60.f,							// FovY
		_float(WINCX) / _float(WINCY),	// Aspect
		1.f,							// Near
		500.f);


	return S_OK;
}

HRESULT CStaticCamera::LateInit_GameObject()
{
	return S_OK;
}

_int CStaticCamera::Update_GameObject(const _float& fTimeDelta)
{

	m_tCameraInfo.vAt = m_tStaticCamInfo.vAtPos;
	m_tCameraInfo.vEye = m_tStaticCamInfo.vEyePos;
	m_tCameraInfo.vUp = _vec3(0.f, 1.f, 0.f);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	Engine::CCamera::Update_GameObject(fTimeDelta);

	CGraphicDevice::Get_Instance()->SetViewMatrix(m_tCameraInfo.matView);
	CGraphicDevice::Get_Instance()->SetProjMatrix(m_tProjInfo.matProj);


	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

_int CStaticCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return NO_EVENT;
}

void CStaticCamera::Render_GameObject(const _float& fTimeDelta)
{
}

CGameObject* CStaticCamera::Clone_GameObject(void* pArg)
{


	CGameObject* pInstance = new CStaticCamera(*this);
	StaticCameraInfo tInfo = *reinterpret_cast<StaticCameraInfo*>(pArg);

	if (FAILED(static_cast<CStaticCamera*>(pInstance)->Ready_GameObject(tInfo)))
		return nullptr;


	return pInstance;
}

CStaticCamera* CStaticCamera::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CStaticCamera* pInstance = new CStaticCamera(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void CStaticCamera::Free()
{
	CCamera::Free();

}
