#include "MathMgr.h"
#include "Collider.h"
USING(Engine)
IMPLEMENT_SINGLETON(CMathMgr)

CMathMgr::CMathMgr()
{
}


CMathMgr::~CMathMgr()
{
}

_bool CMathMgr::Collision_AABB(CCollider * pDstCollider, CCollider * pSrcCollider, _vec3 * vDir)
{
	_vec3		vDestMin, vDestMax, vSourMin, vSourMax;
	_float		fMin, fMax;

	vDestMin.TransformCoord(*pDstCollider->Get_Min(),pDstCollider->Get_WorldMat());
	vDestMax.TransformCoord(*pDstCollider->Get_Max(), pDstCollider->Get_WorldMat());
	

	vSourMin.TransformCoord(*pSrcCollider->Get_Min(), pSrcCollider->Get_WorldMat());
	vSourMax.TransformCoord(*pSrcCollider->Get_Max(), pSrcCollider->Get_WorldMat());


	// x축에서 바라봤을 때

	fMin = max(vDestMin.x, vSourMin.x);
	fMax = min(vDestMax.x, vSourMax.x);

	if (fMax < fMin)
	{
		if (!pDstCollider->Get_IsCollision())
			pDstCollider->Set_IsCol(false);
		if (!pSrcCollider->Get_IsCollision())
			pSrcCollider->Set_IsCol(false);
		return false;
	}


	// y축에서 바라봤을 때

	fMin = max(vDestMin.y, vSourMin.y);
	fMax = min(vDestMax.y, vSourMax.y);

	if (fMax < fMin)
	{
		if (!pDstCollider->Get_IsCollision())
			pDstCollider->Set_IsCol(false);
		if (!pSrcCollider->Get_IsCollision())
			pSrcCollider->Set_IsCol(false);
		return false;
	}

	// z축에서 바라봤을 때

	fMin = max(vDestMin.z, vSourMin.z);
	fMax = min(vDestMax.z, vSourMax.z);

	if (fMax < fMin)
	{
		if (!pDstCollider->Get_IsCollision())
			pDstCollider->Set_IsCol(false);
		if (!pSrcCollider->Get_IsCollision())
			pSrcCollider->Set_IsCol(false);
		return false;
	}

	pDstCollider->Set_IsCol(true);
	pSrcCollider->Set_IsCol(true);
	return true;
}

