#include "pch.h"
#include "ActionCamera.h"


IMPLEMENT_SINGLETON(CActionCamera)

CActionCamera::CActionCamera(void)
{

}

CActionCamera::~CActionCamera(void)
{


	for (int i = 0; i < m_vecCameraSort.size(); i++)
	{
		Engine::Safe_Release(m_vecCameraSort[i]);
	}


}

void CActionCamera::UpdateActionCamera(const _float & fTimeDelta)
{
	if (m_bIsCheckTool == false)
		return;



}

void CActionCamera::RenderCamera(void)
{
	if (m_bIsCheckTool == false)
		return;


}

void CActionCamera::AddCamera()
{


}


