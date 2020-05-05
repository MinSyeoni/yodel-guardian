#include "stdafx.h"
#include "ActionCamera.h"
#include "BaziorMgr.h"
#include "GraphicDevice.h"
CActionCamera::CActionCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CCamera(pGraphicDevice,pCommandList)
{
}

CActionCamera::CActionCamera(const CActionCamera& rhs)
	: Engine::CCamera(rhs)
	,m_vecAt(rhs.m_vecAt),
	m_vecEye(rhs.m_vecEye),
	m_CameraName(rhs.m_CameraName),
	m_fTime(rhs.m_fTime)
{
}

HRESULT CActionCamera::Ready_GameObjectPrototype(vector<_vec3> vecEye, vector<_vec3> vecAt, wstring CameraName, float fTime)
{
	m_vecAt = vecAt;
	m_vecEye =vecEye;

	m_CameraName = CameraName;
	m_fTime = fTime;


	return S_OK;
}

HRESULT CActionCamera::Ready_GameObject()
{
	m_fAccTime = 0.f;

	m_tProjInfo = PROJ_INFO(60.f,							// FovY
		_float(WINCX) / _float(WINCY),	// Aspect
		1.f,							// Near
		500.f);


		// Far
    m_tOrthoInfo=ORTHO_INFO(WINCX,	// Viewport Width
			WINCY,	// Viewport Height
			0.f,	// Near
			1.f);

	return NO_EVENT;
}

HRESULT CActionCamera::LateInit_GameObject()
{
	return NO_EVENT;
}

_int CActionCamera::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta * (1.f / m_fTime);

	m_tCameraInfo.vEye = CheckBazior(m_vecEye);
	m_tCameraInfo.vAt = CheckBazior(m_vecAt);
	m_tCameraInfo.vUp = _vec3(0.f, 1.f, 0.f);

	if (m_fAccTime > 1.0f)
	{
		return DEADOBJ;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	Engine::CCamera::Update_GameObject(fTimeDelta);

	CGraphicDevice::Get_Instance()->SetViewMatrix(m_tCameraInfo.matView);
	CGraphicDevice::Get_Instance()->SetProjMatrix(m_tProjInfo.matProj);


	return NO_EVENT;
}

_int CActionCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return NO_EVENT;
}

void CActionCamera::Render_GameObject(const _float& fTimeDelta)
{
}

_vec3 CActionCamera::CheckBazior(vector<_vec3> vecPoint)
{
	_vec3 Temp;
	int iSize = vecPoint.size();

	if (iSize == 1)
		return vecPoint[0];
	else if (iSize == 2)
		return BaziorMgr::Bazior2(vecPoint[0], vecPoint[1], m_fAccTime);
	else if (iSize == 3)
		return BaziorMgr::Bazior3(vecPoint[0], vecPoint[1], vecPoint[2], m_fAccTime);
	else if (iSize == 4)
		return BaziorMgr::Bazior4(vecPoint[0], vecPoint[1], vecPoint[2], vecPoint[3], m_fAccTime);


}

CGameObject* CActionCamera::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CActionCamera(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CActionCamera* CActionCamera::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, vector<_vec3> vecEye, vector<_vec3> vecAt, wstring CameraName, float fTime)
{
	CActionCamera* pInstnace = new CActionCamera(pGraphicDevice, pCommandList);

	if (FAILED(pInstnace->Ready_GameObjectPrototype(vecEye, vecAt, CameraName, fTime)))
		Engine::Safe_Release(pInstnace);

	return pInstnace;
}

void CActionCamera::Free()
{
	CCamera::Free();
}
