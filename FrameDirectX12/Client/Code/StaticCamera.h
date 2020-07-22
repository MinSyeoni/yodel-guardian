#pragma once
#include "Include.h"
#include "Camera.h"
#include "Player.h"

typedef struct tagStaticCameraInfo
{
	_vec3 vAtPos;
	_vec3 vEyePos;
}StaticCameraInfo;


class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CStaticCamera(const CStaticCamera& rhs);
	virtual ~CStaticCamera() = default;

public:
	HRESULT			Ready_GameObjectPrototype();

	virtual HRESULT	Ready_GameObject(StaticCameraInfo tInfo);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
private:
	StaticCameraInfo m_tStaticCamInfo;
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CStaticCamera* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

