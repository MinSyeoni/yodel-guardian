#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	_vec3	Get_RightVector()		{ _vec3 vRight; memcpy(&vRight, &m_matWorld._11, sizeof(_vec3)); return vRight; };
	_vec3	Get_UpVector()			{ _vec3 vUp; memcpy(&vUp, &m_matWorld._21, sizeof(_vec3)); return vUp; };
	_vec3	Get_LookVector()		{ _vec3 vLook; memcpy(&vLook, &m_matWorld._31, sizeof(_vec3)); return vLook; };
	_vec3	Get_PositionVector()	{ _vec3 vPosition; memcpy(&vPosition, &m_matWorld._41, sizeof(_vec3)); return vPosition; };
public:
	HRESULT			Ready_Component();
	virtual void	Update_Component(const _float& fTimeDelta);

public:
	_vec3	m_vScale	= INIT_VEC3(1.f);
	_vec3	m_vAngle	= INIT_VEC3(0.f);
	_vec3	m_vPos		= INIT_VEC3(0.f);
	_vec3	m_vDir		= INIT_VEC3(0.f);
	_matrix m_matWorld	= INIT_MATRIX;

public:
	virtual CComponent* Clone();
	static CTransform*	Create();
private:
	virtual void		Free();
};

END