_bool CMathMgr::Collision_OBB(CCollider * pDstCollider, CCollider * pSrcCollider, _vec3 * vDir)
{
	_vec3	vTempDir;
	_float	fDistance = 0;

	if (pDstCollider == nullptr
		|| pSrcCollider == nullptr)
		return false;

	_matrix matSrcWorld, matDstWorld;
	matSrcWorld = pSrcCollider->Get_WorldMat();
	matDstWorld = pDstCollider->Get_WorldMat();

	_vec3 vSrcMin, vSrcMax, vDstMin, vDstMax;
	vDstMin.TransformCoord(*pDstCollider->Get_Min(), pDstCollider->Get_WorldMat());
	vDstMax.TransformCoord(*pDstCollider->Get_Max(), pDstCollider->Get_WorldMat());
	vSrcMin.TransformCoord(*pSrcCollider->Get_Min(), pSrcCollider->Get_WorldMat());
	vSrcMax.TransformCoord(*pSrcCollider->Get_Max(), pSrcCollider->Get_WorldMat());

	_vec3 vSrcCenter, vDstCenter, vSrcRect, vDstRect, vSrcAxis[3], vDstAxis[3], vCenterDistance;

	vSrcRect = (vSrcMax - vSrcMin) * 0.5f;
	vSrcCenter = vSrcMin + vSrcRect;
	vDstRect = (vDstMax - vDstMin) * 0.5f;
	vDstCenter = vDstMin + vDstRect;

	vCenterDistance = vDstCenter - vSrcCenter;

	_float fSrcDistance, fDstDistance, fCenterDistance;
	// OBB fCenterDistan충돌
	memcpy(&vSrcAxis[0], &matSrcWorld._11, sizeof(_vec3));
	memcpy(&vSrcAxis[1], &matSrcWorld._21, sizeof(_vec3));
	memcpy(&vSrcAxis[2], &matSrcWorld._31, sizeof(_vec3));
	memcpy(&vDstAxis[0], &matDstWorld._11, sizeof(_vec3));
	memcpy(&vDstAxis[1], &matDstWorld._21, sizeof(_vec3));
	memcpy(&vDstAxis[2], &matDstWorld._31, sizeof(_vec3));

	for (_int i = 0; i < 3; ++i)
	{
		vSrcAxis[i].Normalize();
		vDstAxis[i].Normalize();
	}

	_vec3 vSrcProAxis[3], vDstProAxis[3];
	for (_int i = 0; i < 3; ++i)
	{
		vSrcProAxis[i] = vSrcAxis[i] * fabsf(vSrcRect.Dot(vSrcAxis[i]));
		vDstProAxis[i] = vDstAxis[i] * fabsf(vDstRect.Dot(vDstAxis[i]));
	}


	for (_int i = 0; i < 3; ++i)
	{
		_vec3 vAxis;

		vAxis = vSrcAxis[i];
		fSrcDistance = fabs(vAxis.Dot(vSrcProAxis[0])) + fabs(vAxis.Dot(vSrcProAxis[1])) + fabs(vAxis.Dot(vSrcProAxis[2]));
		fDstDistance = fabs(vAxis.Dot(vDstProAxis[0])) + fabs(vAxis.Dot(vDstProAxis[1])) + fabs(vAxis.Dot(vDstProAxis[2]));
		fCenterDistance = fabs(vAxis.Dot(vCenterDistance));

		if (fCenterDistance > fSrcDistance + fDstDistance)
		{
			if (!pDstCollider->Get_IsCollision())
				pDstCollider->Set_IsCol(false);
			if (!pSrcCollider->Get_IsCollision())
				pSrcCollider->Set_IsCol(false);
			return false;
		}

		_float fTempDistance = 0.f;
		fTempDistance = fSrcDistance + fDstDistance - fCenterDistance;

		if (0 == fDistance)
		{
			fDistance = fTempDistance;
			vTempDir = vAxis;

			if (0 > vAxis.Dot(vCenterDistance))
				vTempDir *= -1.f;
		}
		else
		{
			if (fDistance > fTempDistance)
			{
				fDistance = fTempDistance;
				vTempDir = vAxis;

				if (0 > vAxis.Dot(vCenterDistance))
					vTempDir *= -1.f;
			}
		}

		vAxis = vDstAxis[i];
		fSrcDistance = fabs(vAxis.Dot(vSrcProAxis[0])) + fabs(vAxis.Dot(vSrcProAxis[1])) + fabs(vAxis.Dot(vSrcProAxis[2]));
		fDstDistance = fabs(vAxis.Dot(vDstProAxis[0])) + fabs(vAxis.Dot(vDstProAxis[1])) + fabs(vAxis.Dot(vDstProAxis[2]));
		fCenterDistance = fabs(vAxis.Dot(vCenterDistance));

		if (fCenterDistance > fSrcDistance + fDstDistance)
		{
			if (!pDstCollider->Get_IsCollision())
				pDstCollider->Set_IsCol(false);
			if (!pSrcCollider->Get_IsCollision())
				pSrcCollider->Set_IsCol(false);
			return false;
		}

		fTempDistance = fSrcDistance + fDstDistance - fCenterDistance;

		if (fDistance > fTempDistance)
		{
			fDistance = fTempDistance;
			vTempDir = vAxis;

			if (0 > vAxis.Dot(vCenterDistance))
				vTempDir *= -1.f;
		}
	}

	if (nullptr != vDir)
		*vDir = vTempDir * fDistance;

	pDstCollider->Set_IsCol(true);
	pSrcCollider->Set_IsCol(true);

	return true;
}

_bool CMathMgr::Collision_Spere(CCollider * pDstCollider, CCollider * pSrcCollider, _vec3 * vDir)
{
	return true;
}

void CMathMgr::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
{
}

void CMathMgr::Set_Axis(OBB * pObb)
{
}



void CMathMgr::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy MathMgr");
#endif

}




