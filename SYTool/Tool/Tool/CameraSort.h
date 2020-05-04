#pragma once

#include "GameObject.h"
#include "Export_Function.h"


class CCameraSort : public Engine::CGameObject
{
private:
	explicit CCameraSort(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraSort(void);

public:
	virtual HRESULT     Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);

	void AddCameraPos(vector<CToolPoint*>vecAtVector, vector<CToolPoint*>vecEyeVector, wstring CameraName, float Time);

	_vec3 Bazior_2(_vec3 Pos1,_vec3 Pos2);
	_vec3 Bazior_3(_vec3 Pos1,_vec3 Pos2,_vec3 Pos3);
	_vec3 Bazior_4(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, _vec3 Pos4);
	
	_vec3 CheckBazior(vector<CToolPoint*> vecToolPoint);

	vector<	CToolPoint*> m_vecCamAt;
	vector<	CToolPoint*> m_vecCamEye;
	wstring m_strCameraName;
	float m_fTime = 0.f;

	float m_fAccTime = 0.f;
	int iIndex = 0;
	virtual void		Free(void);

	_bool m_bIsPlayCamera=false ;
public:
	static CCameraSort*	Create(LPDIRECT3DDEVICE9 pGraphicDev, vector<CToolPoint*>vecAtVector, vector<CToolPoint*>vecEyeVector, wstring CameraName, float Time);

};

