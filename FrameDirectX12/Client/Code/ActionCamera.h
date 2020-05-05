#pragma once
#include "Include.h"
#include "Camera.h"
#include "Player.h"
class CActionCamera : public Engine::CCamera
{
private:
	explicit CActionCamera(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CActionCamera(const CActionCamera& rhs);
	virtual ~CActionCamera()=default;

public:
	HRESULT			Ready_GameObjectPrototype(vector<_vec3>vecEye, vector<_vec3>vecAt, wstring CameraName, float fTime);

	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
private:
	_vec3 CheckBazior(vector<_vec3> vecPoint);
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	vector<_vec3>m_vecEye;
	vector<_vec3>m_vecAt;


	float m_fTime = 0.f;
	float m_fAccTime = 0.f;
	wstring m_CameraName;
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CActionCamera* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList,
		vector<_vec3>vecEye,vector<_vec3>vecAt,wstring CameraName,float fTime);
private:
	virtual void			Free();
};

