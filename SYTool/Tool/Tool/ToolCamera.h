#pragma once

#include "Export_Function.h"

class CToolCamera
{
	DECLARE_SINGLETON(CToolCamera)

private:
	explicit CToolCamera(void);
	~CToolCamera(void);

public:
	void			Initialize_Camera();
	void			Update_Camera(const _float& fTimeDelta);
	void			Move_Camera(_vec3 vDir);

	_vec3&			Get_CameraPos();
	void			Set_CameraPos(_vec3* pPos);

	void			Rotation_Axis(const _vec3* pAxis, const _float& fRadian, const _float& fTimeDelta);

	void			Camera_Pitch(_float fAngle);
	void			Camera_Yaw(_float fAngle);
	void			Camera_Roll(_float fAngle);

	_matrix&		Change_CameraMat(_matrix* pMatView);
	void			SetGrapicDevice(LPDIRECT3DDEVICE9 pDev) { m_pGraphicDev = pDev; };

	_matrix			Get_WorldMatrix() { return m_matWorld; };

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	_vec3					m_vPos;
	_vec3					m_vRight;
	_vec3					m_vUp;
	_vec3					m_vLook;
	_vec3					m_vAngle;

	_matrix					m_matWorld;
	_matrix					m_matView;
	_matrix					m_matProj;
};

