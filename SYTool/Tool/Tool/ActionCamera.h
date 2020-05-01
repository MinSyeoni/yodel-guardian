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
	void DeleteCamera(int Index);
	void AddCamera(vector<CToolPoint*>vecAtVector,vector<CToolPoint*>vecEyeVector,wstring CameraName,float Time);
	void PlayCamera(int iIndex);
	void FinishCamera();

	bool m_bIsCheckTool = false;

	vector<CCameraSort*> m_vecCameraSort;
};
