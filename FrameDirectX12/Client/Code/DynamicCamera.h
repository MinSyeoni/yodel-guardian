#pragma once
#include "Include.h"
#include "Camera.h"

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera();

public:
	HRESULT			Ready_GameObjectPrototype(const CAMERA_INFO& tCameraInfo,
											  const PROJ_INFO& tProjInfo,
											  const ORTHO_INFO& tOrthoInfo);
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

private:
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/


public:
	virtual CGameObject*	Clone_GameObject(void* pArg);
	static CDynamicCamera*	Create(ID3D12Device* pGraphicDevice,
								   ID3D12GraphicsCommandList* pCommandList,
								   const CAMERA_INFO& tCameraInfo,
								   const PROJ_INFO& tProjInfo,
								   const ORTHO_INFO& tOrthoInfo);
private:
	virtual void			Free();
};

