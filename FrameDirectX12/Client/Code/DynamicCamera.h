#pragma once
#include "Include.h"
#include "Camera.h"
#include "Player.h"
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
	void MouseInput();
private:
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	CPlayer* m_pPlayer = nullptr;
	_matrix* m_pPlayerMatrix = nullptr;
	CPlayerArm* m_pPlayerArm = nullptr;
	CPlayerLeg* m_pPlayerLeg = nullptr;

	_vec3 m_vDir = _vec3{ 0.0f,0.0f,-1.0f };

	_float m_fViewZ = 20.f;
	_float fRigth = 0.f;
	_float fUp = 0.f;
	_float fLook = 0.f;
	
	_matrix* m_pmatArmCamera = nullptr;
	_matrix* m_pmatLegCamera = nullptr;
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

