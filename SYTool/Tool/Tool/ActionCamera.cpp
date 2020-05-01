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
	
	for (int i = 0; i < m_vecCameraSort.size(); i++)
	{
		m_vecCameraSort[i]->Update_Object(fTimeDelta);

	}

}

void CActionCamera::RenderCamera(void)
{
	if (m_bIsCheckTool == false)
		return;


}

void CActionCamera::DeleteCamera(int Index)
{



	Engine::Safe_Release(m_vecCameraSort[Index]);

	m_vecCameraSort.erase(m_vecCameraSort.begin() + Index);

}

void CActionCamera::AddCamera(vector<CToolPoint*>vecAtVector, vector<CToolPoint*>vecEyeVector, wstring CameraName, float Time)
{
	CCameraSort* pInstance = CCameraSort::Create(CGraphicDev::GetInstance()->GetDevice(), vecAtVector, vecEyeVector, CameraName, Time);
	m_vecCameraSort.push_back(pInstance);
}

void CActionCamera::PlayCamera(int iIndex)
{
	if (m_vecCameraSort.size() < iIndex)
		return;


	m_bIsCheckTool = true;
	CToolCamera::GetInstance()->SetCameraOnOff(false);

	m_vecCameraSort[iIndex]->m_bIsPlayCamera = true;




}

void CActionCamera::FinishCamera()
{
	CToolCamera::GetInstance()->SetCameraOnOff(true);
	m_bIsCheckTool = false;
}


