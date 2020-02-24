#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"
#include "TerrainTex.h"
#include "Transform.h"
#include "Collider.h"
#include "CSCollider.h"
BEGIN(Engine)

class ENGINE_DLL CCalculator : public CComponent
{
public:
	enum State{MaxState,MinState};
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator(void);

public:
	HRESULT	Ready_Calculator(void);
	_float Compute_HeightOnTerrain(const _vec3* pPos, 
									const _vec3* pTerrainVtx, 
									const _ulong& dwCntX, 
									const _ulong& dwCntZ);

	_vec3		Picking_OnTerrain(
									const CTerrainTex* pTerrainVtxCom,
									const CTransform* pTerrainTransCom,POINT pt);

	_int      Picking_OnTerrainMax(const CTerrainTex* pTerrainVtxCom,
		                           const CTransform* pTerrainTransCom, POINT pt,State _eState);

	_int     Picking_Collider(CCollider* pCollider, POINT pt);

	BOOL     Picking_Spere(CSCollider* pCollider, POINT pt,_vec3* vecDist);


private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	_vec3 m_vMax;
	_vec3 m_vMin;
public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9	pGraphicDev);
private:
	virtual void Free(void);

};


END
#endif // Calculator_h__
