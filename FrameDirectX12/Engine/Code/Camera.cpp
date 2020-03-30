#include "Camera.h"

USING(Engine)

CCamera::CCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CGameObject(pGraphicDevice, pCommandList)
{
	ZeroMemory(&m_tCameraInfo, sizeof(CAMERA_INFO));
	ZeroMemory(&m_tProjInfo, sizeof(PROJ_INFO));
	ZeroMemory(&m_tOrthoInfo, sizeof(ORTHO_INFO));
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_tCameraInfo(rhs.m_tCameraInfo)
	, m_tProjInfo(rhs.m_tProjInfo)
	, m_tOrthoInfo(rhs.m_tOrthoInfo)
{
}


CCamera::~CCamera()
{
}

void CCamera::Set_ProjForV(_float fForV)
{

	m_tProjInfo.fFovY = fForV;
}

HRESULT CCamera::Ready_GameObject(const CAMERA_INFO& tCameraInfo,
								  const PROJ_INFO& tProjInfo,
								  const ORTHO_INFO& tOrthoInfo)
{
	m_tCameraInfo	= tCameraInfo;
	m_tProjInfo		= tProjInfo;
	m_tOrthoInfo	= tOrthoInfo;

	/*____________________________________________________________________
	[ 원근 투영 행렬 ]
	______________________________________________________________________*/
	m_tProjInfo.matProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_tProjInfo.fFovY),
												   m_tProjInfo.fAspect,
												   m_tProjInfo.fNearZ,
												   m_tProjInfo.fFarZ);

	/*____________________________________________________________________
	[ 직교 투영 행렬 ]
	______________________________________________________________________*/
	m_tOrthoInfo.matProj = XMMatrixOrthographicLH(m_tOrthoInfo.fWidth,
												  m_tOrthoInfo.fHeight,
												  m_tOrthoInfo.fNearZ, 
												  m_tOrthoInfo.fFarZ);

	return S_OK;
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{


	/*____________________________________________________________________
	[ 뷰 행렬 Update ]
	______________________________________________________________________*/
	m_tCameraInfo.matView = XMMatrixLookAtLH(m_tCameraInfo.vEye.Get_XMVECTOR(), 
											 m_tCameraInfo.vAt.Get_XMVECTOR(), 
											 m_tCameraInfo.vUp.Get_XMVECTOR());

	m_tProjInfo.matProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_tProjInfo.fFovY),
		m_tProjInfo.fAspect,
		1.f,
		m_tProjInfo.fFarZ);
	return NOEVENT;
}

void CCamera::Free()
{
	CGameObject::Free();
}
