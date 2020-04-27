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

	void AddCameraPos(_vec3 vAt , _vec3 vEye);



	vector<	CToolPoint*> m_vecCamAt;
	vector<	CToolPoint*> m_vecCamEye;
	wstring m_strCameraName;


public:
	static CCameraSort*	Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring CameraName);

};

