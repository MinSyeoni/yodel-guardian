#include"pch.h"
#include "CameraSort.h"
#include "ActionCamera.h"
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
	if (m_bIsPlayCamera == true)
	{
		m_fAccTime += fTimeDelta * (1.f / m_fTime);

		_vec3 CamEye =CheckBazior(m_vecCamEye);
		_vec3 CamAt = CheckBazior(m_vecCamAt);
		_vec3 CamUp = { 0.f,1.f,0.f };
		if (m_fAccTime >= 1.f)
		{
			CActionCamera::GetInstance()->FinishCamera();
			m_bIsPlayCamera = false;
		m_fAccTime = 0.f;
		}

		_matrix matView;
		D3DXMatrixLookAtLH(&matView, &CamEye,& CamAt, &CamUp);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	}
	

	return 1;
}

void CCameraSort:: Render_Object(void)
{



}
void CCameraSort::AddCameraPos(vector<CToolPoint*>vecAtVector, vector<CToolPoint*>vecEyeVector, wstring CameraName, float Time)
{
	m_vecCamAt = vecAtVector;
	m_vecCamEye = vecEyeVector;
	m_strCameraName = CameraName;
	m_fTime = Time;

}

_vec3 CCameraSort::Bazior_2(_vec3 Pos1, _vec3 Pos2)
{
	_vec3 Temp  = (1 - m_fAccTime)*Pos1 + m_fAccTime * Pos2;

	return Temp;
}

_vec3 CCameraSort::Bazior_3(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3)
{
	_vec3 Temp = ((1.f - m_fAccTime)*(1.f - m_fAccTime))*Pos1 + (2.f*m_fAccTime)*(1.f - m_fAccTime)*Pos2 + (m_fAccTime*m_fAccTime*Pos3);

	return Temp;
}

_vec3 CCameraSort::Bazior_4(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, _vec3 Pos4)
{
	_vec3 Temp = (Pos1 * pow(1.f - m_fAccTime, 3)) + ((3.f*Pos2)*m_fAccTime*pow(1.f - m_fAccTime, 2)) +
		3.f*Pos3*pow(m_fAccTime, 2)*(1.f - m_fAccTime) + Pos4 * pow(m_fAccTime, 3);

	return Temp;
}

_vec3 CCameraSort::CheckBazior(vector<CToolPoint*> vecToolPoint)
{
	/*CTransform* pEyeTransform = dynamic_cast<CTransform*>(m_vecCamAt[iIndex]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));


	_vec3 CamEye = (1 - m_fAccTime)*pEyeTransform->m_vInfo[INFO_POS]; */
	_vec3 Temp;
	int iSize = vecToolPoint.size();
	if (iSize == 1)
	{
		CTransform* pTrans1 = dynamic_cast<CTransform*>(vecToolPoint[0]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		Temp = pTrans1->m_vInfo[INFO_POS];
	}
	else if (iSize == 2)
	{
		CTransform* pTrans1 = dynamic_cast<CTransform*>(vecToolPoint[0]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans2 = dynamic_cast<CTransform*>(vecToolPoint[1]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));

		Temp = Bazior_2(pTrans1->m_vInfo[INFO_POS], pTrans2->m_vInfo[INFO_POS]);
	}
	else if (iSize == 3)
	{
		CTransform* pTrans1 = dynamic_cast<CTransform*>(vecToolPoint[0]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans2 = dynamic_cast<CTransform*>(vecToolPoint[1]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans3 = dynamic_cast<CTransform*>(vecToolPoint[2]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));

		Temp = Bazior_3(pTrans1->m_vInfo[INFO_POS], pTrans2->m_vInfo[INFO_POS],pTrans3->m_vInfo[INFO_POS]);
	}
	else if (iSize == 4)
	{
		CTransform* pTrans1 = dynamic_cast<CTransform*>(vecToolPoint[0]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans2 = dynamic_cast<CTransform*>(vecToolPoint[1]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans3 = dynamic_cast<CTransform*>(vecToolPoint[2]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		CTransform* pTrans4 = dynamic_cast<CTransform*>(vecToolPoint[3]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC));

		Temp = Bazior_4(pTrans1->m_vInfo[INFO_POS], pTrans2->m_vInfo[INFO_POS], pTrans3->m_vInfo[INFO_POS],pTrans4->m_vInfo[INFO_POS]);

	}




	return Temp;
}

void CCameraSort::Free(void)
{
	for (int i = 0; i < m_vecCamAt.size(); i++)

	{
		Engine::Safe_Release(m_vecCamAt[i]);
	}

	for (int i = 0; i < m_vecCamEye.size(); i++)
	{

		Engine::Safe_Release(m_vecCamEye[i]);
	}

	Engine::CGameObject::Free();
}

CCameraSort * CCameraSort::Create(LPDIRECT3DDEVICE9 pGraphicDev, vector<CToolPoint*> vecAtVector, vector<CToolPoint*> vecEyeVector, wstring CameraName, float Time)
{
	CCameraSort* pInstance = new CCameraSort(pGraphicDev);

	pInstance->AddCameraPos(vecAtVector, vecEyeVector, CameraName, Time);


	return pInstance;
}
