#include"pch.h"
#include "CameraSort.h"

CCameraSort::CCameraSort(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCameraSort::~CCameraSort(void)
{

}

HRESULT  CCameraSort:: Ready_Object()
{


	return S_OK;
}

_int CCameraSort::Update_Object(const _float& fTimeDelta)
{
	return 1;


}

void CCameraSort:: Render_Object(void)
{



}
void CCameraSort::AddCameraPos(_vec3 vAt, _vec3 vEye)
{

}

void CCameraSort::Free(void)
{
	f\
}
