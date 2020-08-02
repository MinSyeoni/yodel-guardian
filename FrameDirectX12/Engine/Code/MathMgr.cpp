#include "MathMgr.h"
#include "Collider.h"
#include "GraphicDevice.h"
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
	_vec3 vDstPos = pDstCollider->Get_WorldPos();
	_vec3 vSrcPos = pSrcCollider->Get_WorldPos();

	_float vDist = vDstPos.Get_Distance(vSrcPos);
	_float vRadiusDist = pDstCollider->Get_Radius() + pSrcCollider->Get_Radius();

	if (vDist <= vRadiusDist)
		return true;

	return false;
}

_int CMathMgr::Collision_Ray(CCollider* pDstCollider)
{
	
	_matrix matView;

	_vec3 vRayPos,vRayDir;

  matView=	Engine::CGraphicDevice::Get_Instance()->GetViewMatrix();
	
  matView = XMMatrixInverse(nullptr, matView);
  vRayPos = _vec3{ matView._41,matView._42,matView._43 };
  vRayDir = _vec3{ matView._31,matView._32,matView._33 };

  vRayDir.Normalize();



  _matrix matWorld;
  matWorld = pDstCollider->Get_WorldMat();
  _vec3 vPos;
  vPos = pDstCollider->Get_WorldPos();
 
  _matrix matPos,matInv;
  matPos= XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
 matInv= XMMatrixInverse(nullptr,matPos);

 _vec3 vDist = vPos - vRayPos;

 float fDist = vPos.Get_Distance(vRayPos*-1);

 fDist = vDist.Get_Length();

  _vec3 vRayOrigin = vRayPos;
  _vec3 vRayOriginDir = vRayDir;

  vRayOrigin.TransformCoord(vRayPos, matInv);
  vRayOriginDir.TransformNormal(vRayDir, matInv);

  _float fRadius = pDstCollider->Get_Radius()*10.f;
 

  float a = vRayOriginDir.Dot(vRayOriginDir);
  float b = vRayOriginDir.Dot(vRayOrigin);
  float c = (vRayOrigin.x * vRayOrigin.x + vRayOrigin.y * vRayOrigin.y + vRayOrigin.z * vRayOrigin.z);
  float d =( fRadius*0.1f) * (fRadius * 0.1f);
  
  fDist -= (fRadius * 0.1f);
  if (b * b - a * (c-d) >= 0)
  {
	  return (_int)fDist;

  }




	return 999;
}

