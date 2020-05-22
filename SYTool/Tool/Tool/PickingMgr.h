#pragma once

#include "Export_Function.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "StaticObject.h"
#include "ToolCamera.h"

BEGIN(Engine)
class CToolCollider;
END

class CPickingMgr : public CBase
{
    DECLARE_SINGLETON(CPickingMgr)

private:
    explicit CPickingMgr();
    ~CPickingMgr();

public:
    void	        PickTerrainCubePos(D3DXVECTOR3* pOut, const VTXTEX* pTerrainVtx , float fDeap );
	void	        PickTerrainIndex(DWORD* pOutIndex, const VTXTEX* pTerrainVtx);
	void	        SetTerrainSize(DWORD dwCol, DWORD dwRow);
    _float          Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtx, const _ulong& dwCntX, const _ulong& dwCntZ);
    void	        PickingTerrain(D3DXVECTOR3* pOut, const VTXTEX* pVertex, const D3DXMATRIX* pmatWorld);
	void	        PickTerrainTextPos(D3DXVECTOR3 * pOut, const VTXTEX * pTerrainVtx, float fDeap);

	bool	        IsCheckSphereCollider(Engine::CToolCollider* pCollider);
	bool	        IsCheckColiderMesh(const LPD3DXMESH* pMesh, D3DXMATRIX pMeshWorld);
	bool	        IsCheckStaticObjgectMesh(CStaticObject* pMesh, D3DXMATRIX pMeshWorld, _float* pDist, _vec3* pPos);
	_vec3	        IsCheckCollMesh(CStaticObject* pMesh, D3DXMATRIX pMeshWorld);

    void	        Translation_ViewSpace(void);
    void	        Translation_Local(const D3DXMATRIX* pWorld);

private:
    HRESULT	        Initialize(void);
    //_ulong	Release(void);
    void            Free();
public:
	DWORD	m_dwCol;
	DWORD	m_dwRow;
	
    //CInfoSubject*			m_pInfoSubject;
    //CCameraObserver*		m_pCameraObserver;
    RAY						m_tRay;
};