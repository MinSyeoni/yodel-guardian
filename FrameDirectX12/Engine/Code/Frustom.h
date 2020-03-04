#pragma once

#include "Base.h"

BEGIN(Engine)
//Plane 은 x 부터 상단 위 시계방향
class ENGINE_DLL CFrustom :public CBase
{
	DECLARE_SINGLETON(CFrustom)

private:
	explicit CFrustom();
	virtual ~CFrustom() = default;

public:
	_matrix Get_LightProj() { return m_matProj; };
	_matrix Get_LightView() { return m_matView; };
	_vec3* Get_Point() { return m_vPoint; };

	HRESULT Update_Frustom_Manager();
	HRESULT Ready_Light_Frustom_Manager();

private:
	_matrix m_matProj;
	_matrix m_matView;


	_vec3   m_vOriPoint[8];
	_vec3   m_vPoint[8];
	_vec3   m_vLightDir;
	_vec3   m_vMinPoint;
	_vec3   m_vMaxPoint;
	_vec3   m_vCenPos;
	_vec3   m_vLightPos;

	_vec4  m_vPlane[6];

	_float  m_fRadius;
	 
protected:
	virtual void Free(void);

};
END