_bool CMathMgr::Collision_SpereWithMousePoint(CCollider* pDstCollider, HWND hwnd, float* fDist)
{

	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);

	_vec3 vMousePos;


	vMousePos.x = ptMouse.x / (WINSIZEX * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(WINSIZEY * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	_matrix matProj, matView;

	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();
	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();

	matProj = XMMatrixInverse(nullptr, matProj);

	vMousePos.TransformCoord(vMousePos, matProj);

	_vec3 vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	matView = XMMatrixInverse(nullptr, matView);
	vRayDir.TransformNormal(vRayDir, matView);
	vRayPos.TransformCoord(vRayPos, matView);

	BoundingSphere sphere;
	sphere.Center = pDstCollider->Get_WorldPos();
	sphere.Radius = pDstCollider->Get_Radius();

	vRayDir.Normalize();

	bool Collision = sphere.Intersects(vRayPos.Get_XMVECTOR(), vRayDir.Get_XMVECTOR(), *fDist);

	*fDist -= sphere.Radius;

	return Collision;
}
_bool CMathMgr::Collision_BoxWithMousePoint(CCollider* pDstCollider, HWND hwnd, float* fDist)
{


	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);

	_vec3 vMousePos;


	vMousePos.x = ptMouse.x / (WINSIZEX * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(WINSIZEY * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	_matrix matProj, matView;

	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();
	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();

	matProj = XMMatrixInverse(nullptr, matProj);

	vMousePos.TransformCoord(vMousePos, matProj);

	_vec3 vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	matView = XMMatrixInverse(nullptr, matView);
	vRayDir.TransformNormal(vRayDir, matView);
	vRayPos.TransformCoord(vRayPos, matView);


	vRayDir.Normalize();

	_matrix matWorld;

	matWorld = pDstCollider->Get_WorldMat();

	matWorld = XMMatrixInverse(nullptr, matWorld);
	vRayDir.TransformNormal(vRayDir, matWorld);
	vRayPos.TransformCoord(vRayPos, matWorld);



	vRayDir.Normalize();

	BoundingBox Box;

	_vec3 vMin, vMax;
	vMin = *pDstCollider->Get_Min();
	vMax = *pDstCollider->Get_Max();

	Box.CreateFromPoints(Box,vMin.Get_XMVECTOR(), vMax.Get_XMVECTOR());


	return Box.Intersects(vRayPos.Get_XMVECTOR(), vRayDir.Get_XMVECTOR(), *fDist);



}

_bool CMathMgr::Collision_BoXWithCamera(CCollider* pDstCollider, float* fDist)
{
	_matrix matView;

	_vec3 vRayPos, vRayDir;

	matView = Engine::CGraphicDevice::Get_Instance()->GetViewMatrix();

	matView = XMMatrixInverse(nullptr, matView);
	vRayPos = _vec3{ matView._41,matView._42,matView._43 };
	vRayDir = _vec3{ matView._31,matView._32,matView._33 };

	vRayDir.Normalize();

	_matrix matWorld;

	matWorld = pDstCollider->Get_WorldMat();

	matWorld = XMMatrixInverse(nullptr, matWorld);
	vRayDir.TransformNormal(vRayDir, matWorld);
	vRayPos.TransformCoord(vRayPos, matWorld);



	vRayDir.Normalize();

	BoundingBox Box;

	_vec3 vMin, vMax;
	vMin = *pDstCollider->Get_Min();
	vMax = *pDstCollider->Get_Max();

	Box.CreateFromPoints(Box, vMin.Get_XMVECTOR(), vMax.Get_XMVECTOR());


	return Box.Intersects(vRayPos.Get_XMVECTOR(), vRayDir.Get_XMVECTOR(), *fDist);

}

_bool CMathMgr::Collision_SphereWithCamera(CCollider* pDstCollider, float* fDist)
{
	_matrix matView;

	_vec3 vRayPos, vRayDir;

	matView = Engine::CGraphicDevice::Get_Instance()->GetViewMatrix();

	matView = XMMatrixInverse(nullptr, matView);
	vRayPos = _vec3{ matView._41,matView._42,matView._43 };
	vRayDir = _vec3{ matView._31,matView._32,matView._33 };

	vRayDir.Normalize();


	BoundingSphere sphere;
	sphere.Center = pDstCollider->Get_WorldPos();
	sphere.Radius = pDstCollider->Get_Radius();

	vRayDir.Normalize();

	bool Collision = sphere.Intersects(vRayPos.Get_XMVECTOR(), vRayDir.Get_XMVECTOR(), *fDist);

	*fDist -= sphere.Radius;

	return Collision;


}

_bool CMathMgr::Collision_PlayerViewPoint(_vec3 vRayPos, _vec3 vRayDir, CCollider* pDstCollider, float* fDist)
{
	

	_matrix matWorld = pDstCollider->Get_WorldMat();

	matWorld = XMMatrixInverse(nullptr, matWorld);
	vRayDir.TransformNormal(vRayDir, matWorld);
	vRayPos.TransformCoord(vRayPos, matWorld);



	vRayDir.Normalize();

	BoundingBox Box;

	_vec3 vMin, vMax;
	vMin = *pDstCollider->Get_Min();
	vMax = *pDstCollider->Get_Max();

	Box.CreateFromPoints(Box, vMin.Get_XMVECTOR(), vMax.Get_XMVECTOR());

	return Box.Intersects(vRayPos.Get_XMVECTOR(), vRayDir.Get_XMVECTOR(), *fDist);
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




