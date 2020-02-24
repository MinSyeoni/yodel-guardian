#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"
#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
public:
	enum USINGQUADTREE { NUSE, USE };

private:
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization(void);

public:
	HRESULT	Ready_Optimization(USINGQUADTREE eType, 
								const _ulong& dwCntX, 
								const _ulong& dwCntZ);

public:
	_bool       Is_InFrustum_ForObject(const _vec3* pWorldPos, const _float fRadius);
	void		Is_InFrustum_ForTerrain(const _vec3* pVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										Engine::INDEX32* pIndex,
										_ulong* pTriCnt);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone;
	CFrustum*				m_pFrustum = nullptr;
	CQuadTree*				m_pQuadTree = nullptr;

public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										USINGQUADTREE eType,
										const _ulong& dwCntX, 
										const _ulong& dwCntZ);

	CComponent*					Clone(void);
	virtual		void			Free(void);

};

END
#endif // Optimization_h__
