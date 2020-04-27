#pragma once

#include "Export_Function.h"
#include "CameraSort.h"
class CActionCamera
{
	DECLARE_SINGLETON(CActionCamera)

private:
	explicit CActionCamera(void);
	~CActionCamera(void);

public:
	void UpdateActionCamera(const _float& fTimeDelta);
	void RenderCamera(void);

	void AddCamera();

	

	bool m_bIsCheckTool = false;

	vector<CCameraSort*> m_vecCameraSort;
};
