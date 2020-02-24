#include "Calculator.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::~CCalculator(void)
{

}

HRESULT Engine::CCalculator::Ready_Calculator(void)
{


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);





	return S_OK;
}

_float Engine::CCalculator::Compute_HeightOnTerrain(const _vec3* pPos,
	const _vec3* pTerrainVtx,
	const _ulong& dwCntX,
	const _ulong& dwCntZ)
{
	_ulong	dwIndex = _ulong(pPos->z / 1.f) * _ulong(dwCntX) + _ulong(pPos->x / 1.f);

	_float	fRatioX = (pPos->x - pTerrainVtx[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtx[dwIndex + dwCntX].z - pPos->z) / 1.f;

	_float	fHeight[4] = {

		pTerrainVtx[dwIndex + dwCntX].y, 
		pTerrainVtx[dwIndex + dwCntX + 1].y,
		pTerrainVtx[dwIndex + 1].y,
		pTerrainVtx[dwIndex].y

	};


	// 오른쪽 위
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// 왼쪽 아래
	else
	{
		return fHeight[0] + (fHeight[2] - fHeight[3]) * fRatioX + (fHeight[3] - fHeight[0]) * fRatioZ;
	}

}

Engine::CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
		Engine::Safe_Release(pInstance);





	return pInstance;
}

void Engine::CCalculator::Free(void)
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}

_vec3 Engine::CCalculator::Picking_OnTerrain(const CTerrainTex* pTerrainVtxCom,const CTransform* pTerrainTransCom,POINT pt)
{

	if (0 > pt.x || 1200 < pt.x || 0 > pt.y || 900 < pt.y)
		return _vec3{ 0.f,0.f,0.f };
	POINT		ptMouse{};

	ptMouse = pt;

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	_matrix			matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix		matWorld;

	pTerrainTransCom->Get_WorldMatrix2(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong	dwVtxCntX = pTerrainVtxCom->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainVtxCom->Get_VtxCntZ();

	const	_vec3*		pTerrainVtxPos = pTerrainVtxCom->Get_VtxPos();

	_ulong	dwVtxIdx[3];
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(vRayPos + (vRayDir*fDist));
			}

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(vRayPos + (vRayDir*fDist));
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);

}

_int CCalculator::Picking_OnTerrainMax(const CTerrainTex * pTerrainVtxCom, const CTransform * pTerrainTransCom, POINT pt, State _eState)
{
	if (0 > pt.x || 1200 < pt.x || 0 > pt.y || 900 < pt.y)
		return 10000;

	POINT		ptMouse{};

	ptMouse = pt;

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	_matrix			matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix		matWorld;

	pTerrainTransCom->Get_WorldMatrix2(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong	dwVtxCntX = pTerrainVtxCom->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainVtxCom->Get_VtxCntZ();

	const	_vec3*		pTerrainVtxPos = pTerrainVtxCom->Get_VtxPos();

	_ulong	dwVtxIdx[3];
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				if(_eState==MaxState)
					return dwIndex + dwVtxCntX + 1;
				else
					return dwIndex + 1;
			
			}

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				if (_eState == MaxState)
					return dwIndex + dwVtxCntX;
				else
					return dwIndex;
			}
		}
	}

	return 0;





}

_int CCalculator::Picking_Collider(CCollider * pCollider, POINT pt)
{
	POINT		ptMouse{};

	ptMouse = pt;

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix matWorld;
	matWorld = pCollider->Get_ColliderWorldMatrix2();

	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
   D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

   m_vMax = pCollider->Get_Max2();
   m_vMin = pCollider->Get_Min2();



   VTXCUBE*		pVtxCube = nullptr;

   m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

   pVtxCube[0].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
   pVtxCube[0].vTex = pVtxCube[0].vPos;

   pVtxCube[1].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
   pVtxCube[1].vTex = pVtxCube[1].vPos;

   pVtxCube[2].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
   pVtxCube[2].vTex = pVtxCube[2].vPos;

   pVtxCube[3].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
   pVtxCube[3].vTex = pVtxCube[3].vPos;


   pVtxCube[4].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
   pVtxCube[4].vTex = pVtxCube[4].vPos;

   pVtxCube[5].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
   pVtxCube[5].vTex = pVtxCube[5].vPos;

   pVtxCube[6].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
   pVtxCube[6].vTex = pVtxCube[6].vPos;

   pVtxCube[7].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
   pVtxCube[7].vTex = pVtxCube[7].vPos;

   m_pVB->Unlock();

   D3DXVECTOR3 VecWorld[8];

   for (int i = 0; i < 8; i++)
   {
	   VecWorld[i] = pVtxCube[i].vPos;

   }


   float	fU, fV, fDist;


   if (D3DXIntersectTri(&VecWorld[1],
	   &VecWorld[2],
	   &VecWorld[0],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {
	  

	   return fDist;
   }

   // 앞 아래
   if (D3DXIntersectTri(&VecWorld[3],
	   &VecWorld[0],
	   &VecWorld[2],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   //오른 위
   if (D3DXIntersectTri(&VecWorld[1],
	   &VecWorld[5],
	   &VecWorld[6],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {
	

	   return fDist;
   }

   //오른 아래
   if (D3DXIntersectTri(&VecWorld[1],
	   &VecWorld[6],
	   &VecWorld[2],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }

   //뒤 위
   if (D3DXIntersectTri(&VecWorld[5],
	   &VecWorld[4],
	   &VecWorld[7],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   //뒤 아래
   if (D3DXIntersectTri(&VecWorld[5],
	   &VecWorld[7],
	   &VecWorld[6],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   //왼 위
   if (D3DXIntersectTri(&VecWorld[4],
	   &VecWorld[0],
	   &VecWorld[3],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   //왼 아래
   if (D3DXIntersectTri(&VecWorld[4],
	   &VecWorld[3],
	   &VecWorld[7],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }

   //위 위
   if (D3DXIntersectTri(&VecWorld[4],
	   &VecWorld[5],
	   &VecWorld[1],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   //위 아래
   if (D3DXIntersectTri(&VecWorld[4],
	   &VecWorld[1],
	   &VecWorld[0],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {
	

	   return fDist;
   }


   //아래 위
   if (D3DXIntersectTri(&VecWorld[3],
	   &VecWorld[2],
	   &VecWorld[6],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {


	   return fDist;
   }
   if (D3DXIntersectTri(&VecWorld[3],
	   &VecWorld[6],
	   &VecWorld[7],
	   &vRayPos, &vRayDir, &fU, &fV, &fDist))
   {
	 

	   return fDist;
   }
   return 0.f;


}

BOOL CCalculator::Picking_Spere(CSCollider * pCollider, POINT pt,_vec3* vecDist)
{
  

	POINT		ptMouse{};

	ptMouse = pt;

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix matWorld;
	matWorld = pCollider->Get_ColliderWorldMatrix2();

	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	BOOL bisTrue;
	float fDist;
	D3DXIntersect(pCollider->m_pMesh, &vRayPos, &vRayDir, &bisTrue, NULL, NULL, NULL, &fDist, NULL, NULL);


	
	


	*vecDist = vRayPos;




	return bisTrue;
}


