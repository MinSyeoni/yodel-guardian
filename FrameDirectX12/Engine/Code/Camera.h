#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	const CAMERA_INFO&	Get_CameraInfo()	{ return m_tCameraInfo; }
	const PROJ_INFO&	Get_ProjInfo()		{ return m_tProjInfo; }
	const ORTHO_INFO&	Get_OrthInfo()		{ return m_tOrthoInfo; }
public:
	void Set_CameraInfo(const CAMERA_INFO& tCameraInfo) { m_tCameraInfo = tCameraInfo; }
	void Set_Eye(const _vec3& vEye)						{ m_tCameraInfo.vEye = vEye; }
	void Set_At(const _vec3& vAt)						{ m_tCameraInfo.vEye = vAt; }
	void Set_Up(const _vec3& vUp)						{ m_tCameraInfo.vEye = vUp; }
	void Set_ProjInfo(const PROJ_INFO& tProjInfo)		{ m_tProjInfo = tProjInfo; }
	void Set_OrthoInfo(const ORTHO_INFO& tOrthoInfo)	{ m_tOrthoInfo = tOrthoInfo; }
	void Set_ProjForV(_float fForV);
public:
	HRESULT			Ready_GameObject(const CAMERA_INFO& tCameraInfo,
									 const PROJ_INFO& tProjInfo,
									 const ORTHO_INFO& tOrthoInfo);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
private:
	void			Update_CBValue();

protected:
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	CAMERA_INFO	m_tCameraInfo;	// Camera - Eyte, At, Up
	PROJ_INFO	m_tProjInfo;	// 원근 투영 정보를 담은 구조체.
	ORTHO_INFO	m_tOrthoInfo;	// 직교 투영 정보를 담은 구조체.

public:
	virtual CGameObject*	Clone_GameObject(void * parg) PURE;
protected:
	virtual void			Free();
};

END