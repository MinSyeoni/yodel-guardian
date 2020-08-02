#pragma once
#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)
class CCollider;

typedef struct tagOBB
{
	_vec3			vPoint[8];		// 큐브를 이루는 정점
	_vec3			vCenter;		// 큐브의 센터 지점
	_vec3			vProjAxis[3];	// 큐브의 센터에서 각 면으로 뻗어나가는 방향 벡터
	_vec3			vAxis[3];		// 큐브의 세 면과 평행하는 임의의 축 벡터

}OBB;

typedef struct tagRay
{
	_vec3 vOri;
	_vec3 vDir;
}RAY;

class ENGINE_DLL CMathMgr : public CBase
{
	DECLARE_SINGLETON(CMathMgr)

private:
	explicit CMathMgr();
	virtual ~CMathMgr();
public:
	_bool	Collision_AABB(CCollider* pDstCollider, CCollider * pSrcCollider, _vec3* vDir);
	_bool	Collision_OBB(CCollider* pDstCollider, CCollider * pSrcCollider, _vec3* vDir);
	_bool	Collision_Spere(CCollider* pDstCollider, CCollider * pSrcCollider, _vec3* vDir);
	_int    Collision_Ray(CCollider* pDstCollider);

	_bool   Collision_SpereWithMousePoint(CCollider* pDstCollider, HWND hwnd,float* fDist);
	_bool   Collision_BoxWithMousePoint(CCollider* pDstCollider,HWND hwnd,float* fDist);

	_bool   Collision_BoXWithCamera(CCollider* pDstCollider, float* fDist);
	_bool   Collision_SphereWithCamera(CCollider* pDstCollider, float* fDist);


public:
	void	Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void	Set_Axis(OBB* pObb);

	RAY m_tRay;
private:
	virtual void Free();
};

END
