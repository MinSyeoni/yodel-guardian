#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{

}

Engine::CTransform::~CTransform(void)
{

}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDir, &vDir);

	m_vInfo[INFO_POS] += vDir * fSpeed;

	_matrix matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix * CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3		vDirection = *pTargetPos - m_vInfo[INFO_POS];

	_vec3  vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDirection);

	_vec3  vUp;
	_matrix matRot;

	return 	D3DXMatrixRotationAxis(&matRot, &vAxis,
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDirection, &vDirection),
			D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));;
}

void Engine::CTransform::Move_TargetPos(const _vec3* pTargetPos,
	const _float& fSpeed,
	const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < 4; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));


	return S_OK;
}

Engine::CTransform* Engine::CTransform::Create(void)
{
	CTransform*	pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}


void Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	_matrix			matRot[ROT_END];
	
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	



	memcpy(&m_matNRotWorld.m[3][0], &m_vInfo[INFO_POS], sizeof(_vec3));
}

void Engine::CTransform::Free(void)
{

}