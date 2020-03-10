#include "Frustom.h"
#include "GraphicDevice.h"
USING(Engine)

IMPLEMENT_SINGLETON(CFrustom)
CFrustom::CFrustom(void)
{
	//원평
	m_vOriPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vOriPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vOriPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vOriPoint[3] = _vec3(-1.f, -1.f, 0.f);

	//근평
	m_vOriPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vOriPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vOriPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vOriPoint[7] = _vec3(-1.f, -1.f, 1.f);

	m_vLightDir = _vec3(-1.0f, -1.f, 1.0f);
}

HRESULT CFrustom::Update_Frustom_Manager()
{
	_matrix matView, matProj,matViewProj;

	matView=CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	matViewProj = matView * matProj;

	//가까운면
		m_vPlane[0].x = matViewProj._14 + matViewProj._13;
		m_vPlane[0].y = matViewProj._24 + matViewProj._23;
		m_vPlane[0].z = matViewProj._34 + matViewProj._33;
		m_vPlane[0].w = matViewProj._44 + matViewProj._43;
	
		m_vPlane[0].Normalize();
//먼 평면
		m_vPlane[1].x = matViewProj._14 - matViewProj._13;
		m_vPlane[1].y = matViewProj._24 - matViewProj._23;
		m_vPlane[1].z = matViewProj._34 - matViewProj._33;
		m_vPlane[1].w = matViewProj._44 - matViewProj._43;

		m_vPlane[1].Normalize();
//왼쪽 평면

		m_vPlane[2].x = matViewProj._14 + matViewProj._11;
		m_vPlane[2].y = matViewProj._24 + matViewProj._21;
		m_vPlane[2].z = matViewProj._34 + matViewProj._31;
		m_vPlane[2].w = matViewProj._44 + matViewProj._41;

		m_vPlane[2].Normalize();
//오른쪽 평면

		m_vPlane[3].x = matViewProj._14 - matViewProj._11;
		m_vPlane[3].y = matViewProj._24 - matViewProj._21;
		m_vPlane[3].z = matViewProj._34 - matViewProj._31;
		m_vPlane[3].w = matViewProj._44 - matViewProj._41;

		m_vPlane[3].Normalize();
//윗 평면
		m_vPlane[4].x = matViewProj._14 - matViewProj._12;
		m_vPlane[4].y = matViewProj._24 - matViewProj._22;
		m_vPlane[4].z = matViewProj._34 - matViewProj._32;
		m_vPlane[4].w = matViewProj._44 - matViewProj._42;
				 
		m_vPlane[4].Normalize();

//아랫 평면

		m_vPlane[5].x = matViewProj._14 + matViewProj._12;
		m_vPlane[5].y = matViewProj._24 + matViewProj._22;
		m_vPlane[5].z = matViewProj._34 + matViewProj._32;
		m_vPlane[5].w = matViewProj._44 + matViewProj._42;
				 
		m_vPlane[5].Normalize();

		Ready_Light_Frustom_Manager();

	return S_OK;
}

HRESULT CFrustom::Ready_Light_Frustom_Manager()
{

	_matrix matViewProjInv, matViewInv;

	_matrix matProj,matView;


	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();
	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();

	matViewProjInv = matView * matProj;
	matViewInv = matView;
	matViewProjInv=XMMatrixInverse(nullptr, matViewProjInv);
	matViewInv=XMMatrixInverse(nullptr, matViewInv);
	ZeroMemory(m_vPoint, sizeof(_vec3) * 8);

	for (int i = 0; i < 8; i++)
	{
		m_vPoint[i].TransformCoord(m_vOriPoint[i], matViewProjInv);

	}

	_vec3 vCamDir;
	_vec3 vCamPos;

	memcpy(&vCamDir, &matViewInv._31, sizeof(_vec3));
	memcpy(&vCamPos, &matViewInv._41, sizeof(_vec3));

	_vec3 vSum = _vec3{ 0.f,0.f,0.f };

	m_vMaxPoint = m_vPoint[0];
	m_vMinPoint = m_vPoint[0];

	for (_int i = 0; i < 8; i++)
	{

		vSum += m_vPoint[i];
	}
	m_vCenPos.x = vSum.x / 8.f;
	m_vCenPos.y = vSum.y / 8.f;
	m_vCenPos.z = vSum.z / 8.f;

	for (_int i = 0; i < 8; i++)
	{
		if (m_vMaxPoint.x < m_vPoint[i].x)
			m_vMaxPoint.x = m_vPoint[i].x;

		if (m_vMaxPoint.y < m_vPoint[i].y)
			m_vMaxPoint.y = m_vPoint[i].y;

		if (m_vMaxPoint.z < m_vPoint[i].z)
			m_vMaxPoint.z = m_vPoint[i].z;


		if (m_vMinPoint.x > m_vPoint[i].x)
			m_vMinPoint.x = m_vPoint[i].x;

		if (m_vMinPoint.y > m_vPoint[i].y)
			m_vMinPoint.y = m_vPoint[i].y;

		if (m_vMinPoint.z > m_vPoint[i].z)
			m_vMinPoint.z = m_vPoint[i].z;
	}
	m_fRadius = _vec3(m_vMaxPoint - m_vMinPoint).Get_Length();

	m_vLightPos = m_vCenPos + ( ( m_vLightDir*-1.f ) * m_fRadius);


	_vec3 vUp, vAsis;

	vUp = _vec3{ 0.f,1.0f,0.f };
	m_matView=XMMatrixLookAtLH(m_vLightPos.Get_XMVECTOR(),_vec3(m_vLightDir+m_vLightPos).Get_XMVECTOR(),vUp.Get_XMVECTOR() );
	m_matProj = XMMatrixOrthographicLH(m_fRadius, m_fRadius, 0.02f, m_fRadius*2.f);
	return S_OK;
}

void CFrustom::Free(void)
{
}
